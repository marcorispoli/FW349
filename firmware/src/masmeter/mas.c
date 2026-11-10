#define _MAS_C

#include "application.h"
#include "mas.h"
#include "../Protocol/protocol.h"
#include "../Shared/CAN/MET_can_protocol.h"

#define TC0_us_TICK                 21.333
#define TC0_ms_to_TICK(x)           (x*(1000/21.333))

// Calibration data
#define INTERNAL_NOMINAL_TEST_mA    205
#define INTERNAL_NOMINAL_TST_PULSES 19473
#define CALIB_INTEGRATION_TIME_ms   1000
#define AUTO_CALIBRATION_ITERATIONS          3

// Masmeter monitoring data
#define MONITOR_INTEGRATION_TIME_ms 1000   


/**
 * This is a One Sho Timer to calculate the current mAs pulses in a timed slot
 * 
 * @param status
 * @param context
 */
static void TCO_MasCallback(TC_TIMER_STATUS status, uintptr_t context){
    
    // Acquisizione del conteggio e azzeramento del registro.
    // Attenzione: non usare TC2_TimerStop perchè in tal caso il registro 
    // dei conteggi non sarà più disponibile sul BUS
    MAS_Data.mas_count = TC2_Timer16bitCounterGet();
    TC2_Timer16bitCounterSet(0);
    
    // Se c'è in corso l'autocalibrazione, aggiunge i conteggi e gestisce il
    // numero di iterazioni
    if(MAS_Data.auto_calibration.status){
        MAS_Data.auto_calibration.cumulated_pulses += MAS_Data.mas_count;
        MAS_Data.auto_calibration.iterations++;
        if(MAS_Data.auto_calibration.iterations == AUTO_CALIBRATION_ITERATIONS){
            MAS_Data.auto_calibration.status = false;
            MAS_Data.auto_calibration.completed = true;
        }
    }
    
    // Se è ancora attiva la modalità di monitoraggio prosegue con il conteggio
    // E si rilancia il timer al prossimo timeot
    if(MAS_Data.mas_test_status){
        TC0_TimerStart();
        // TC2 è in modalità conteggio continuo e non deve essere riacceso
    }else{
        TC2_TimerStop();
    }
    return;
}
/**
 * This function activates the Test current to monitor the status 
 * of the mAsmeter and to allow its automatic calibration.
 * 
 * The Calibration test applies a 2.048V on the VFO circuit input
 * corresponding to the 204.8mA virtual anodic current. 
 * 
 * In case of success activation, the related LED on the 
 * board is activated.
 * 
 * During the Test activation, the input signal frequence  
 * is monitored in order to detected the calibrated value of the mAs
 * and anodic current. The calculation is performed on a base of 1 second.
 * 
 * The following data will be assigned during the test activation:
 * + Every second, the number of pulses will be assigned to the MasTestStatusRegister;
 * + Every second, the calculated anodic current will be assigned to the MasTestStatusRegister;
 * 
 * The calculated anodic current is indirectly calculated using the actual 
 * mAsmeter calibration parameter with the following formula:
 * 
 * - Calibrated mAs: CMas = 1s_Pulses * mAsCalibrationParameter;
 * - Virtual Anodic Current: CIa(mA) = CMas;
 *  
 * @param stat: the current status of the activation
 * @return 
 */
static bool masTestActivation(bool stat){
    
    
    // Attivazione
    if(stat){
        MAS_Data.mas_test_status = true;
        uC_TEST_mAs_Set();
        LED_mAsTest_Set();      
        
        // Attiva One-Shot Timer
        TC0_Timer16bitPeriodSet(TC0_ms_to_TICK(1000));        
        TC0_TimerStart();
        TC0_TimerCommandSet(1);
        
        // Attiva il conteggio
        TC2_Timer16bitCounterSet(0);
        TC2_TimerStart();

    }else{
        MAS_Data.mas_test_status = false;
        uC_TEST_mAs_Clear();
        LED_mAsTest_Clear();        
        TC0_TimerStop();
        TC2_Timer16bitCounterSet(0);
        TC2_TimerStop();        
    }

    // Aggiorna lo STatus register con il valore attuale
    SystemStatusRegister.anodic_test_current_activated = stat;
    encodeStatusRegister(&SystemStatusRegister);
    
    return stat;
}




void MasInit(void){
    REGISTER_STRUCT_t* canreg;
    
    TC0_TimerCallbackRegister( TCO_MasCallback, 0 );
    
    // Reset the autocalibration process
    MAS_Data.auto_calibration.status = false;
    
    // Inizializzazione parametri di default
    MasParamRegister.calibrated_10ms_time = CALIB_INTEGRATION_TIME_ms/10;
    MasParamRegister.calibrated_ma = INTERNAL_NOMINAL_TEST_mA;
    MasParamRegister.calibrated_L = INTERNAL_NOMINAL_TST_PULSES & 0x00FF;
    MasParamRegister.calibrated_H = (INTERNAL_NOMINAL_TST_PULSES>>8) & 0x00FF;
    canreg = (REGISTER_STRUCT_t*) & MasParamRegister;
    
    // Set del valore di default per questo registro
    MET_Can_Protocol_SetDefaultParameter(canreg->idx,canreg->d0, canreg->d1, canreg->d2, canreg->d3);
    
    // Get the actual value: the default value is rejected in the case the EEPROM should be modified
    decodeParamRegister(&MasParamRegister);
    
    // Inizializza il test 
    masTestActivation(false);
    
}

/**
 * Function returning the calibrated Anodic current (mA) from the current counted pulses in a given integration time.
 * 
 * The Anodic current is calculated based on calibration paramters.
 * 
 * In the calibration fase, a predefined current is used and the total count is stored 
 * with the calibration integration time. 
 * 
 * As default, the parameters are set as follows:
 * + Nominal current = 204mA: this is the nominal virtual current with the test voltage (2.04V) in the VFO input.
 * + Integration time = 1second.
 * + Pulses: pulses measured in a test board.
 * 
 * Further calibrations may change the defaut value to a more accurate value.
 * 
 * @param pulses: the pulses used to estimate the anodic current
 * @param integration_10ms_time: the integration time used to accumulate the pulses.
 * @return 
 */
unsigned short getCalibratedAnodicCurrent(unsigned short pulses, unsigned short integration_10ms_time){
    
    // Aggiorna il contenuto del registro dalla struttura del CAN bus
    decodeParamRegister(&MasParamRegister);
    
    // Ricava il numero i impulsi rilevati in calibrazione con una corrente nota ed un tempo di integrazione noto.
    float calibrated_test_pulse = (float) ((unsigned short) MasParamRegister.calibrated_L + (unsigned short) MasParamRegister.calibrated_H * 256);
    
    // Scala rispetto al tempo di integrazione attuale rispetto a quello usato in calibrazione
    float ki = (float) integration_10ms_time / MasParamRegister.calibrated_10ms_time ;
    
    return (unsigned short) ((float) pulses * (float) MasParamRegister.calibrated_ma * ki /(calibrated_test_pulse));
    
}

void ComputeAutoCalibration(void){
    MasParamRegister.calibrated_10ms_time = MAS_Data.auto_calibration.iteration_time_slot_10ms;
    unsigned short pulses = MAS_Data.auto_calibration.cumulated_pulses/(MAS_Data.auto_calibration.iterations);
    MasParamRegister.calibrated_H = pulses >>8;
    MasParamRegister.calibrated_L = pulses & 0x00FF;
    MasParamRegister.calibrated_ma = MAS_Data.auto_calibration.anodic_current_test;
    encodeParamRegister(&MasParamRegister);
    MET_Can_Protocol_StoreParameters();
    return;
    
}

/**
 * RTC GEneral Purpose timer to handle the Mas Activities in Idle
 */
void rtcMas15msCallback(void){
    unsigned short anodic_test_current;
    
    // Update the SystemDataRegister
    decodeDataRegister(&SystemDataRegister);
    
    // CurrentTest and Mas Calibration
    if(SystemDataRegister.activate_mas_test_current != MAS_Data.mas_test_status ) masTestActivation(SystemDataRegister.activate_mas_test_current);
    
    // Durante il test, il valore degli impulsi e della corrente anodica stimata viene aggiornato 
    // nella struttura dati e nei registri del protocollo Can
    if(MAS_Data.mas_test_status){
        // Aggiorna il conteggio sul registro
        MasTestStatusRegister.mas_test_pulses_H = ((MAS_Data.mas_count >> 8) & 0x00FF);
        MasTestStatusRegister.mas_test_pulses_L = MAS_Data.mas_count & 0x00FF;  
        
        anodic_test_current = getCalibratedAnodicCurrent(MAS_Data.mas_count, MONITOR_INTEGRATION_TIME_ms/10) ;
        MasTestStatusRegister.anodic_test_current_L = anodic_test_current;
        MasTestStatusRegister.anodic_test_current_H = (anodic_test_current>>8);
        encodeStatusRegister(&MasTestStatusRegister);
        
        // Gestione del termine auto-calibrazione
        if(MAS_Data.auto_calibration.completed){
            MAS_Data.auto_calibration.completed = false;
            ComputeAutoCalibration();
            
            SystemStatusRegister.auto_calibration_activated = 0;
            encodeStatusRegister(&SystemStatusRegister);
        }
    }
    
    
    
}


bool MasAutoCalibration(void){
    
    // Stops the running anodic test 
    if(MAS_Data.mas_test_status) masTestActivation(false);
    
    MAS_Data.auto_calibration.anodic_current_test = INTERNAL_NOMINAL_TEST_mA;
    MAS_Data.auto_calibration.cumulated_pulses = 0;
    MAS_Data.auto_calibration.iterations = 0;
    MAS_Data.auto_calibration.iteration_time_slot_10ms = CALIB_INTEGRATION_TIME_ms/10;
    
    // Uses the default calibration parameters to execute the calibration
    MAS_Data.auto_calibration.status = true;
    MAS_Data.auto_calibration.completed = false;
    SystemStatusRegister.auto_calibration_activated = 1;
    encodeStatusRegister(&SystemStatusRegister);
    
    masTestActivation(true);    
    return true;
}


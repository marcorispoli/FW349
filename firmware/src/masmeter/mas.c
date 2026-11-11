#define _MAS_C

#include "application.h"
#include "mas.h"
#include "../Protocol/protocol.h"
#include "../Shared/CAN/MET_can_protocol.h"

#define TC0_us_TICK                 21.333
#define TC0_ms_to_TICK(x)           (x*(1000/21.333))

// Monitor and Calibration data
#define INTERNAL_NOMINAL_TEST_mA                205
#define INTERNAL_NOMINAL_TST_PULSES             19473
#define CALIB_INTEGRATION_TIME_ms               1000
#define AUTO_CALIBRATION_ITERATIONS             3
#define MANUAL_CALIBRATION_ITERATIONS           4


static bool masMonitorActivation(bool stat);
/**
 * This is a One Sho Timer to calculate the current mAs pulses in a timed slot
 * 
 * @param status
 * @param context
 */
static void TCO_MasCallback(TC_TIMER_STATUS status, uintptr_t context){
    
    // Se c'è in corso l'autocalibrazione, aggiunge i conteggi e gestisce il
    // numero di iterazioni
    if(MAS_Data.auto_calibration.status){
        MAS_Data.auto_calibration.cumulated_pulses += TC2_Timer16bitCounterGet();
        TC2_Timer16bitCounterSet(0);
        MAS_Data.auto_calibration.iterations++;
        if(MAS_Data.auto_calibration.iterations == AUTO_CALIBRATION_ITERATIONS){
            MAS_Data.auto_calibration.status = false;
            MAS_Data.auto_calibration.completed = true;
        }
    }else if(MAS_Data.manual_calibration.status){ // MAnual Calibration process
        MAS_Data.manual_calibration.cumulated_pulses += TC2_Timer16bitCounterGet();
        TC2_Timer16bitCounterSet(0);
        MAS_Data.manual_calibration.iterations++;
        if(MAS_Data.manual_calibration.iterations == MANUAL_CALIBRATION_ITERATIONS){
            MAS_Data.manual_calibration.status = false;
            MAS_Data.manual_calibration.completed = true;
        }
    }else if(MAS_Data.monitor.status){
        MAS_Data.monitor.pulses = TC2_Timer16bitCounterGet();
        TC2_Timer16bitCounterSet(0);
    }
    
    return;
}


void MasInit(void){
    REGISTER_STRUCT_t* canreg;
    
    TC0_TimerCallbackRegister( TCO_MasCallback, 0 );
    
    // Reset the auto and manual calibration process
    MAS_Data.auto_calibration.status = false;
    MAS_Data.manual_calibration.status = false;
    
    // inizializzazione struttura per monitoraggio del masmetro
    MAS_Data.monitor.status = false;
    MAS_Data.monitor.anodic_current_test = INTERNAL_NOMINAL_TEST_mA;
    MAS_Data.monitor.pulses = 0;
    MAS_Data.monitor.iteration_time_slot_10ms = CALIB_INTEGRATION_TIME_ms/10;
    
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
    
    // Disattivazione Del Monitoraggio
    masMonitorActivation(false);
    
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

void ComputeManualCalibration(void){
    MasParamRegister.calibrated_10ms_time = MAS_Data.manual_calibration.iteration_time_slot_10ms;
    unsigned short pulses = MAS_Data.manual_calibration.cumulated_pulses/(MAS_Data.manual_calibration.iterations);
    MasParamRegister.calibrated_H = pulses >>8;
    MasParamRegister.calibrated_L = pulses & 0x00FF;
    MasParamRegister.calibrated_ma = MAS_Data.manual_calibration.anodic_current_test;
    encodeParamRegister(&MasParamRegister);
    MET_Can_Protocol_StoreParameters();
    return;
    
}

/**
 * RTC GEneral Purpose timer to handle the Mas Activities in Idle
 */
void rtcMas15msCallback(void){
    unsigned short anodic_current;
    
    // Update the SystemDataRegister
    decodeDataRegister(&SystemDataRegister);
    
    // Test Current Activation control: not during the manual or automatic calibration
    if(! ((MAS_Data.auto_calibration.status) || (MAS_Data.manual_calibration.status))){
        if(SystemDataRegister.activate_mas_monitor_current != MAS_Data.monitor.status ) masMonitorActivation(SystemDataRegister.activate_mas_monitor_current);

        // Durante il monitoring, il valore degli impulsi e della corrente anodica stimata viene aggiornato 
        // nella struttura dati e nei registri del protocollo Can
        if(MAS_Data.monitor.status){
            // Aggiorna il conteggio sul registro
            MasMonitorStatusRegister.mas_pulses_H = ((MAS_Data.monitor.pulses >> 8) & 0x00FF);
            MasMonitorStatusRegister.mas_pulses_L = MAS_Data.monitor.pulses & 0x00FF;  

            anodic_current = getCalibratedAnodicCurrent(MAS_Data.monitor.pulses, MAS_Data.monitor.iteration_time_slot_10ms) ;
            MasMonitorStatusRegister.anodic_current_L = anodic_current;
            MasMonitorStatusRegister.anodic_current_H = (anodic_current>>8);
            encodeStatusRegister(&MasMonitorStatusRegister);

        }
    }
    
    // Gestione del termine auto-calibrazione
    if(MAS_Data.auto_calibration.completed){
        MAS_Data.auto_calibration.completed = false;
        ComputeAutoCalibration();

        SystemStatusRegister.auto_calibration_activated = 0;
        encodeStatusRegister(&SystemStatusRegister);
    }

    // Gestione del termine manual-calibrazione
    if(MAS_Data.manual_calibration.completed){
        MAS_Data.manual_calibration.completed = false;
        ComputeManualCalibration();

        SystemStatusRegister.manual_calibration_activated = 0;
        encodeStatusRegister(&SystemStatusRegister);
    }
    
}


bool MasAutoCalibration(void){
    
    // Stop Timer 0 per resettare il periodo di integrazione
    TC0_TimerStop();
    
    masMonitorActivation(false);
    
    MAS_Data.auto_calibration.anodic_current_test = INTERNAL_NOMINAL_TEST_mA;
    MAS_Data.auto_calibration.cumulated_pulses = 0;
    MAS_Data.auto_calibration.iterations = 0;
    MAS_Data.auto_calibration.iteration_time_slot_10ms = CALIB_INTEGRATION_TIME_ms/10;
    
    // Uses the default calibration parameters to execute the calibration
    MAS_Data.auto_calibration.status = true;
    MAS_Data.auto_calibration.completed = false;
    SystemStatusRegister.auto_calibration_activated = 1;
    encodeStatusRegister(&SystemStatusRegister);
    
    // Attiva la corrente di test che serve per Auto Calibrazione
    masMonitorActivation(true);    
    
    return true;
}

bool MasManualCalibration(unsigned char anodic_current){
    
    // Stop Timer 0 per resettare il periodo di integrazione
    TC0_TimerStop();
    
    // Stops the running anodic test 
    masMonitorActivation(false);
    
    MAS_Data.manual_calibration.anodic_current_test = anodic_current;
    MAS_Data.manual_calibration.cumulated_pulses = 0;
    MAS_Data.manual_calibration.iterations = 0;
    MAS_Data.manual_calibration.iteration_time_slot_10ms = CALIB_INTEGRATION_TIME_ms/10;
    
    // Uses the default calibration parameters to execute the calibration
    MAS_Data.manual_calibration.status = true;
    MAS_Data.manual_calibration.completed = false;

    SystemStatusRegister.manual_calibration_activated = 1;
    encodeStatusRegister(&SystemStatusRegister);

    // Attivazione timer e counter
    TC0_Timer16bitPeriodSet(TC0_ms_to_TICK(CALIB_INTEGRATION_TIME_ms));        
    TC0_TimerStart();
    TC0_TimerCommandSet(1);

    // Attiva il conteggio
    TC2_Timer16bitCounterSet(0);
    TC2_TimerStart();
    return true;
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
bool masMonitorActivation(bool stat){
    
    // Stop Timer 0 per resettare il periodo di integrazione
    TC0_TimerStop();
    
    // Attivazione
    if(stat){
        MAS_Data.monitor.status = true;
        uC_TEST_mAs_Set();
        LED_mAsTest_Set();      

        // Attivazione timer e counter
        TC0_Timer16bitPeriodSet(TC0_ms_to_TICK(CALIB_INTEGRATION_TIME_ms));        
        TC0_TimerStart();
        TC0_TimerCommandSet(1);
        
        // Attiva il conteggio
        TC2_Timer16bitCounterSet(0);
        TC2_TimerStart();

    }else{
        MAS_Data.monitor.status = false;
        uC_TEST_mAs_Clear();
        LED_mAsTest_Clear();        
        
        // Disattiva il conteggio
        TC2_Timer16bitCounterSet(0);
        TC2_TimerStop();        
    }

    // Aggiorna lo STatus register con il valore attuale
    SystemStatusRegister.anodic_monitor_current_activated = stat;
    encodeStatusRegister(&SystemStatusRegister);    
    return stat;
}



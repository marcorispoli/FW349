#define _MAS_C

#include "application.h"
#include "mas.h"
#include "../Protocol/protocol.h"

#define TC0_us_TICK                 21.333
#define TC0_ms_to_TICK(x)           (x*(1000/21.333))
#define INTERNAL_NOMINAL_TEST_mA    205
#define INTERNAL_NOMINAL_TST_PULSES 19473
#define CALIB_INTEGRATION_TIME      1000


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
    
    // Inizializzazione parametri di default
    MasParamRegister.calibrated_10ms_time = CALIB_INTEGRATION_TIME/10;
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
 * RTC GEneral Purpose timer to handle the Mas Activities in Idle
 */
void rtcMas15msCallback(void){
    // Update the SystemDataRegister
    decodeDataRegister(&SystemDataRegister);
    
    // CurrentTest and Mas Calibration
    if(SystemDataRegister.activate_mas_test_current != MAS_Data.mas_test_status ) masTestActivation(SystemDataRegister.activate_mas_test_current);
    
    // Aggiorna il conteggio sul registro
    MasTestStatusRegister.mas_test_pulses_H = ((MAS_Data.mas_count >> 8) & 0x00FF);
    MasTestStatusRegister.mas_test_pulses_L = MAS_Data.mas_count & 0x00FF;
    
    
    unsigned short calibrated_test_pulse = (unsigned short) MasParamRegister.calibrated_L + (unsigned short) MasParamRegister.calibrated_H * 256;
    
    unsigned short anodic_test_current = (unsigned short) ((float) MAS_Data.mas_count * (float) MasParamRegister.calibrated_ma / (float) calibrated_test_pulse);
    
    MasTestStatusRegister.anodic_test_current_L = anodic_test_current;
    MasTestStatusRegister.anodic_test_current_H = (anodic_test_current>>8);
    
            
    encodeStatusRegister(&MasTestStatusRegister);
    
    
    
}

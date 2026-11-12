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

#define EXPOSURE_MONITOR_INTEGRATION_TIME_ms    10

static void masVrefActivation(bool stat);
static void masIntegrationStart(unsigned char period_10ms);
static void masIntegrationStop(void);
static void MasInternalCurrentMonitor(bool stat, bool internal_vref);

/**
 * This is a One Sho Timer to calculate the current mAs pulses in a timed slot
 * 
 * @param status
 * @param context
 */
static void TCO_MasCallback(TC_TIMER_STATUS status, uintptr_t context){
    
    // La funzione di monitoraggio è sempre attiva e contiene il valore accumulato in 
    // un singolo time slot di integrazione.    
    MAS_Data.monitor.pulses = TC2_Timer16bitCounterGet();
    TC2_Timer16bitCounterSet(0);
        
    // Se c'è in corso una calibrazione, aggiunge i conteggi e gestisce il
    // numero di iterazioni
    if(MAS_Data.calibration.status){
        MAS_Data.calibration.cumulated_pulses += MAS_Data.monitor.pulses;
        MAS_Data.calibration.iterations++;
        if(MAS_Data.calibration.iterations == MAS_Data.calibration.num_iterations){
            MAS_Data.calibration.status = false;
            MAS_Data.calibration.completed = true;
            }
    }
    
    return;
}


void MasInit(void){
    REGISTER_STRUCT_t* canreg;
    
    TC0_TimerCallbackRegister( TCO_MasCallback, 0 );
    
    // Reset the auto and manual calibration process
    MAS_Data.calibration.status = false;
    SystemStatusRegister.mas_auto_calibration_activated = 0;
    SystemStatusRegister.mas_manual_calibration_activated = 0;
    
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
        
    // inizializzazione struttura per monitoraggio del masmetro    
    MAS_Data.monitor.pulses = 0;
    MAS_Data.monitor.integration_time_10ms = MasParamRegister.calibrated_10ms_time;

    // Ferma l'integrazione
    TC0_TimerStop();        
    TC2_TimerStop(); 
    MAS_Data.monitor.integration_status = false;
     
    // Spegne la tensione di riferimento
    MAS_Data.monitor.vref_stat = false;
    uC_TEST_mAs_Clear();
    LED_mAsTest_Clear();        
    SystemStatusRegister.mas_vref_activated = false;

    // Spegne Monitor Interno 
    MAS_Data.monitor.status = false;
    
    // Aggiorna lo status register
    encodeStatusRegister(&SystemStatusRegister);    
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

void ComputeCalibrationResult(void){
    MasParamRegister.calibrated_10ms_time = MAS_Data.calibration.integration_time_10ms;
    unsigned short pulses = MAS_Data.calibration.cumulated_pulses/(MAS_Data.calibration.num_iterations);
    MasParamRegister.calibrated_H = pulses >>8;
    MasParamRegister.calibrated_L = pulses & 0x00FF;
    MasParamRegister.calibrated_ma = MAS_Data.calibration.anodic_current_test;
    encodeParamRegister(&MasParamRegister);
    MET_Can_Protocol_StoreParameters();
    return;
    
}


/**
 * RTC GEneral Purpose timer to handle the Mas Activities in Idle
 */
void rtcMas15msCallback(void){
   
    // Update the SystemDataRegister
    decodeDataRegister(&SystemDataRegister);
    
    // Il monitoraggio tramite tensione di riferimento non deve essere eseguito durante la calibrazione in corso.
    if(!MAS_Data.calibration.status){
        if(
           (SystemDataRegister.mas_internal_monitor != MAS_Data.monitor.status ) || 
           (SystemDataRegister.mas_monitor_with_vref != MAS_Data.monitor.vref_stat )
          )
            MasInternalCurrentMonitor(SystemDataRegister.mas_internal_monitor,SystemDataRegister.mas_monitor_with_vref);

        // Durante il monitoring, il valore degli impulsi e della corrente anodica stimata viene aggiornato 
        // nella struttura dati e nei registri del protocollo Can
        if(MAS_Data.monitor.status){
            // Aggiorna il conteggio sul registro
            MasMonitorStatusRegister.mas_counts_H = ((MAS_Data.monitor.pulses >> 8) & 0x00FF);
            MasMonitorStatusRegister.mas_counts_L = MAS_Data.monitor.pulses & 0x00FF;  

            MAS_Data.monitor.anodic_current = getCalibratedAnodicCurrent(MAS_Data.monitor.pulses, MAS_Data.monitor.integration_time_10ms) ;
            MasMonitorStatusRegister.mas_anodic_current_L = MAS_Data.monitor.anodic_current;
            MasMonitorStatusRegister.mas_anodic_current_H = (MAS_Data.monitor.anodic_current>>8);
            encodeStatusRegister(&MasMonitorStatusRegister);

        }
    }
    
    // Gestione del termine auto-calibrazione
    if(MAS_Data.calibration.completed){
        MAS_Data.calibration.completed = false;
        ComputeCalibrationResult();

        SystemStatusRegister.mas_auto_calibration_activated = 0;
        SystemStatusRegister.mas_manual_calibration_activated = 0;
        encodeStatusRegister(&SystemStatusRegister);
    }
    
}


bool MasAutoCalibration(void){
    
    // Ferma l'esecusione del monitoraggio interno
    MasInternalCurrentMonitor(false,false);
        
    // Set the content of the calibration structure
    MAS_Data.calibration.anodic_current_test = INTERNAL_NOMINAL_TEST_mA;
    MAS_Data.calibration.cumulated_pulses = 0;
    MAS_Data.calibration.iterations = 0;
    MAS_Data.calibration.num_iterations = AUTO_CALIBRATION_ITERATIONS;    
    MAS_Data.calibration.integration_time_10ms = CALIB_INTEGRATION_TIME_ms/10;
    MAS_Data.calibration.calibration_mode = MAS_AUTO_CALIBRATION;
    
    // Uses the default calibration parameters to execute the calibration
    MAS_Data.calibration.status = true;
    MAS_Data.calibration.completed = false;
    SystemStatusRegister.mas_auto_calibration_activated = 1;
    SystemStatusRegister.mas_manual_calibration_activated = 0;
        
    // Attiva la tensione di riferimento
    masVrefActivation(true);
    
    // Attiva l'integrazione
    masIntegrationStart( MAS_Data.calibration.integration_time_10ms );   
    
    encodeStatusRegister(&SystemStatusRegister);
    return true;
    
}

bool MasManualCalibration(unsigned char anodic_current){
    
    // Ferma l'esecusione del monitoraggio interno
    MasInternalCurrentMonitor(false, false);
    
    // Set the content of the calibration structure
    MAS_Data.calibration.anodic_current_test = anodic_current;
    MAS_Data.calibration.cumulated_pulses = 0;
    MAS_Data.calibration.iterations = 0;
    MAS_Data.calibration.num_iterations = MANUAL_CALIBRATION_ITERATIONS;    
    MAS_Data.calibration.integration_time_10ms = CALIB_INTEGRATION_TIME_ms/10;
    MAS_Data.calibration.calibration_mode = MAS_MANUAL_CALIBRATION;
    
    // Uses the default calibration parameters to execute the calibration
    MAS_Data.calibration.status = true;
    MAS_Data.calibration.completed = false;
    SystemStatusRegister.mas_auto_calibration_activated = 0;
    SystemStatusRegister.mas_manual_calibration_activated = 1;
    
    masIntegrationStart( MAS_Data.calibration.integration_time_10ms );   
    
    encodeStatusRegister(&SystemStatusRegister);
    return true;
}

void MasInternalCurrentMonitor(bool stat, bool internal_vref){
   
    // Stop and reset the timer for the pulse integration
    masIntegrationStop();
    
    // Attivazione
    if(stat){    
        // Switch off the internal vref
        masVrefActivation(internal_vref);
        masIntegrationStart( MAS_Data.monitor.integration_time_10ms );   

    }else{       
        // Switch off the internal vref
        masVrefActivation(false);        
    }
    
    MAS_Data.monitor.status = stat;
    SystemStatusRegister.mas_monitor_activated = stat;
    encodeStatusRegister(&SystemStatusRegister);
    return;
}



void masIntegrationStart(unsigned char period_10ms){
    if(MAS_Data.monitor.integration_status) return;
    
    // Attivazione timer e counter
    TC0_Timer16bitPeriodSet(TC0_ms_to_TICK(period_10ms * 10));        
    TC0_TimerStart();
    TC0_TimerCommandSet(1);

    // Attiva il conteggio
    TC2_Timer16bitCounterSet(0);
    TC2_TimerStart();
    MAS_Data.monitor.integration_status = true;
    return;
}

void masIntegrationStop(void){
    if(!MAS_Data.monitor.integration_status) return;
    
    TC0_TimerStop();        
    TC2_TimerStop();                
    MAS_Data.monitor.integration_status = false;
    return;
}

void masVrefActivation(bool stat){
    if(MAS_Data.monitor.vref_stat == stat) return;
  
    // Attivazione
    if(stat){
        MAS_Data.monitor.vref_stat = true;
        uC_TEST_mAs_Set();
        LED_mAsTest_Set();      

    }else{
        MAS_Data.monitor.vref_stat = false;
        uC_TEST_mAs_Clear();
        LED_mAsTest_Clear();        
        
    }

    // Aggiorna lo STatus register con il valore attuale
    SystemStatusRegister.mas_vref_activated = stat;
    encodeStatusRegister(&SystemStatusRegister);    
    return ;
}

#define _STARTER_C

#include "application.h"
#include "starter.h"

#define INIT_BLANKING_TIME 100 
#define RESET_FAULT_ATTEMPTS  4

static void StarterHandleResetFault(void);
static void StarterHandleHsRun(void);
static void StarterHandleLsRun(void);


        
void rtcStarter15msCallback(void){   
    
    // Aggiorna lo stato dei segnali in input
    _Starter_Data.fault = uC_STARTER_FAULT_Get();
    //_Starter_Data.speed_ok = uC_STARTER_SPEED_OK_Get();
    
    // Nessun comando viene processato durante il blanking timer
    if(_Starter_Data.command.blanking_timer){
        _Starter_Data.command.blanking_timer--;
        return;
    }
    
    switch(_Starter_Data.command.cmd){
        case STARTER_STOP:
            break;
        case STARTER_RESET_FAULT: StarterHandleResetFault(); break;
        case STARTER_RUN_HS: StarterHandleHsRun(); break;
        case STARTER_RUN_LS: StarterHandleLsRun(); break;
    }
    return;
}


void StarterInit(void){
    
    uC_STARTER_RUN_Clear();
    uC_STARTER_FREERUN_Clear();
    uC_STARTER_SPEED_Clear();
    _Starter_Data.fault = uC_STARTER_FAULT_Get();
    _Starter_Data.speed_ok = false;
    _Starter_Data.stop_timer = 0;
    
    _Starter_Data.command.blanking_timer = INIT_BLANKING_TIME;
    _Starter_Data.command.cmd = STARTER_STOP;
    _Starter_Data.command.executed = true;
    _Starter_Data.command.sequence = 0;
  
    // Se viene rilevata una condizione di Fault all'accensione 
    // si attiva la procedura di reset, ma solo all'esaurimento del blanking timer
    StarterResetFault();
    
    return;
}

/**
 * This function activate the Reset Fault Procedure. 
 * 
 * The Reset Fault Procedure starts only if a fault condition 
 * is actually present.
 * 
 */
void StarterResetFault(void){
    if(!_Starter_Data.fault) return;
    
    _Starter_Data.command.sequence = 0;    
    _Starter_Data.command.cmd = STARTER_RESET_FAULT;
    _Starter_Data.command.executed = false;
    _Starter_Data.command.attempt = RESET_FAULT_ATTEMPTS;
    
    return;
}
/**
 * This is the Fault Reset Procedure.
 * 
 * The Fault Reset Procedure for the Sedecal Starter 
 * is composed by the following signal sequence:
 * + RUN = 0 and FREERUN = 0;
 * + RUN = 0 and FREERUN = 1;
 * 
 * The Function insertws a 15ms delay in between
 * then waits almost 30ms before to check if the FAULT condition expires.
 * 
 * The procedure attempts four times to reset the fault condition, 
 * with a 100ms interval between any attempts. 
 * 
 * The Procedure termines in STOP whether the fault is reset or not.  
 */
void StarterHandleResetFault(void){
    switch(_Starter_Data.command.sequence){
        case 0:
            // Azzera RUN e FREERUN
            uC_STARTER_RUN_Clear();
            uC_STARTER_FREERUN_Clear();
            _Starter_Data.speed_ok = false;
            
            _Starter_Data.command.blanking_timer = 1; // Attende 15ms            
            _Starter_Data.command.sequence++;            
            break;
            
        case 1:
            // Set FREERUN
            uC_STARTER_FREERUN_Set();
            _Starter_Data.blanking_timer = 2; // Attende 30ms
            
            _Starter_Data.command_sequence++;            
            break;
            
        case 2:
            if(_Starter_Data.fault){
                if(_Starter_Data.command_attempt ){
                    // Ripete la sequenza dopo circa 100ms
                    _Starter_Data.command_attempt--;
                    _Starter_Data.command_sequence = 0;
                    _Starter_Data.blanking_timer = 7;
                    break;
                }
            }
            
            // Fine sequenza
            _Starter_Data.current_command = STARTER_STOP;
            uC_STARTER_RUN_Clear();
            uC_STARTER_FREERUN_Clear();
            break;
    }
}

bool StarterHsRun(void){
    
    // Il comando è già in esecuzione
    if(_Starter_Data.current_command == STARTER_RUN_HS) return true;
    
    _Starter_Data.command_sequence = 0;
    _Starter_Data.speed_ok = false;    
    _Starter_Data.current_command = STARTER_RUN_HS;
    _Starter_Data.command_attempt = 1;
    
    return;
}

void StarterHandleHsRun(void){

}

void StarterHandleLsRun(void){

}

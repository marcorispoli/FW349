#define _STARTER_C

#include "application.h"
#include "starter.h"
#include "Protocol/protocol.h"

// Definizioni delle macro per trasformare i tempi in unità di lacio della callback
#define TIMER_TICK_ms (15.64)
#define SEC_TO_TICK(x)((unsigned short) ((float) x * 1000 / TIMER_TICK_ms))
#define MS_TO_TICK(x)((unsigned short) ((float) x/ TIMER_TICK_ms))

#define RESET_FAULT_ATTEMPTS  4

// Definisce i valori di default per il parametero dei timeouts
#define DEFAULT_HS_TIMEOUT_s 300
#define DEFAULT_LS_TIMEOUT_s 120

// Definisce i timeout per l'esecuzione dei comandi di attivazione roazioni
#define HS_COMMAND_TMO_ms 5000
#define LS_COMMAND_TMO_ms 3000

// Definisce la logica di attivazione del pin di impostazione della velocità
#define SET_HS_SPEED uC_STARTER_SPEED_Clear()
#define SET_LS_SPEED uC_STARTER_SPEED_Set()
#define HS_SPEED_MODE true
#define LS_SPEED_MODE false

static void StarterHandleResetFault(void);
static void StarterHandleHsRun(void);
static void StarterHandleLsRun(void);

static void SetLeds(_LED_STAT_t ls, _LED_STAT_t hs);
static void SetStatus(_STARTER_STATUS_t stat);
static void SetFreeMode(bool stat);
static void SetRunMode(bool stat);
static void SetSpeedMode(bool hs);
static bool GetFaultInput(void);
static bool GetSpeedOkInput(void);
static void SetFaultStatus(void);



void SetStatus(_STARTER_STATUS_t stat){
    Starter_Data.status = stat;
    SystemStatusRegister.starter_status = (unsigned char) (stat & 0x7);
    encodeStatusRegister(&SystemStatusRegister); 
}

void SetFreeMode(bool stat){
    if(stat) uC_STARTER_FREERUN_Set();
    else uC_STARTER_FREERUN_Clear();
    
    Starter_Data.free_output = stat;
    SystemStatusRegister.starter_free_mode_output = stat;
    encodeStatusRegister(&SystemStatusRegister); 
}
void SetRunMode(bool stat){
    if(stat) uC_STARTER_RUN_Set();
    else uC_STARTER_RUN_Clear();
    
    Starter_Data.run_output = stat;
    SystemStatusRegister.starter_run_mode_output = stat;
    encodeStatusRegister(&SystemStatusRegister); 
}

void SetSpeedMode(bool mode){
    if(mode==HS_SPEED_MODE) SET_HS_SPEED;
    else   SET_LS_SPEED;
    Starter_Data.speed_output = uC_STARTER_SPEED_Get();    
    SystemStatusRegister.starter_speed_mode_output = Starter_Data.speed_output;
    encodeStatusRegister(&SystemStatusRegister); 
}

bool GetFaultInput(void){
    Starter_Data.fault_input = uC_STARTER_FAULT_Get();
    SystemStatusRegister.starter_fault_input = Starter_Data.fault_input;
    encodeStatusRegister(&SystemStatusRegister); 
    return Starter_Data.fault_input;

}

bool GetSpeedOkInput(void){
    Starter_Data.speed_ok = uC_STARTER_SPEED_OK_Get();
    SystemStatusRegister.starter_speed_ok_input = Starter_Data.speed_ok;
    encodeStatusRegister(&SystemStatusRegister); 
    return Starter_Data.speed_ok;
}

void SetFaultStatus(void){    
    SetStatus(STARTER_FAULT_STATUS);    
    SetRunMode(false);
    SetFreeMode(true);
    
    // Set dei led LS,  HS
    SetLeds(LED_BLINK, LED_BLINK);    
    return;
}

/**
 * This is the Fault Reset Procedure.
 * 
 * The Fault Reset Procedure for the Sedecal Starter 
 * is composed by the following signal sequence:
 * + RUN = 0 and FREERUN = 0; DELAY 15ms
 * + RUN = 0 and FREERUN = 1; DELAY 15ms
 * 
 * The procedure attempts four times to reset the fault condition, 
 * with a 100ms interval between any attempts. 
 * 
 * The Procedure terminates with the Starter in STOP.
 * 
 */
void StarterHandleResetFault(void){
    
    // Nessun comando viene processato durante il blanking timer
    if(Starter_Data.command.blanking_timer){
        Starter_Data.command.blanking_timer--;
        return;
    }
    
    switch(Starter_Data.command.sequence){
        case 0:
            // Azzera RUN e FREERUN
            SetRunMode(false);
            SetFreeMode(false);
            
            Starter_Data.command.blanking_timer = 1; // Attende 15ms            
            Starter_Data.command.sequence++;            
            break;
            
        case 1:
            // Set FREERUN
            SetFreeMode(true);
            Starter_Data.command.blanking_timer = 2; // Attende 30ms            
            Starter_Data.command.sequence++;            
            break;
            
        case 2:
            if(GetFaultInput()){
                if(Starter_Data.command.attempt ){
                    // Ripete la sequenza dopo circa 100ms
                    Starter_Data.command.attempt--;
                    Starter_Data.command.sequence = 0;
                    Starter_Data.command.blanking_timer = 7;
                    break;
                }
            }
            
            // Fine sequenza. Il flag viene nuovamente attivato se lo starter è ancora in fault
            if(GetFaultInput()){
                SetFaultStatus();                
            }else{
                SetStatus(STARTER_STOP_STATUS);    
                // Set dei led LS,  HS
                SetLeds(LED_OFF, LED_OFF);    
            }
            
            Starter_Data.command.cmd = STARTER_NO_CMD;
            Starter_Data.command.executed = true;
            Starter_Data.command.blanking_timer = 0;
            Starter_Data.command.sequence = 0;
    
            SetRunMode(false);
            SetFreeMode(true);           
            break;
    }

}
void StarterHandleHsRun(void){
   // Nessun comando viene processato durante il blanking timer
    if(Starter_Data.command.blanking_timer){
        Starter_Data.command.blanking_timer--;
        return;
    }
    
    switch(Starter_Data.command.sequence){
        case 0:
            // Imposta il segnale di velocità al device e attiva la modalità di controllo della velocità
            SetSpeedMode(HS_SPEED_MODE);
            SetFreeMode(false);
            
            Starter_Data.command.blanking_timer = 1; // Attende 15ms            
            Starter_Data.command.sequence++;            
            break;
            
        case 1:
            // Attivazione della partenza
            SetRunMode(true);
            Starter_Data.command.command_timeout_tk = MS_TO_TICK(HS_COMMAND_TMO_ms); 
            Starter_Data.command.blanking_timer = MS_TO_TICK(500); // Attende almeno 500ms. Normalmente occorrono 3 secondi            
            Starter_Data.command.sequence++;            
            break;
            
        case 2:
            // Attende il segnale di rotazione OK o di fault
            if(GetFaultInput()){
                // Finisce qui in condizione di fault
                SetFaultStatus();
                Starter_Data.command.cmd = STARTER_NO_CMD;
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = 0;
                return;
            }
            
            // Il segnale di speed OK è arrivato dal device
            if(GetSpeedOkInput()){
                Starter_Data.command.cmd = STARTER_NO_CMD;
                SetStatus(STARTER_RUN_HS_STATUS);                
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = SEC_TO_TICK(StarterParamRegister.HS_Timeout_L + 256 * StarterParamRegister.HS_Timeout_H);
                // Set dei led LS,  HS
                SetLeds(LED_OFF, LED_ON);    
                return;
            }
            
            // Verifica timeout attivazione
            if(Starter_Data.command.command_timeout_tk) Starter_Data.command.command_timeout_tk--;
            else{
                // Finisce qui in condizione di stop
                SetStatus(STARTER_STOP_STATUS);
                Starter_Data.command.cmd = STARTER_NO_CMD;
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = 0;
                SetRunMode(false);
                SetFreeMode(true);
                
                // Set dei led LS,  HS
                SetLeds(LED_OFF, LED_OFF);    
                return;
            }
            
            break;
    }
}

void StarterHandleLsRun(void){
 // Nessun comando viene processato durante il blanking timer
    if(Starter_Data.command.blanking_timer){
        Starter_Data.command.blanking_timer--;
        return;
    }
    
    switch(Starter_Data.command.sequence){
        case 0:
            // Imposta il segnale di velocità al device e attiva la modalità di controllo della velocità
            SetSpeedMode(LS_SPEED_MODE);
            SetFreeMode(false);
            
            Starter_Data.command.blanking_timer = 1; // Attende 15ms            
            Starter_Data.command.sequence++;            
            break;
            
        case 1:
            // Attivazione della partenza
            SetRunMode(true);
            Starter_Data.command.command_timeout_tk = MS_TO_TICK(LS_COMMAND_TMO_ms); 
            Starter_Data.command.blanking_timer = MS_TO_TICK(500); // Attende almeno 500ms. Normalmente occorrono 3 secondi            
            Starter_Data.command.sequence++;            
            break;
            
        case 2:
            // Attende il segnale di rotazione OK o di fault
            if(GetFaultInput()){
               // Finisce qui in condizione di fault
                SetFaultStatus();
                Starter_Data.command.cmd = STARTER_NO_CMD;
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = 0;
                return;
            }
            
            // Il segnale di speed OK è arrivato dal device
            if(GetSpeedOkInput()){
                Starter_Data.command.cmd = STARTER_NO_CMD;
                SetStatus(STARTER_RUN_LS_STATUS);
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = SEC_TO_TICK(StarterParamRegister.LS_Timeout_L + 256 * StarterParamRegister.LS_Timeout_H);
                // Set dei led LS,  HS
                SetLeds(LED_ON, LED_OFF);    
                return;
            }
            
            // Verifica timeout attivazione
            if(Starter_Data.command.command_timeout_tk) Starter_Data.command.command_timeout_tk--;
            else{
                // Finisce qui in condizione di stop
                SetStatus(STARTER_STOP_STATUS);
                Starter_Data.command.cmd = STARTER_NO_CMD;
                Starter_Data.command.executed = true;
                Starter_Data.command.blanking_timer = 0;
                Starter_Data.command.sequence = 0;
                Starter_Data.command.activation_timeout_tk = 0;
                SetRunMode(false);
                SetFreeMode(true);
                // Set dei led LS,  HS
                SetLeds(LED_OFF, LED_OFF);    
                return;
            }
            
            break;
    }
}

void rtcStarter15msCallback(void){   
    
    // Gestione dei led
    if(Starter_Data.leds.timer_period){
       if(Starter_Data.leds.timer_blink){ 
           Starter_Data.leds.timer_blink--; 
           if(!Starter_Data.leds.timer_blink){
               Starter_Data.leds.timer_blink = Starter_Data.leds.timer_period;
               if(Starter_Data.leds.ls == LED_BLINK) LED_LS_Toggle();
               if(Starter_Data.leds.hs == LED_BLINK) LED_HS_Toggle();
           }
       }
    }       
    
    if(Starter_Data.command.cmd){
        switch(Starter_Data.command.cmd){            
            case STARTER_CMD_RESET_FAULT: StarterHandleResetFault(); return;
            case STARTER_CMD_RUN_HS: StarterHandleHsRun(); return;
            case STARTER_CMD_RUN_LS: StarterHandleLsRun(); return;
            case STARTER_NO_CMD: break;
        }        
    }
    if(Starter_Data.status == STARTER_COMMAND_EXECUTION_STATUS) return;
    
    // Quando il sistema è in fault solo un comando di reset fault può farlo uscire
    if(Starter_Data.status == STARTER_FAULT_STATUS) {
        
        return;
    }
    
    // Il Device ha attivato il Fault
    if(GetFaultInput()){
        SetFaultStatus();               
    }
           
    // Durante l'attivazione HS o LS viene gestito il timeout
    if( (Starter_Data.status == STARTER_RUN_HS_STATUS) || (Starter_Data.status == STARTER_RUN_LS_STATUS)){
        
        // Acquisizione del segnale speed ok
        GetSpeedOkInput();    
    
        if(Starter_Data.command.activation_timeout_tk){ 
            Starter_Data.command.activation_timeout_tk--;
            
            // Timeout event! Viene messo in STOP
            if(!Starter_Data.command.activation_timeout_tk){
                StarterStop();
            }
        }
    }
    return;
}


/**
 * \ingroup StarterImplementation
 * 
 * This function initializes the Parameters that are 
 * related with the Starter functions.
 * 
 * @remark the parameter initialization will have no effect 
 * whenever the EEPROM (where all the parameters reside) is stored for almost ones.
 * 
 * @warning never store parameters in the initialization function: this may 
 * prevent other modules to set their proper default parameters.
 */

void StarterParameterInit(void){
    REGISTER_STRUCT_t* canreg;
    
    // Inizializzazione parametri di default
    StarterParamRegister.HS_Timeout_L = (unsigned char) DEFAULT_HS_TIMEOUT_s;
    StarterParamRegister.HS_Timeout_H = (unsigned char) (DEFAULT_HS_TIMEOUT_s>>8);
    StarterParamRegister.LS_Timeout_L = (unsigned char) DEFAULT_LS_TIMEOUT_s;
    StarterParamRegister.LS_Timeout_H = (unsigned char) (DEFAULT_LS_TIMEOUT_s>>8);
    
    canreg = (REGISTER_STRUCT_t*) & StarterParamRegister;
    
    // Set del valore di default per questo registro. L'istruzione non ha effetto se la EEPROM è già stata
    // modificata. 
    MET_Can_Protocol_SetDefaultParameter(canreg->idx,canreg->d0, canreg->d1, canreg->d2, canreg->d3);
    
}

void SetLeds(_LED_STAT_t ls, _LED_STAT_t hs){
    if((Starter_Data.leds.ls == ls) && (Starter_Data.leds.hs == hs)) return;
    
    Starter_Data.leds.ls = ls;
    Starter_Data.leds.hs = hs;
    
    // Inizializza
    if(ls == LED_OFF) LED_LS_Clear();
    else LED_LS_Set();
    
    if(hs == LED_OFF) LED_HS_Clear();
    else LED_HS_Set();
    
    // Gestione del lampeggio sincronizzato
    if( (ls==LED_BLINK) || (hs==LED_BLINK) ){
        Starter_Data.leds.timer_period = MS_TO_TICK(300);
        Starter_Data.leds.timer_blink = Starter_Data.leds.timer_period; 
    }else Starter_Data.leds.timer_period = 0;
}

void StarterInit(void){
    
    SetRunMode(false);
    SetFreeMode(true);
    StarterSetFreeMode(true);
    SetSpeedMode(LS_SPEED_MODE);
    GetSpeedOkInput();
    
    // inizializzazione dei parametri di default
    StarterParameterInit();
    
     // INizializza stuttura led
    Starter_Data.leds.ls = LED_OFF;
    Starter_Data.leds.hs = LED_OFF;
    LED_LS_Clear();
    LED_HS_Clear();
    Starter_Data.leds.timer_period = 0;
    Starter_Data.leds.timer_blink = 0;
    
    if(GetFaultInput()) SetFaultStatus();
    else SetStatus(STARTER_STOP_STATUS);
    
    // Inizializzazione struttura dai per gestione comandi
    Starter_Data.command.blanking_timer = 0;
    Starter_Data.command.cmd = STARTER_NO_CMD;
    Starter_Data.command.executed = false;
    Starter_Data.command.sequence = 0;
    Starter_Data.command.activation_timeout_tk = 0;
  
    // Se viene rilevata una condizione di Fault all'accensione 
    // si attiva la procedura di reset, ma solo all'esaurimento del blanking timer
    if(Starter_Data.status == STARTER_FAULT_STATUS) StarterResetFault();
    
    StarterActivateHsRun();
    return;
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This command executes the Reset Fault procedure.
 * 
 * @remark The command cannot be executed if another command is executing (busy)
 * 
 * @return
 * + true: the procedure is started
 * + false: busy
 */ 
bool StarterResetFault(void){
    // Se non è in fault finisce subito
    if(Starter_Data.status != STARTER_FAULT_STATUS) return true;
    
    // Non può essere eseguito se un comando è in attesa di essere completato
    if(Starter_Data.status == STARTER_COMMAND_EXECUTION_STATUS) return false;
   
    Starter_Data.command.blanking_timer = 0;
    Starter_Data.command.sequence = 0;    
    Starter_Data.command.cmd = STARTER_CMD_RESET_FAULT;
    SetStatus(STARTER_COMMAND_EXECUTION_STATUS);
    
    Starter_Data.command.executed = false;
    Starter_Data.command.attempt = RESET_FAULT_ATTEMPTS;
    Starter_Data.command.activation_timeout_tk = 0;
    
    // Il flag di rotazone ok viene azzerato
    Starter_Data.speed_ok = false;
    
    return true;
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This command executes the Starter activation to High Speed.
 * 
 * @remark The command cannot be executed in Fault condition!
 * @remark The command cannot be executed if another command is executing (busy)
 * @remark Use the GetStarterStatus() to monitor the command completion;
 * @remark Use the StarterGetSpeedOk() to monitor the speed-ok condition;
 * 
 * 
 * @return
 * + 0: command successfully started;
 * + -1: command rejected because in fault condition;
 * + -2: command rejected because of B
 */ 
int StarterActivateHsRun(void){
    
      // Non può essere eseguito se il device si trova in fault
   if(Starter_Data.status == STARTER_FAULT_STATUS) return -1;
   if(GetFaultInput()) return -1;
    
    // Il comando è già in esecuzione
   if(Starter_Data.command.cmd == STARTER_CMD_RUN_HS) return 0;

   // Non può essere eseguito se un comando è in attesa di essere completato
   if(Starter_Data.status == STARTER_COMMAND_EXECUTION_STATUS) return -2;
   
    // Lo starter già è in modalità HS (o comando o mantenimento)
    if(Starter_Data.status == STARTER_RUN_HS_STATUS) return 0;
    if(Starter_Data.command.cmd == STARTER_CMD_RUN_HS) return 0;

    
    Starter_Data.command.blanking_timer = 0;
    Starter_Data.command.sequence = 0;    
    Starter_Data.command.cmd = STARTER_CMD_RUN_HS;
    SetStatus(STARTER_COMMAND_EXECUTION_STATUS);
    
    Starter_Data.command.executed = false;
    Starter_Data.command.attempt = RESET_FAULT_ATTEMPTS;
    
    // Il flag di rotazone ok viene azzerato
    Starter_Data.speed_ok = false;
    
    // Set dei led LS,  HS
    SetLeds(LED_OFF, LED_BLINK);        
    return 0;
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This command executes the Starter activation to Low Speed.
 * 
 * @remark The command cannot be executed in Fault condition!
 * @remark The command cannot be executed if another command is executing (busy) 
 * @remark The command cannot be executed if the High speed starter rotation is in action;
 * @remark Use the GetStarterStatus() to monitor the command completion;
 * @remark Use the StarterGetSpeedOk() to monitor the speed-ok condition;
 * 
 * 
 * @return
 * + 0: command successfully started;
 * + -1: command rejected because in fault condition;
 * + -2: command rejected because of Busy condition; 
 */
int StarterActivateLsRun(void){
    
   // Non può essere eseguito se il device si trova in fault
   if(Starter_Data.status == STARTER_FAULT_STATUS) return -1;
   if(GetFaultInput()) return -1;
    
   // Il comando è già in esecuzione
   if(Starter_Data.command.cmd == STARTER_CMD_RUN_LS) return 0;
   
   // Non può essere eseguito se un comando è in attesa di essere completato
   if(Starter_Data.status == STARTER_COMMAND_EXECUTION_STATUS) return -2;
   
    // Lo starter già è in modalità HS e il passaggio a velocità inferiore non è consentito
    if(Starter_Data.status == STARTER_RUN_HS_STATUS) return 0;
   
    // Lo starter già è in modalità LS (o comando o mantenimento)
    if(Starter_Data.status == STARTER_RUN_LS_STATUS) return 0;
    
    
           
    Starter_Data.command.blanking_timer = 0;
    Starter_Data.command.sequence = 0;    
    Starter_Data.command.cmd = STARTER_CMD_RUN_LS;
    SetStatus(STARTER_COMMAND_EXECUTION_STATUS);
    Starter_Data.command.executed = false;
    Starter_Data.command.attempt = RESET_FAULT_ATTEMPTS;
    
    // Il flag di rotazone ok viene azzerato
    Starter_Data.speed_ok = false;
    
    // Set dei led LS,  HS
    SetLeds(LED_BLINK, LED_OFF);    
    return 0;
    
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This command execute the Stop Starter execution.
 *  
 * @remark The command cannot be executed if another command is executing (busy)
 *  
 * @return 
 * + true: command success;
 * + false: a command is executing (but not completed)
 */
bool StarterStop(void){
   // Se si trova in fault l'anodo è gia fermo!
   // Non restituisce false perchè effettivamente il comando di stop chiederebbe di fermare l'anodo
   if(Starter_Data.status == STARTER_FAULT_STATUS) return true;
   
   // Non può essere eseguito se un comando è in attesa di essere completato
   if(Starter_Data.status == STARTER_COMMAND_EXECUTION_STATUS) return false;
    
    // Lo starter già è in modalità HS (o comando o mantenimento)
    if(Starter_Data.status == STARTER_STOP_STATUS) return true;
   
    SetStatus(STARTER_STOP_STATUS);
    SetRunMode(false);
    SetFreeMode(true);
    SetSpeedMode(LS_SPEED_MODE);
 
    // Set dei led 
    SetLeds(LED_OFF, LED_OFF);
    return true;
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This function executes the reset command timeout
 * (only for HS or LS active commands) 
 * 
 */
void StarterResetTimeout(void){
    if(Starter_Data.status == STARTER_RUN_LS_STATUS){
        Starter_Data.command.activation_timeout_tk = SEC_TO_TICK(StarterParamRegister.LS_Timeout_L + 256 * StarterParamRegister.LS_Timeout_H);
    }else if(Starter_Data.status == STARTER_RUN_HS_STATUS){
        Starter_Data.command.activation_timeout_tk = SEC_TO_TICK(StarterParamRegister.HS_Timeout_L + 256 * StarterParamRegister.HS_Timeout_H);
    }
    return;
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This function returns the status of the Speed Ok signal of the Starter device
 * @return 
 * + true: the speed ok is active;
 * + false: the speed ok is not active;
 */
bool StarterGetSpeedOk(void){
    return Starter_Data.speed_ok;
    
}

/**
 * \ingroup StarterInternalInterface
 * 
 * This function returns the Starter Module operating status
 * @return 
 * + the current operating status. see \ref _STARTER_STATUS_t
 */
_STARTER_STATUS_t GetStarterStatus(void){
    return Starter_Data.status;
}
    
/**
 * \ingroup StarterInternalInterface
 * 
 * @remark this command is ignored out of the HS or LS active rotation commands.
 * @remark this command is overridden by further activation commands
 * 
 * @param stat
 * + true: activates the free mode
 * + false: deactivate the free mode
 * 
 * @return
 * + true: command executed;
 * + false: command rejected;
 */
bool StarterSetFreeMode(bool stat){
     if((Starter_Data.status != STARTER_RUN_LS_STATUS)&&(Starter_Data.status != STARTER_RUN_HS_STATUS)) return false;
     
     SetFreeMode(stat);
     return true;
}
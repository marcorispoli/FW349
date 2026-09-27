#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"



static void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ); //!< This is the Command protocol callback
static volatile unsigned char current_command = 0;
/**
 * This function initializes the CAN Protocol module.
 * 
 * The function calls the library API  MET_Can_Protocol_Init() in order to:
 * + Set the Device Identifier;
 * + The number of implemented STATUS registers;
 * + The number of implemented DATA registers;
 * + The number of implemented PARAMETER registers;
 * + The Application revision 
 * + The protocol command handler;
 * 
 * The function initializes the Parameters with the default value   
 * with the library MET_Can_Protocol_SetDefaultParameter() function.
 * 
 */
void ApplicationProtocolInit ( void )
{
     
    // Initialize the Met Can Library
    MET_Can_Protocol_Init(MET_CAN_APP_DEVICE_ID, MET_CAN_STATUS_REGISTERS, MET_CAN_DATA_REGISTERS, MET_CAN_PARAM_REGISTERS, APPLICATION_MAJ_REV, APPLICATION_MIN_REV, APPLICATION_SUB_REV, ApplicationProtocolCommandHandler);
    
    
    
}
  
/**
 * This function shall be called by the MAIN loop application 
 * in order to manage the reception/transmission protocol activities.
 * 
 * The function merely call the library function  MET_Can_Protocol_Loop().
 */
void inline ApplicationProtocolLoop(void){
    
    // Handles the transmission/reception protocol
    MET_Can_Protocol_Loop();        
    
}

/**
 * This is the Command Handler implementation
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){
    
    
    switch(cmd){
        case MET_COMMAND_ABORT:  // This is the Library mandatory 
            MET_Can_Protocol_returnCommandAborted();
            break;
        
       
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}
        


void encodeStatusRegister(void* reg){
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 0, ((REGISTER_STRUCT_t*) reg)->d0 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 1, ((REGISTER_STRUCT_t*) reg)->d1 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 2, ((REGISTER_STRUCT_t*) reg)->d2 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 3, ((REGISTER_STRUCT_t*) reg)->d3 );
}

void decodeParamRegister(void* reg){
     int register_offset = 0;
     
    
   
    ((REGISTER_STRUCT_t*) reg)->d0 = MET_Can_Protocol_GetParameter(register_offset, 0);
    ((REGISTER_STRUCT_t*) reg)->d1 = MET_Can_Protocol_GetParameter(register_offset, 1);
    ((REGISTER_STRUCT_t*) reg)->d2 = MET_Can_Protocol_GetParameter(register_offset, 2);
    ((REGISTER_STRUCT_t*) reg)->d3 = MET_Can_Protocol_GetParameter(register_offset, 3);
    return;
   
    
}
#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"
#include "../masmeter/mas.h"



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
        
        case CMD_MAS_AUTO_CALIBRATION:
            
            if(MasAutoCalibration()) MET_Can_Protocol_returnCommandExecuted(0,0);
            else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
            break;
            
        case CMD_MAS_MANUAL_CALIBRATION:
            
            if(MasManualCalibration(d0)) MET_Can_Protocol_returnCommandExecuted(0,0);
            else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
            break;  
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}
        

/**
 * This function assign a status register data to the STATUS REGISTERS of the CAN BUS
 * 
 * #Usage
 * + Define a register structure initialized with the IDX of the STATUS register index;
 * + Set properly the content of the register.
 * + Call the function:  encodeStatusRegister(&TargetRegister). This call insert the Target register 
 * content into the CAN STATUS register at the proper IDX.
 * 
 * 
 * @param reg: this is the register structure to be filled with the STATUS register
 */
void encodeStatusRegister(void* reg){
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 0, ((REGISTER_STRUCT_t*) reg)->d0 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 1, ((REGISTER_STRUCT_t*) reg)->d1 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 2, ((REGISTER_STRUCT_t*) reg)->d2 );
    MET_Can_Protocol_SetStatusReg(((REGISTER_STRUCT_t*) reg)->idx, 3, ((REGISTER_STRUCT_t*) reg)->d3 );
}

/**
 * This function provides the CAN DATA Register content.
 * 
 * #Usage
 * + Define a register structure initialized with the IDX of the DATA register index;
 * + Call the function:  decodeDataRegister(&TargetRegister);
 * + Use the TargetRegister.fields as needed.
 * 
 * NOTE: the DATA register is a read only register which data content 
 * is set by the remote MASTER application.
 * 
 * @param reg: this is the register structure to be filled with the DATA register
 */
void decodeDataRegister(void* reg){
     int register_offset = ((REGISTER_STRUCT_t*) reg)->idx;
   
    ((REGISTER_STRUCT_t*) reg)->d0 = MET_Can_Protocol_GetData(register_offset, 0);
    ((REGISTER_STRUCT_t*) reg)->d1 = MET_Can_Protocol_GetData(register_offset, 1);
    ((REGISTER_STRUCT_t*) reg)->d2 = MET_Can_Protocol_GetData(register_offset, 2);
    ((REGISTER_STRUCT_t*) reg)->d3 = MET_Can_Protocol_GetData(register_offset, 3);
    return;
}

/**
 * This function provides the CAN PARAM Register content.
 * 
 * #Usage
 * + Define a register structure initialized with the IDX of the target PARAM register index;
 * + Call the function:  decodeParamRegister(&TargetRegister);
 * + Use the TargetRegister.fields as needed.
 * 
 * NOTE: the PARAM register is a read only register which data content 
 * is set by the remote MASTER application.
 * 
 * @param reg: this is the register structure to be filled with the PARAM register.
 */
void decodeParamRegister(void* reg){
    
    // Read the idx from the reg structure. It shall be initialized with the target IDX
    int register_offset = ((REGISTER_STRUCT_t*) reg)->idx;
     
    ((REGISTER_STRUCT_t*) reg)->d0 = MET_Can_Protocol_GetParameter(register_offset, 0);
    ((REGISTER_STRUCT_t*) reg)->d1 = MET_Can_Protocol_GetParameter(register_offset, 1);
    ((REGISTER_STRUCT_t*) reg)->d2 = MET_Can_Protocol_GetParameter(register_offset, 2);
    ((REGISTER_STRUCT_t*) reg)->d3 = MET_Can_Protocol_GetParameter(register_offset, 3);
    return;
   
}

/**
 * This function assign a param register content to the PARAM REGISTER Array of the CAN BUS
 * 
 * #Usage
 * + Define a register structure initialized with the IDX of the STATUS register index;
 * + Set properly the content of the register.
 * + Call the function:  encodeParamRegister(&TargetRegister). This call insert the Target register 
 * content into the CAN PARAM register at the proper IDX.
 * 
 * 
 * @param reg: this is the register structure to be filled with the PARAM register
 */
void encodeParamRegister(void* reg){
    MET_Can_Protocol_SetParameter(((REGISTER_STRUCT_t*) reg)->idx, 0, ((REGISTER_STRUCT_t*) reg)->d0 );
    MET_Can_Protocol_SetParameter(((REGISTER_STRUCT_t*) reg)->idx, 1, ((REGISTER_STRUCT_t*) reg)->d1 );
    MET_Can_Protocol_SetParameter(((REGISTER_STRUCT_t*) reg)->idx, 2, ((REGISTER_STRUCT_t*) reg)->d2 );
    MET_Can_Protocol_SetParameter(((REGISTER_STRUCT_t*) reg)->idx, 3, ((REGISTER_STRUCT_t*) reg)->d3 );
}
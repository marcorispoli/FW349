#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"
#include "../masmeter/mas.h"
#include "../starter/starter.h"


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
* \ingroup mAsmeterCanInterface
* 
* Command ID: CMD_MAS_AUTO_CALIBRATION (see \ref PROTOCOL_COMMANDS_t)
* 
* This command executes the MasAutoCalibration() routine,
* starting the Auto Calibration procedure. 
* See the Auto Calibration function description.
* 
* @param
* + no parameters needed         
*  
* @return 
* + IMMEDIATE EXECUTED(0,0) Auto Calibration Started 
* + IMMEDIATE ERROR(MET_CAN_COMMAND_NOT_ENABLED) if cannot be executed
* 
*/
void _CMD_MAS_AUTO_CALIBRATION(void){
    if(MasAutoCalibration()) MET_Can_Protocol_returnCommandExecuted(0,0);
    else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
    return;
}

/**
* \ingroup mAsmeterCanInterface
* 
* Command ID: CMD_MAS_MANUAL_CALIBRATION (see \ref PROTOCOL_COMMANDS_t)
* 
* This command executes the MasManualCalibration() routine,
* starting the Manual Calibration procedure. 
* See the Manual Calibration section for details. 
*          
* @param
* + d0: this is the anodic current in mA used for calibration.
* 
* @return 
* + SUCCESS: MET_Can_Protocol_returnCommandExecuted(0,0);
* + ERROR: MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
* 
* 
*/
void _CMD_MAS_MANUAL_CALIBRATION(uint8_t d0){
    
    if(MasManualCalibration(d0)) MET_Can_Protocol_returnCommandExecuted(0,0);
    else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
    return;
}

/**
* \ingroup mAsmeterCanInterface
* 
* Command ID: CMD_MAS_START_INTEGRATOR (see \ref PROTOCOL_COMMANDS_t)
* 
* This command executes the StartIntegrator() routine, starting external Integrator function.
*
* At the Integration completion, the resulting data will be available in the \ref MasIntegratorStatusRegister Status Register.
* 
* This command accepts two composed parameters:
* + dMas = d0 + 256 * d1. This is the mAs in 0.1 units (10 equals 1 mAs).
* + Integrator Timeout (ms) = d2 + 256 * d3;
*
* @param
* 
* + d0: dmas low byte;
* + d1: dmas high byte;
* + d2: timeout low byte;
* + d3: timeout high byte;
* 
* @return
* 
* + SUCCESS: MET_Can_Protocol_returnCommandExecuted(0,0);
* + ERROR: MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
*/
void _CMD_MAS_START_INTEGRATOR(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3){
    
   if(StartIntegrator(((unsigned short) d0 + 256* (unsigned short) d1), ((unsigned short) d2 + 256 * (unsigned short) d3), 0)) MET_Can_Protocol_returnCommandExecuted(0,0);
   else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
   return;
}
                
/**
* \ingroup StarterCanInterface
* 
* Command ID: CMD_STARTER (see \ref PROTOCOL_COMMANDS_t)
* 
* This command executes commands on the High Speed Starter device.
* The command accepts one parameter:
* + Command Code = d0;
* 
* Command Code Options (see \ref CMD_STARTER_CODES_t):
* + CMD_STARTER_STOP: STOP ROTATING. The High Speed Switch Off the anode rotation.
* + CMD_STARTER_HS_MODE: START HIGH SPEED MODE. The Starter executes an High Speed Activation.
* + CMD_STARTER_LS_MODE: START LOW SPEED MODE. The Starter executes a Low Speed Activation.
* + CMD_STARTER_RST_TMO: RESET TIMEOUT. The activation timeout is reset to 0
* + CMD_STARTER_RST_FAULT: RESET FAULT CONDITION. In case of an active fault this command try to reset it.
*
* @warning in case of an active fault condition, only the RESET FAULT command is processed.
* 
* @param d0: command code
* 
* @return
* 
* + SUCCESS: MET_Can_Protocol_returnCommandExecuted(0,0);
* + ERROR: MET_CAN_COMMAND_NOT_ENABLED in case of disabled condition (fault).
* + ERROR: MET_CAN_COMMAND_BUSY in case of command pending.
*/
void _CMD_STARTER(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3){
   int retcode = 0;
    
   switch(d0){
       case CMD_STARTER_STOP: // Stop rotation command
           if(StarterStop()) MET_Can_Protocol_returnCommandExecuted(0,0);
           else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
           break;
       case CMD_STARTER_HS_MODE:
           retcode = StarterActivateHsRun();
           if(retcode == 0)  MET_Can_Protocol_returnCommandExecuted(0,0);
           else if(retcode == -1)  MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
           else if(retcode == -2)  MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
           else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
           break;
        case CMD_STARTER_LS_MODE:
           retcode = StarterActivateLsRun();
           if(retcode == 0)  MET_Can_Protocol_returnCommandExecuted(0,0);
           else if(retcode == -1)  MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
           else if(retcode == -2)  MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
           else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_ENABLED);
           break;
        case CMD_STARTER_RST_TMO:
           StarterResetTimeout();
           MET_Can_Protocol_returnCommandExecuted(0,0);           
           break;
        case CMD_STARTER_RST_FAULT:
           if(StarterResetFault()) MET_Can_Protocol_returnCommandExecuted(0,0);
           else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);                   
           break;
   } 
 
   return;
}

/**
 * This is the Command Handler implementation
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){
    switch(cmd){
        case MET_COMMAND_ABORT: MET_Can_Protocol_returnCommandAborted(); break;
        case CMD_MAS_AUTO_CALIBRATION: _CMD_MAS_AUTO_CALIBRATION(); break;            
        case CMD_MAS_MANUAL_CALIBRATION:_CMD_MAS_MANUAL_CALIBRATION(d0); break;
        case CMD_MAS_START_INTEGRATOR: _CMD_MAS_START_INTEGRATOR(d0,d1,d2,d3); break;
        case CMD_STARTER:_CMD_STARTER(d0,d1,d2,d3); break;
        default:MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
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
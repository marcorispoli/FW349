#ifndef _PROTOCOL_H    
#define _PROTOCOL_H

#include "definitions.h"  
#include "application.h"  
#include "Shared/CAN/MET_can_protocol.h"

#undef ext
#undef ext_static

#ifdef _PROTOCOL_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif



typedef struct {
    unsigned char idx;
    unsigned char d0;
    unsigned char d1;
    unsigned char d2;
    unsigned char d3;
}REGISTER_STRUCT_t;
    

typedef enum{
    MET_CAN_APP_DEVICE_ID    =  0x19,      //!< Application DEVICE CAN Id address
    MET_CAN_STATUS_REGISTERS =  3,        //!< Defines the total number of implemented STATUS registers 
    MET_CAN_DATA_REGISTERS   =  1,        //!< Defines the total number of implemented Application DATA registers 
    MET_CAN_PARAM_REGISTERS  =  1       //!< Defines the total number of implemented PARAMETER registers 
}PROTOCOL_DEFINITION_DATA_t;


ext void ApplicationProtocolInit ( void);
ext void ApplicationProtocolLoop(void);
ext void encodeStatusRegister(void* reg);
ext void decodeParamRegister(void* reg);
ext void encodeParamRegister(void* reg);
ext void decodeDataRegister(void* reg);

//________________________________________ STATUS REGISTER DEFINITION SECTION _

/**
 * \ingroup StarterCanInterface
 * \ingroup mAsmeterCanRegisters
 */
typedef enum{
    SYSTEM_STATUS_IDX = 0, //!< This is the System general data status register 
    MAS_MONITOR_STATUS_IDX = 1, //!< This is the mAs test related status register
    MAS_INTEGRATOR_STATUS_IDX = 2, //!< This register provides the last integration result
}STATUS_INDEX_t;

    /**
     *  \ingroup mAsmeterCanInterface
     *  \ingroup StarterCanInterface
     * 
     *   STATUS REGISTER ADDRESS: 0
     */
    typedef struct {
        const unsigned char idx;                            //!< ID Address = 0   
        unsigned char mas_monitor_activated:1;              //!< This is the monitor activity feedback
        unsigned char mas_vref_activated:1;                 //!< This is the internal vref activation mode
        unsigned char mas_auto_calibration_activated:1;     //!< This is the auto calibration activation status flag
        unsigned char mas_manual_calibration_activated:1;   //!< This is the manual calibration activation status flag
        unsigned char do_spare:4;
        
        unsigned char starter_status:3;                      //!< This is the current status of the Starter module (see \ref _STARTER_STATUS)
        unsigned char starter_fault_input:1;                 //!< This is the status of the starter hardware fault input
        unsigned char starter_speed_ok_input:1;              //!< This is the status of the starter hardware speed ok input
        unsigned char starter_free_mode_output:1;            //!< This is the status of the starter hardware free mode output
        unsigned char starter_speed_mode_output:1;           //!< This is the status of the starter hardware speed mode output
        unsigned char starter_run_mode_output:1;             //!< This is the status of the starter hardware run mode output
        
        unsigned char d2; 
        unsigned char d3; 
    }SYSTEM_STATUS_t;
    
    /**
     *  \ingroup mAsmeterCanInterface
     * 
     *   STATUS REGISTER ADDRESS: 1
     * 
     *   This Status register provides the result of the Monitor activity.
     *   The Anodic current is measured in 1 second integration time
     *   using the calibration parameters.
     */
    typedef struct {
        const unsigned char idx;  //!< ID Address = 1 

        unsigned char mas_anodic_current_L; //!< This is the measured anodic current low byte
        unsigned char mas_anodic_current_H; //!< This is the measured anodic current high byte
        unsigned char mas_counts_L;         //!< This is the monitor counting low byte
        unsigned char mas_counts_H;         //!< This is the monitor counting high byte
        
    }MAS_MONITOR_STATUS_t;
    
     /**
     *  \ingroup mAsmeterCanInterface
     * 
     *   STATUS REGISTER ADDRESS: 2
     * 
     *   This Status register provides the result of the External Integrator activation command
     *   The Anodic current is measured in 1 second integration time
     *   using the calibration parameters.
     */
    typedef struct {
        const unsigned char idx; //!< ID Address = 2 

        unsigned char dmas_integrated_L;    //!< This is the total integrated mAs in 0.1 mAs units (low byte)
        unsigned char dmas_integrated_H;    //!< This is the total integrated mAs in 0.1 mAs units (high byte)
        unsigned char mas_time_L;           //!< This is the time measured for the integration in ms (low byte)
        unsigned char mas_time_H;           //!< This is the time measured for the integration in ms (HIGH byte)        
    }MAS_INTEGRATOR_STATUS_t;
    
    
    #ifdef _PROTOCOL_C       
        SYSTEM_STATUS_t SystemStatusRegister = {.idx=SYSTEM_STATUS_IDX};
        MAS_MONITOR_STATUS_t MasMonitorStatusRegister = {.idx=MAS_MONITOR_STATUS_IDX};
        MAS_INTEGRATOR_STATUS_t MasIntegratorStatusRegister = {.idx=MAS_INTEGRATOR_STATUS_IDX};
        
    #else
        extern SYSTEM_STATUS_t SystemStatusRegister;
        extern MAS_MONITOR_STATUS_t MasMonitorStatusRegister;
        extern MAS_INTEGRATOR_STATUS_t MasIntegratorStatusRegister;
    #endif  
        
        
//________________________________________ DATA REGISTER DEFINITION SECTION _   


typedef enum{
    SYSTEM_DATA_IDX = 0,
}DATA_INDEX_t;

/**
 *  \ingroup mAsmeterCanInterface
 * 
 *   DATA REGISTER ADDRESS 0
 */
typedef struct {
    const unsigned char idx; //!< ID Address = 0   
    
    unsigned char mas_internal_monitor:1; //!< mAmsmeter monitor activation bit
    unsigned char mas_monitor_with_vref:1;//!< mAmsmeter Vref activatioon bit
    unsigned char spare_d0:6;
    
    unsigned char d1;
    unsigned char d2;
    unsigned char d3;
}SYSTEM_DATA_t;


 #ifdef _PROTOCOL_C
    SYSTEM_DATA_t SystemDataRegister = {.idx=SYSTEM_DATA_IDX};
    
#else
    extern SYSTEM_DATA_t SystemDataRegister;
 
#endif  
    
//_________________PARAM REGISTER DEFINITION SECTION __________________________     
 
 
typedef enum{
    MAS_PARAM_IDX = 0, //!< mAsmeter calibration parameter index
    STARTER_PARAM_IDX = 1, //!< Starter parameter for timouts
}PARAM_INDEX_t;

/**
 *  \ingroup mAsmeterCanInterface
 *   PARAM REGISTER ADDRESS 0
 * 
 *   This is the parameter for the mas-meter calibration
 */
typedef struct {
    const unsigned char idx;    //!< ID Address = 0    
    unsigned char calibrated_L; //!< Calibration counted pulses (L byte)
    unsigned char calibrated_H; //!< Calibration counted pulses (H byte)
    unsigned char calibrated_ma;//!< Calibration current mA (max 255mA)     
    unsigned char calibrated_10ms_time;//!< Calibration integration time in 10ms units    
}MAS_PARAM_t;

/**
 *  \ingroup StarterCanInterface
 *   PARAM REGISTER ADDRESS 1
 * 
 *   This parameter regulates the timeouts for HS rotation and LS rotation
 */
typedef struct {
    const unsigned char idx;    //!< ID Address = 1    
    unsigned char HS_Timeout_L; //!< Timeout in seconds for the HS rotation (low byte)
    unsigned char HS_Timeout_H; //!< Timeout in seconds for the HS rotation (high byte)
    unsigned char LS_Timeout_L; //!< Timeout in seconds for the LS rotation (low byte)
    unsigned char LS_Timeout_H; //!< Timeout in seconds for the LS rotation (high byte)
}STARTER_PARAM_t;


 #ifdef _PROTOCOL_C

    MAS_PARAM_t MasParamRegister = {.idx=MAS_PARAM_IDX};
    STARTER_PARAM_t StarterParamRegister = {.idx=STARTER_PARAM_IDX};
#else
    extern MAS_PARAM_t MasParamRegister;
    extern STARTER_PARAM_t StarterParamRegister;
 
#endif  

        
//_______________________________________ PROTOCOL COMMANDS DEFINITION SECTION _ 
        
/**
 * \ingroup mAsmeterCanInterface
 * \ingroup StarterCanInterface
 */ 
typedef enum{
   CMD_ABORT = 0,               //!< Abort Command
   CMD_MAS_AUTO_CALIBRATION=1,    //!< Auto Calibration Activation Command
   CMD_MAS_MANUAL_CALIBRATION=2,  //!< Manual Calibration Activation Command
   CMD_MAS_START_INTEGRATOR=3,    //!< Execute an Integration session
   CMD_STARTER=4,                 //!< Starter Device activation commands
}PROTOCOL_COMMANDS_t;

/**
 * \ingroup StarterCanInterface
 */ 
typedef enum{
   CMD_STARTER_STOP = 0,  //!< STOP ROTATING. The High Speed Switch Off the anode rotation.
   CMD_STARTER_HS_MODE=1, //!< START HIGH SPEED MODE. The Starter executes an High Speed Activation.
   CMD_STARTER_LS_MODE=2, //!< START LOW SPEED MODE. The Starter executes an High Speed Activation.
   CMD_STARTER_RST_TMO=3, //!< RESET TIMEOUT. The activation timeout is reset to 0
   CMD_STARTER_RST_FAULT=4, //!< RESET FAULT CONDITION. In case of an active fault this command try to reset it.
}CMD_STARTER_CODES_t;


/// \ingroup CANPROT
/// This is the list of the application COMMAND Errors
typedef enum{
   FIRST_APPLICATION_ERROR = MET_CAN_COMMAND_APPLICATION_ERRORS,      //!< An error during the format collimation has been signaled  
}PROTOCOL_COMMANDS_ERRORS_t;

#endif 
    
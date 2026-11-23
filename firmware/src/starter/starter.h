#ifndef _STARTER_H    
#define _STARTER_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _STARTER_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup  Starter High Speed Starter - Requirement Description
 * \ingroup descriptionGroup
 */

/*!
 * \defgroup  StarterCanInterface High Speed Starter - Can Interface
 * \ingroup Starter
 */

/*!
 * \defgroup  StarterInternalInterface High Speed Starter - Internal Interface
 * \ingroup Starter
 */


/**
 * \addtogroup Starter
 * 
 * \section HIGH SPEED STARTER - IMPLEMENTATION
 * 
 * # Module Overview
 * 
 * This module implements all the commands and registers to control the High Speed starer device 
 * in compliance with the Starter Requirement Specifications.
 * 
 * The module implements:
 * 
 * + Internal and External commands to activate the Starter device in High and Low speed or Stop rotation.
 * + Diagnostic controls
 * + Timeouts for command execution and command activation.
 * 
 * The External commands are CAN BUS command useful for the Master Software to direct controlling
 * the Starter activation for test or operating purposes.
 * 
 * The Internal commands have been aimed to be used by internal modules like the exposure module
 * to implement the exposure preparation workflow.
 * 
 * Internal registers will be accessed with dedicated Functions to get the 
 * important activation status information. Similar information will be 
 * available in dedicated Can Bus registers accessible by the Can protocol.   
 * 
 * # Starter Workflow Management
 * 
 * The Starter module operates into a Callback called every 15.64 ms 
 * from the Firmware main routine. Consequently every action or change 
 * status is executed in a time slot of minimum 15.64 ms.
 * 
 * There are two main workflow execution modes: 
 * + the workflow during a command execution;
 * + the workflow during the idle;
 * 
 * In the workflow dedicated to the command execution, 
 * a status machine will step the command execution from its starting point to 
 * the final status. During this sequence no other commands can be activated.
 * 
 * In the workflow dedicated to the Idle time, the diagnostic checks and 
 * the timeout activation check will be performed.   
 * 
 * See the \ref rtcStarter15msCallback() for implementation details.
 *  
 * # Starter Operating Status
 * 
 * The current Starter operating status is described by its internal status 
 * code (see \ref _STARTER_STATUS_t):
 * 
 * + STARTER_COMMAND_EXECUTION_STATUS: a command is executing;
 * + STARTER_STOP_STATUS: the device is in STOP mode;
 * + STARTER_RUN_HS_STATUS: the Device is running in High Speed Mode;
 * + STARTER_RUN_LS_STATUS: the Device is running in Low Speed Mode;
 * + STARTER_FAULT_STATUS: the Device is in Fault mode;
 * 
 * The current status can be accessed:
 * + The internal modules with the GetStarterStatus() function;
 * + The External Master Software with the Can Bus Status register  \ref SYSTEM_STATUS_t;
 * 
 * # Starter Hardware Lines Status
 * 
 * The Starter Module makes use of GPIO lines to control the Starter External device
 * following the activation protocol of that device. 
 * 
 * The internal procedures handling the device activation and monitoring set properly those 
 * lines. 
 * 
 * The actual status of those lines are copied into the Can Bus Status register  \ref SYSTEM_STATUS_t
 * and hence can be accessed by the Master Software for test and monitoring purpose.
 * 
 * # External Starter Control Procedures
 * 
 * The Starter module provides the following Can Interface command to control the Starer device:
 * + _CMD_STARTER()
 * 
 * The parameters passed to that command defines the execution (see the CAN Command section for details).
 * 
 * The possible commands are:
 * + Starts the Rotation to High Speed;
 * + Starts the Rotation to Low Speed; 
 * + Restarts the command timeout;
 * + Stops Rotation;
 * + Fault Reset;
 *  
 * # Internal Starter Control Procedures
 * 
 * The module provides internal Functions to be called by internal modules 
 * to activate the Starter commands:
 * +  StarterActivateHsRun(): Starts the Rotation to High Speed;
 * +  StarterActivateLsRun(): Starts the Rotation to Low Speed;
 * +  StarterResetTimeout(): Restarts the command timeout;
 * +  StarterStop(): Stops Rotation;
 * +  StarterResetFault(): Fault Reset procedure activation;
 * 
 * # Activation to Low Speed description
 * 
 * The Starter can be activated in Low speed rotation by either internal or external commands
 * as described in the previous sections.
 * 
 * There are some limitations to execute this command:
 * + the Starter shall not be in fault condition;
 * + there isn't an executing command (busy);
 * + the Starter is running in high speed mode.
 * 
 * @remark to prevent stress to the Tube bearings, the starter doesn't change 
 * its actual high speed to pass to low speed. Actually to pass to low speed the Stop
 * command should be executed first.
 * 
 * The procedure set the GPIO's as expected then waits 
 * the signal SPEED-OK activation should come from the Starter device.
 * 
 * If the signal shouldn't arrive in  2.5 seconds the command 
 * terminates in STOP condition.
 * 
 * @remark although a fault condition is not directly set by the Starter module 
 * due to a timeout condition, the calling  module (internal or external) should 
 * stop the executing workflow with a proper error condition.
 * 
 * If finally the command successfully completes, the activation timer 
 * is initialized with the timeout parameter \ref STARTER_PARAM_t LS_Timeout.
 * 
 * If no Reset Timeout command should be further received, the Starter will Stop 
 * when the timeout expires. 
 * 
 * See the \ref STARTER_PARAM_t for details.
 * 
 * # Activation to High Speed description
 * 
 * The Starter can be activated in High speed rotation by either internal or external commands
 * as described in the previous sections.
 * 
 * There are some limitations to execute this command:
 * + the Starter shall not be in fault condition;
 * + there isn't an executing command (busy);
 *  
 * The procedure sets the GPIO's as expected then waits 
 * the signal SPEED-OK activation should come from the Starter device.
 * 
 * If the signal shouldn't arrive in 5 seconds the command 
 * terminates in STOP condition.
 * 
 * @remark although a fault condition is not directly set by the Starter module 
 * due to a timeout condition, the calling  module (internal or external) should 
 * stop the executing workflow with a proper error condition.
 * 
 * If finally the command should successfully complete, the activation timer 
 * is initialized with the timeout parameter \ref STARTER_PARAM_t HS_Timeout.
 * 
 * If no Reset Timeout command should be further received, the Starter will Stop 
 * when the timeout expires. 
 * 
 * See the \ref STARTER_PARAM_t for details.
 * 
 * # Activation Timer
 * 
 * Only for high speed and low speed activation commands, 
 * when the command should successfully complete and the Starter device 
 * should have activated the SPEED-OK signal input, an Activation timer 
 * is inizialized with a proper expiring time value in seconds.
 * 
 * + The Timeout value for High Speed activation timer is: \ref STARTER_PARAM_t HS_Timeout
 * + The Timeout value for Low Speed activation timer is: \ref STARTER_PARAM_t LS_Timeout
 * 
 * Those parameters are initialized after board programming with a default value of:
 * + 300 seconds for high speed rotation;
 * + 120 seconds for low speed rotation;
 * 
 * @note the timeout for the high speed is specifically higher then the low speed one 
 * due to the high level of stress caused by the high to low speed transition.
 *   
 * When the timeout should expires the Anode Rotation is stopped.
 * 
 * ## Activation Timer Internal reset command
 * 
 * The internal modules can reset the timer calling the StarterResetTimeout() function.
 * 
 * ## Activation Timer External reset command
 * 
 * The Master Software may request to reset the timer 
 * calling the Can Interface command:
 * + _CMD_STARTER(CMD_STARTER_RST_TMO);
 * 
 * See the _CMD_STARTER() for details.
 * 
 * # Fault Handling
 * 
 * The Starter Device hardware monitor the rotation activities 
 * and activate the Fault GPIO line in case of malfunction.
 * 
 * The Starter Module as soon as the fault line should be detected active 
 * enters the FAULT status condition.
 * 
 * In Fault Status condition:
 * + every current rotation execution is aborted;
 * + every rotation start command is rejected;
 * 
 * The Master Software (and also the internal modules) may monitor the current 
 * Starter Fault condition:
 * 
 * + The Internal module may call the GetStarterStatus() function;
 * + The Master may read the Status register \ref SYSTEM_STATUS_t
 * 
 * The Master Software may request to reset the fault condition 
 * activating the RESET FAULT procedure with the Can Interface Command: 
 * + _CMD_STARTER(CMD_STARTER_RST_FAULT);
 * 
 * @remark The Internal module should not reset the current fault condition. 
 * The fault condition should require the Master software evaluation criteria.  
 * 
 * # Speed Monitoring 
 * 
 * The internal modules can monitor the status of the anode rotation 
 * calling the function  StarterGetSpeedOk().
 * This function returns the status of the GPIO line SPEED OK but only during
 * tje active rotation command.
 * 
 * @remark this signal is not modified by the Starter device 
 * if the Free Mode should be active.
 * 
 * # Free Run Activation Mode
 * 
 * The Starter device makes an active control on the current rotation rate 
 * in order to keep the rotation at the expected speed. As soon as the rotation rate drops 
 * below a lower threshold the SPEED-OK signal is cleared and a power burst is 
 * applied to the Tube Anode Rotor to accelerate it to the target speed. 
 * 
 * This power burst may cause unpredictable noise if it should happen during 
 * an exposure. For this reason the FREE mode can be momentary activated 
 * by internal modules to prevent the Device to apply power to the Tube.
 * 
 * @warning The Free mode should be activated only for a brief time (during the exposure) 
 * provided that the rotation should not drop under a danger low speed in this time.
 * The Free mode should immediately deactivated after the exposure time.
 * 
 * The Internal modules (and only the internal modules) may activate the Free mode
 * calling the function:
 * + StarterSetFreeMode(); 
 * 
 * @remark this command is ignored out of the HS or LS active commands.
 * 
 * # Panel LED Management
 * 
 * The Starter module controls two Panel Led to let the 
 * activity inspection. 
 * 
 * The Led are labeled on the board as S/LS (Low Speed Led)  and S/HT (High Speed Led)
 * 
 * The module will implement the following light codes:
 * |LED S/LS|LED S/HS|Description|
 * |:--:|:--:|:--:|
 * |OFF|OFF|Starter in Stop Mode|
 * |Blink|OFF|Starter is executing the Low Speed activation|
 * |ON|OFF|Starter is in Low Speed status|
 * |OFF|Blink|Starter is executing the High Speed Activation|
 * |OFF|ON|Starter is in High Speed status|
 * |Blink|Blink|Starter is in Fault condition|
 * ||||
 * 
 * 
 * 
 * 
 * 
 * 
 */


/**
 * \ingroup StarterInternalInterface
 * 
 * This is the enumeration of the possible Module Operating Status 
 */
typedef enum {
    STARTER_COMMAND_EXECUTION_STATUS = 0, //!< A command is executing (not yet completed))
    STARTER_STOP_STATUS = 1,    //!< A Stop command is successfully completed
    STARTER_RUN_HS_STATUS = 2,  //!< A High Speed rotation command is successfully completed
    STARTER_RUN_LS_STATUS = 3,  //!< A Low Speed rotation command is successfully completed
    STARTER_FAULT_STATUS = 4,   //!< The Module is in Fault condition.
            
}_STARTER_STATUS_t;

typedef enum {
    STARTER_NO_CMD = 0,
    STARTER_CMD_RESET_FAULT = 1,
    STARTER_CMD_RUN_HS = 2,
    STARTER_CMD_RUN_LS = 3,
}_STARTER_COMMANDS_t;


typedef struct cmdstruct{
    int blanking_timer;
    int attempt; 
    unsigned char sequence;
    _STARTER_COMMANDS_t cmd;
    bool executed;
    unsigned short command_timeout_tk; //!< Timer for the command execution timeout in tick units (when required))
    unsigned short activation_timeout_tk; //!< The activation timer in ticks (for run HS or LS only))
}_COMMAND_DATA_t;


typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_BLINK = 2    
}_LED_STAT_t;


typedef struct ledstruct{
    _LED_STAT_t ls;
    _LED_STAT_t hs;
    int timer_blink;
    int timer_period;
}_LED_DATA_t;
    
typedef struct starter{
    _STARTER_STATUS_t status; //!< This is the current Starter module status
    _COMMAND_DATA_t command; //!< Command execution data structure
    _LED_DATA_t leds; //!< Panel led management 
    
    bool speed_ok;  //!< The Starter SPEED OK is set
    bool free_output; //!< The Starter FREE output is set
    bool run_output;  //!< The Starter RUN output is set
    bool fault_input;  //!< The Starter FAULT input is set
    bool speed_output; 
    
}_STARTER_Struct_t;

ext _STARTER_Struct_t Starter_Data;



ext void StarterInit (void);
ext void rtcStarter15msCallback(void);


ext int StarterActivateHsRun(void);
ext int StarterActivateLsRun(void);
ext bool StarterStop(void);
ext void StarterResetTimeout(void);
ext bool StarterResetFault(void);
ext _STARTER_STATUS_t GetStarterStatus(void);
ext bool StarterGetSpeedOk(void);
ext bool StarterSetFreeMode(bool stat);

// End Module Definition
#endif 
    
#ifndef _MAS_H    
#define _MAS_H

/**
 * @file mas.h
 * @brief Gestione del modulo mAsmeter.
 */

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _MAS_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup  mAsmeterImplementation mAsmeter Implementation Details
 * \ingroup implementationGroup
 */

/*!
 * \defgroup  mAsmeter Mas-meter - Requirement Description
 * \ingroup descriptionGroup
 */

/*!
 * \defgroup  mAsmeterCanInterface mAsmeter Can Interface
 * \ingroup mAsmeter
 */

/*!
 * \defgroup  mAsmeterInternalInterface mAsmeter - Internal Interface
 * \ingroup mAsmeter
 */

/**
 * \addtogroup mAsmeter
 * 
 * \section mAsmeter mAs-meter implementation description
 * 
 * # Monitoring
 * 
 * ## Overview
 * The monitor function controls the performance of the mas-meter during the Idle time.
 * 
 * The monitor integrates for 1 second the input pulses and calculates the equivalent anodic current,
 * based on the anodic calibration parameters.
 * 
 * The calculated anodic current and the total integrated pulses are provided 
 * by the CAN register to the Communication external master.
 * 
 * The input can be sourced internally by a voltage reference or applying an external voltage (or current) 
 * equivalent to a given anodic current.
 * 
 * ## Monitor activation and options
 * 
 * The Monitor may be activated by  a dedicated bit in the \ref SYSTEM_DATA_t 
 * 
 * When activated, 32 bit counter TC2 starts integrating the input pulses.
 * When the 32 bit timer TC0 set to 1 second period expires, the total number of the 
 * counted pulses is evaluated and a virtual anodic current is calculated 
 * with the following formula:
 * + Ianode = (Pulses * Ic/Pc) * (Tc/T)
 * + Ic: is the calibration current used to calibrate the masmeter;
 * + Pc: is the number of pulses integrated during the calibration process;
 * + T: is the monitoring integration period (1 second);
 * + Tc: is the integration period used during the calibration process
 * 
 * When the Monitor is activated (bit in the DATA register), optionally, 
 * an internal VREF can be either activated, simulating a 204mA of anodic current.
 * + See the Can Data Register Address: \ref SYSTEM_DATA_t for detail.
 *  
 * If the internal Vref should be activated, the external anodic input voltage 
 * signal will be overridden and will have no effect on the counting.
 * 
 * @note The internal Vref is a precise 2.04 volt, equivalent to 204mA on the external 10 ohm 
 * anodic sensing resistor.
 * 
 * @attention The Monitoring is momentary disabled if the Integrator should be activated.
 * @attention The internal Vref is disabled if the Monitor should be disabled
 * 
 * ## Monitor Current Status
 * 
 * Every 15ms the CAN Status registers are updated with the current Monitor status.
 * 
 * The Monitor Activation status is provided:
 * + The current monitor activation status;
 * + The internal Vref activation status;
 * + See the \ref SYSTEM_STATUS_t
 * 
 * ## Monitor Results
 * 
 * Every 15ms the CAN Status registers are updated with the current resulting data.
 * 
 * The Resulting data of the monitoring are provided:
 * + The calculated anodic current (virtual if vref is on or real if the external current should be used);
 * + The total integrated pulses.
 * + See the \ref MAS_MONITOR_STATUS_t
 * 
 * ## How to use the monitoring
 * 
 * The Monitor can be used either during the board test or 
 * during the normal unit activity in a IDLE time to detect
 * the accuracy. 
 * 
 * The external Master software may activate the monitoring with the internal vref 
 * and compare the calculated anodic current with the equivalent 
 * anodic current (204mA). An error in % may be calculated and,
 * in the case of a not acceptable error, may require a new calibration 
 * or a service investigation.
 * 
 * @warning the percentage error of calculated anodic current corresponds to the 
 * percentage error of the integrator during the exposure, and consequently in the 
 * percentage error in the released dose to the patient.
 * 
 * # Integrator 
 * 
 * ## Overview
 * 
 * The Integrator function allows to integrate a predefined number of pulses 
 * with a predefined timeout. this feature is used in the exposure sequences 
 * to provide a predefined dose during the exposure.
 * 
 * The integrator function and its implementing module is though to be internally used 
 * by the exposure control modules. Consequently there is not a data interface
 * exposed to the CAN bus registers when internally activated. 
 * 
 * However, for test purpose, when used with the External activation for test,
 * the result can be accessed by the CAN bus registers.
 * 
 * @remark  the Integrator results during the internal activation usage (exposure sequences) 
 * will be available by the internal modules and provided to the Master Software
 * with proper dedicated data structures.
 * 
 * @warning when the Integrator should be activated (either internally or externally) the 
 * Monitor activity and the Vref activation are momentary disabled. They automatically 
 * restart (if requested) as soon as the Integrator completes its activation.
 * 
 * ## Integrator operations
 * 
 * When the Integrator is activated (Started):
 * + The 32 bit Timer TC0 is set with the period corresponding to the Integrator requested timout;
 * + The 32 bit Counter is activated with a Overflow set to the total pulse to be counted;
 * 
 * The Integrator may terminate in two possible modes:
 * + TC0 timeout event: the timeout error condition is set in the Intergator data structure;
 * + TC2 counter overflow: the integration success flag is set in the Ingrator data structure;
 * 
 * In either cases, the following actions will be performed:
 * + Integration time calculation: the time is stored into the Integrator data structure;
 * + Integrated mAs calculation: the calculated mAs is stored into the Integrator data structure;
 *
 * The Integrator Callback() function is then called for the consumer module:
 * + in the internal activation, the calling module will implement all the actions related to the mAsmeter (exposure termination);
 * + in the External Activation (CAN BUS command for test) the results are stored into the Can Status registers. 
 *  
 * 
 * ## Integrator Internal Activation
 * 
 * The Integrator may be internally called by other modules with the following software Call:
 * + StartIntegrator() 
 * 
 * The calling module (such the exposure handing module) passes a callback for the 
 * quick Integration termination operations. The callback will be called passing as parameters the result of the 
 * Integrator process. See the \ref MAS_COMPLETE_CALLBACK for details
 * 
 * ## Integrator External Activation (for test purposes)
 * 
 * The Firmware implements a dedicated CAN Command:
 * + CMD_MAS_START_INTEGRATOR 
 * 
 * See the CAN Interface comamand dscription for details
 * 
 * ## Integrator result and actions
 * 
 * When the Integrator completes either in timeout or for the counting,
 * the result is calculated and stored into the Integrator data structure.
 * 
 * The Integration time is calculated in the case the counting successfully completes
 * getting the TC0 time register and converting it in ms.
 * 
 * The partial Integrated mAs is calculated in the case of Timeout event with the following 
 * formula:
 * 
 * + mAS = Tc * Ic * P * 1000 / Pc where:
 * + Pc: is the calibrated pulses;
 * + Ic: is the calibration current (mA);
 * + Tc: is the calibration time (ms);
 * + P: is the counted pulses;
 * 
 * If the activation should External, an internal callback is called at the 
 * Integration complete event. In that case the resulting integration (time and mAs) 
 * are copied into the Can Status register \ref MAS_INTEGRATOR_STATUS_t and the 
 * Master Software can read it for test and calibration purposes.
 * 
 * In case the activation should be made with internal modules (like the exposure module) 
 * the module will call callback passing the mAs, time and result as function parameter.
 * 
 * # Calibration 
 * 
 * The mAsmeter needs to be calibrated due to the tollerances of the hardware components.
 * 
 * There are two main modes to calibrate the mAsmeter:
 * + Auto Calibration: the masmeter is calibrated with an internal voltage reference
 * that simulate the voltage developed by an external anodic current of 204mA;
 * 
 * + Manual calibration: the masmeter is alibrated with a known anodic current 
 * (or equivalent voltage) on the mAsmeter input.
 * 
 * In either modes the calibration procedure will store the following values in the 
 * \ref MAS_PARAM_t.
 * 
 * + The Integration time Tc ;
 * + The Calibration anodic current Ic;
 * + The Calibration Pulses Pc;
 * 
 * Those parameters will be further used to calculate the real anodic current in the monitor
 * and the pulses for the Integrator (see related sections) 
 * 
 * ## Auto calibration Description
 * 
 * The Auto calibration is activated with a Can Command: CMD_MAS_AUTO_CALIBRATION
 * 
 * When activated, the internal Voltage reference of 2.04V is applied to the 
 * VFO circuit that start generating pulses. The TC0 timer is set to expire after one second, 
 * and the counter TC2 starts counting the pulses.
 * 
 * After one second the counting stops and the calibration parameters Tc=1s, Ic=204mA, Pc = TC2 counted pulses
 * will be stored.
 * 
 * ## Manual calibration Description
 * 
 * The Manual calibration procedure requires to apply a known voltage 
 * to the masmeter input. In the manaual calibration process it is possible 
 * to select an arbitrary integration time and an arbitrary anodic current.
 * 
 * The Manual calibration is activated with a Can Command: CMD_MAS_MANUAL_CALIBRATION
 * passing the integration time and the anodic current as parameter.
 * 
 * The TC0 timer will be set to expire after the desired integration time. 
 * At the expiring time, as for auto calibration, the parameters Tc, Ic, Pc will be stored.
 * 
 * 
 */

/**
 *  \ingroup mAsmeterInternalInterface
 *  This is the Callback prototype for the Integrator completion event.
 *  @param 
 *  + tmo: true if the timeout event happen;
 *  + dmas: integrated mAs in 0.1 mas units;
 *  + time_ms: the integration time in ms units  
 */ 

typedef void (*MAS_COMPLETE_CALLBACK) (bool tmo, unsigned short dmas, unsigned short time_ms); 


/**
 * \ingroup mAsmeterImplementation
 * This is the mAsmeter Data Structure
 */
typedef enum{
    MAS_AUTO_CALIBRATION = 0, //!< Auto Calibration Constant Definition
    MAS_MANUAL_CALIBRATION = 1, //!< Manual Calibration Constant Definition
}_MAS_CALIB_MODE;

/**
 * \ingroup mAsmeterImplementation
 */
typedef struct masmeter{
    
    // Manual Calibration Process Management
    struct calibration_struct{
        _MAS_CALIB_MODE calibration_mode;   //!< Defines the calibrtion mode
        bool status;                        //!< Calibration Process Running
        bool completed;                     //!< Calibration Process Terminated
        unsigned char anodic_current_test;  //!< Anodic current used for calibration
        unsigned long cumulated_pulses;     //!< Cumulated pulsig during the integration
        unsigned char iterations;           //!< Number of current executing iteration
        unsigned char num_iterations;       //!< Number of total iteration to complete the calibration
        unsigned short integration_time_ms; //!< The integration time in ms units
    }calibration;
    
    // Manual Calibration Process Management
    struct monitor_struct{
        bool status;            //!< The internal monitoring is activated
        unsigned long pulses; //!< Cumulated pulsig during the integration
        unsigned short  anodic_current; //!< Estimated anodi current in the integration slot
        unsigned short integration_time_ms; //!< The integration time in ms units
        bool vref_stat;         //!< The internal voltage reference activation status (2.04V)
        bool integration_status;//!< The integration is running
    }monitor;
    
    // masmeter for the exposure
    struct integration_struct{
        bool status;            //!< The integration is running
        bool completed;         //!< The integration is running
        bool error_timeout;     //!< The integration is early terminated due to a timeout
        float request_mAs;   //!< This is the total mAs the integrator should integrate
        float integrated_mAs;   //!< This is the total integrated mAs
        unsigned short integrated_time_ms; //!< The integration time for the integrated mAs        
        unsigned short timeout_ms;      //!< The Timeout for the counting        
        MAS_COMPLETE_CALLBACK Callback;//!< CAllback called when the integrator ends 
    }integrator;
    
}_MAS_Struct;

ext _MAS_Struct MAS_Data;


ext void MasInit (void);
ext void rtcMas15msCallback(void);
ext bool MasAutoCalibration(void);
ext bool MasManualCalibration(unsigned char anodic_current);
ext bool StartIntegrator(unsigned short dmAs, unsigned short timeout_ms, MAS_COMPLETE_CALLBACK callback);
ext void StopIntegrator(bool timeout);

#endif 
    
#ifndef _MAS_H    
#define _MAS_H

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
 * \defgroup MASMETER_MODULE HS mAsMeter Handling Module
 * \ingroup applicationModule
 * 
 * TBD
 * 
 * ## Dependencies
 * 
 * TBD
 * 
 * 
 * ## Module Description
 * 
 * TBD
 *  
 */


/**
 * \addtogroup MASMETER_MODULE
 * 
 * ## Application API
 * TBD
 * 
 */

typedef enum{
    MAS_AUTO_CALIBRATION = 0,
    MAS_MANUAL_CALIBRATION,
}_MAS_CALIB_MODE;

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
        unsigned char integration_time_10ms; //!< The integration time in 10ms units
    }calibration;
    
    // Manual Calibration Process Management
    struct monitor_struct{
        bool status;            //!< The internal monitoring is activated
        unsigned long pulses; //!< Cumulated pulsig during the integration
        unsigned short  anodic_current; //!< Estimated anodi current in the integration slot
        unsigned char integration_time_10ms; //!< The integration time in 10ms units
        bool vref_stat;         //!< The internal voltage reference activation status (2.04V)
        bool integration_status;//!< The integration is running
    }monitor;
    
        
}_MAS_Struct;

ext _MAS_Struct MAS_Data;

/**
 * Module Initialization
 */
ext void MasInit (void);
ext void rtcMas15msCallback(void);

ext bool MasAutoCalibration(void);
ext bool MasManualCalibration(unsigned char anodic_current);


// End Module Definition
#endif 
    
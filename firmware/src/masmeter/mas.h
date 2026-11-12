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

typedef void (*MAS_COMPLETE_CALLBACK) (void);

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

/**
 * Module Initialization
 */
ext void MasInit (void);
ext void rtcMas15msCallback(void);

ext bool MasAutoCalibration(void);
ext bool MasManualCalibration(unsigned char anodic_current);

ext bool StartIntegrator(unsigned short dmAs, unsigned short timeout_ms, MAS_COMPLETE_CALLBACK callback);
ext void StopIntegrator(bool timeout);

// End Module Definition
#endif 
    
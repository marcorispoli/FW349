#ifndef _EXPOSURE_H    
#define _EXPOSURE_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _EXPOSURE_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup EXPOSURE_MODULE HS Exposure Handling Module
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
 * \addtogroup EXPOSURE_MODULE
 * 
 * ## Application API
 * TBD
 * 
 */


typedef enum {
    EXPOSURE_NONE = 0,
    EXPOSURE_2D_MANUAL,
    EXPOSURE_2D_AEC,        
    EXPOSURE_3D_MANUAL,
    EXPOSURE_3D_AEC,
}_EXPOSURE_MODE;

typedef enum {
    EXPOSURE_WITH_GRID = 0x1,
    EXPOSURE_WITH_MASMETER = 0x2,
    EXPOSURE_WITH_DETECTOR = 0x4,
    EXPOSURE_WITH_HS_STARTER = 0x8,        
}_EXPOSURE_OPTIONS;

typedef enum {
    DIAG_ANODIC     = 0x1,
    DIAG_VBUS       = 0x2,
    DIAG_VSENS      = 0x4,
    DIAG_TMO        = 0x8,
    DIAG_STARTER_OK = 0x10,                    
}_EXPOSURE_DIAG_OPTIONS;

typedef enum {
    EXPSTAT_NONE = 0,
    EXPSTAT_INIZIALIZATION,
    EXPSTAT_XRAY_ON,
    EXPSTAT_WAIT_DATA,
    EXPSTAT_COMPLETED,           
}_EXPOSURE_STAT_FLAG;

typedef struct exposure_status{    
    _EXPOSURE_STAT_FLAG status_flag;
    int sequence;
}_EXPOSURE_STAT;


typedef enum {
    EXPERR_NONE = 0,    
    EXPERR_BUTTON_RELEASE,    
}_EXPOSURE_ERROR;


typedef struct exposure_data{    
        int KV_pre;    //!< KV (100V unit) to be exposed for the pre
        int KV_pulse;  //!< KV (100V unit) to be exposed for the pulse
        int mAs_pre;   //!< Total mAs for the pre-pulse
        int mAs_pulse; //!< Total mAs for the pulse        
        int tmo_pre;   //!< timeout in milliseconds for the pre pulse
        int tmo_pulse; //!< timeout in milliseconds for the pulse

        // Data reserved for the tomo exposure
        int tomo_skips;     //!< tomo samples to be skip for the Tube aligning
        int tomo_pulses;    //!< tomo samples to be exposed

}_EXPOSURE_DATA;
    
typedef struct exposure_result{    
    _EXPOSURE_ERROR error_code;
    int mAs_pre;   //!< Total mAs exposed in the pre-pulse
    int mAs_pulse; //!< Total mAs exposed in the pulse    
}_EXPOSURE_RESULT;

typedef struct exposure_io{    
    bool xray_request_button;
}_EXPOSURE_IO;

typedef struct exposure{
    
    _EXPOSURE_MODE          mode; //!< Defines what kind of exposure has been selected
    _EXPOSURE_OPTIONS       options; //!< Defines what options are selected for the incoming expsure
    _EXPOSURE_DIAG_OPTIONS  diagnostic_options; //!< Defines what diagnostic options are selected for the incoming exposure
    _EXPOSURE_IO            gpio; //!< Status of the gpipo related with the module
    
    _EXPOSURE_DATA          data;//!< Data related to the incoming exposure mode
    _EXPOSURE_STAT          status; //!< Defines what diagnostic options are selected for the incoming exposure
    _EXPOSURE_RESULT        result;
    
}_EXPOSURE_Struct;


ext _EXPOSURE_Struct EXPOSURE_Data;

/**
 * Module Initialization
 */
ext void ExposureInit (void);
ext void rtcExposure15msCallback(void);

ext bool ExposureStartManual2D();
ext void Exposure2DManualSequence(TC_TIMER_STATUS status, uintptr_t context);


ext void ExposureCompleted(_EXPOSURE_ERROR code);

// End Module Definition
#endif 
    
#define _EXPOSURE_C

#include "application.h"
#include "exposure.h"

// Debounce time to fix the current xray button stat
#define XRAYREQ_15ms_DEBOUNCE_TICK 5
static int xrayreq_debounce;

void ExposureInit(void){
    //TC1_TimerCallbackRegister( MasCallback, 0 );
    TC1_TimerStop();
    
    // Xray Button Request INitialization
    EXPOSURE_Data.gpio.xray_request_button = (!uC_SYNC_nREQ_Get());
    xrayreq_debounce = XRAYREQ_15ms_DEBOUNCE_TICK;
}

void rtcExposure15msCallback(void){
    
    // XRAY request Button Debounce Status
    bool buttonstat = (!uC_SYNC_nREQ_Get());
    if(buttonstat == EXPOSURE_Data.gpio.xray_request_button){
        xrayreq_debounce = XRAYREQ_15ms_DEBOUNCE_TICK;
    }else{
        xrayreq_debounce--;
        if(xrayreq_debounce == 0) {
            EXPOSURE_Data.gpio.xray_request_button = buttonstat;
            xrayreq_debounce = XRAYREQ_15ms_DEBOUNCE_TICK;
        }
    }
    
}

bool ExposureStartManual2D(void){    
    // Data Validation
    
    
    // Initialization preparation
    EXPOSURE_Data.status.sequence = 0;
    EXPOSURE_Data.status.status_flag = EXPSTAT_INIZIALIZATION;
    
    TC1_TimerCallbackRegister( Exposure2DManualSequence, 0 );
    TC1_TimerStop();
    return true;
}

void ExposureCompleted(_EXPOSURE_ERROR code){    
    TC1_TimerStop();
    return;
}
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



typedef struct masmeter{
    
    bool mas_test_status; //!< Status of the test current activation
    unsigned short  mas_count;       //!< Number of counted pulses in a integration slot
    unsigned short  anodic_current; //!< Estimated anodi current in the integration slot
    
}_MAS_Struct;

ext _MAS_Struct MAS_Data;

/**
 * Module Initialization
 */
ext void MasInit (void);
ext void rtcMas15msCallback(void);




// End Module Definition
#endif 
    
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
 * \defgroup STARTER_MODULE HS Starter Handling Module
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
 * \addtogroup STARTER_MODULE
 * 
 * ## Application API
 * TBD
 * 
 */



typedef struct starter{
    
    
}_STARTER_Struct;

ext _STARTER_Struct STARTER_Data;

/**
 * Module Initialization
 */
ext void StarterInit (void);
ext void rtcStarter15msCallback(void);





// End Module Definition
#endif 
    
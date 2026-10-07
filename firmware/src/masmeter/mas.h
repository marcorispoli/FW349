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
    
    
}_MAS_Struct;

ext _MAS_Struct MAS_Data;

/**
 * Module Initialization
 */
ext void MasInit (void);





// End Module Definition
#endif 
    
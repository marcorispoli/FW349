#ifndef _GENERATOR_H    
#define _GENERATOR_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _GENERATOR_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup GENERATOR_MODULE Generator Handling Module
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
 * \addtogroup GENERATOR_MODULE
 * 
 * ## Application API
 * TBD
 * 
 */



typedef struct generator{
    unsigned short hvbus; //!< current detected VBUS 
    
    unsigned short hvsens;//!< current detected VSENS
    unsigned short kvsens;//!< current detected KV sens (100V unit)
    
    unsigned short hvref; //!< current hvref value
    unsigned short kvref; //!< current kv ref value (equivalent)
    
    bool hvena_stat;     //!< HV ENA enable signal readback status
    bool sw1_stat;       //!< Current stat of the SW1 switch
    bool sw2_stat;       //!< Current stat of the SW2 switch
    
}_GENERATOR_Struct;

ext _GENERATOR_Struct _GENERATOR_Data;

/**
 * Module Initialization
 */
ext void GeneratorInit (void);

// End Module Definition
#endif 
    
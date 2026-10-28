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



typedef enum {
    STARTER_STOP = 0,
    STARTER_RESET_FAULT,
    STARTER_RUN_HS,
    STARTER_RUN_LS,
}_STARTER_COMMANDS;

typedef struct cmdstruct{
    int blanking_timer;
    int attempt; 
    unsigned char sequence;
    _STARTER_COMMANDS cmd;
    bool executed;
}_COMMAND_DATA;

typedef struct starter{

    bool fault;
    bool speed_ok;
    int stop_timer;
    _COMMAND_DATA command;
    
}_STARTER_Struct;

ext _STARTER_Struct _Starter_Data;


/**
 * Module Initialization
 */
ext void StarterInit (void);
ext void rtcStarter15msCallback(void);

ext void StarterResetFault(void);
ext bool StarterHsRun(void);



// End Module Definition
#endif 
    
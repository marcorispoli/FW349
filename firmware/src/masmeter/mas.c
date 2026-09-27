#define _MAS_C

#include "application.h"
#include "mas.h"


static void MasCallback(TC_TIMER_STATUS status, uintptr_t context){

}

void MasInit(void){
    
    TC0_TimerCallbackRegister( MasCallback, 0 );
    TC0_TimerStart();
}


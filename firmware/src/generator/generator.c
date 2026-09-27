#define _GENERATOR_C

#include "application.h"
#include "generator.h"

static void HVBUS_Callback(ADC_STATUS status, uintptr_t context){   
    if(!(status & ADC_STATUS_RESRDY)) return;   
    return;
}

static void HVSENS_Callback(ADC_STATUS status, uintptr_t context){   
    if(!(status & ADC_STATUS_RESRDY)) return;   
    return;
}


static void HVBUS_Init (void){
    
    // HVBUS Analog Value
    ADC0_CallbackRegister( HVBUS_Callback, 0 );
    ADC0_Enable();
    ADC0_ConversionStart();
}


static void HVSENS_Init (void){
     // HVSENS Analog Input
    ADC1_CallbackRegister( HVSENS_Callback, 0 );
    ADC1_Enable();
    ADC1_ConversionStart();
}

void GeneratorInit(void){
    HVBUS_Init();
    HVSENS_Init();
}

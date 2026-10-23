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
    
    // Inizializza SPI
    uC_CS_Set();
}


bool  GeneratorHvrefSet(unsigned char ctrl, unsigned short value ){
    
    unsigned char buffer[2];
    bool result = false;
    
    buffer[0] = (ctrl&0xF0);
    //buffer[0] |= ((value>>8) & 0x0F);
    // = (value & 0xff);
    buffer[0] |= 0x8;
    buffer[1] = 0xFF;
    uC_CS_Clear();
    result = SERCOM4_SPI_Write((void*) buffer, 2);
    uC_CS_Set();  
    
    return result;
}
#define _GENERATOR_C

#include "application.h"
#include "generator.h"


static bool  GeneratorHvrefSetG1(unsigned short value );
static bool  GeneratorHvrefSetG2(unsigned short value );

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
    
    // Azzeramento Vreference
    GeneratorHvrefSetG1(0);
}

/**
 * This function set the externa dac 4821 to the 
 * requested value with Gain = 1.
 * 
 * The Maximum output voltage will be 2.048V
 * 
 * @param value = digital value [0:4095]
 * 
 * @return 
 */
bool  GeneratorHvrefSetG1(unsigned short value ){
    unsigned char buffer[2];
    bool result = false;
    
    buffer[0] = HVREF_GAINx1|HVREF_RUN;
    buffer[0] |= (unsigned char)  ((value & 0x0F00) >> 8);
    buffer[1] =  (unsigned char)  (value & 0x00FF);
 
    uC_CS_Clear();
    result = SERCOM4_SPI_Write((void*) buffer, 2);
    uC_CS_Set();  
    
    return result;
}

/**
 * This function set the externa dac 4821 to the 
 * requested value with Gain = 2.
 * 
 * The Maximum output voltage will be 4.096V
 * 
 * @param value = digital value [0:4095]
 * @return 
 */
bool  GeneratorHvrefSetG2(unsigned short value ){
    unsigned char buffer[2];
    bool result = false;
    
    buffer[0] = HVREF_GAINx2|HVREF_RUN;
    buffer[0] |= (unsigned char)  ((value & 0x0F00) >> 8);
    buffer[1] =  (unsigned char)  (value & 0x00FF);
 
    uC_CS_Clear();
    result = SERCOM4_SPI_Write((void*) buffer, 2);
    uC_CS_Set();  
    
    return result;
}

/**
 * This function provides a calibration correction factor to be applied to the 
 * DAC calculation. This factor may depend by the taarget kV.
 * 
 * @param kv
 * @return 
 */
float GeneratorHVCorrectionFactor(float kv ){
    
    // To Be Done
    return 1;
}

/**
 * This function set the Vreference of the UC3841 
 * to the value corrispondent to the desidered output HV voltage.
 * 
 * 
 * + Nsec/Nsens = 87
 * + Vreference_3841 = (Vfeedback - 1.4) * 8.32 / 1000 (Input voltage divisor network)
 * + Vreference_3841 = [(Vo/87) - 1.4] * [8.32 / 1000] 
 * + Vdac = N/1000 (Gain = 2)
 * + Vreference_3841 = 2 * Vdac = (2*N/1000);
 * 
 * N = Vreference_3841 * 1000 / 2 = (Vo/87 - 1.4) * 8.32 / 2 
 * N = (Vo / 20,91 - 5,82)
 * 
 * @param kv: target output voltage
 * @return 
 */
bool GeneratorHVSet(float kv ){
    unsigned short N;
    
    // Calcolo N con Gain = 2 e con eventuale correzione introdotta da calibrazione;
    N = (unsigned short) (GeneratorHVCorrectionFactor(kv) * ((kv / 20.91) - 5.82));
    return GeneratorHvrefSetG2(N);
    
}
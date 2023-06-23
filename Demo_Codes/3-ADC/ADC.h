#include <SPI.h>
#include <math.h>

#define CS_ADC_Pwr          0b00000010
#define ADC_CTRL_PORT       PORTH
#define ADC_CTRL_PORT_DIR   DDRH

#define ALM_ADC_Pwr         0b00100000
#define ADC_ALM_PORT        PORTE
#define ADC_ALM_PORT_DIR    DDRE


void ADCinit();
void ADCread(uint16_t channel, uint8_t* rtrnChnl, float *rtrnVolt);
float ADCconvert(uint8_t channel, float voltage);


/*
 * Interorbital Systems' satellite kit demo program
 * Demo-FRAM
 * Version 1.0, July 2020
 * Author: Sylvain BURDOT (SGLB)
 * 
 * This is a demo program to get you started with you satellite. It is neither complete nor optimized to access the full functions of the kit. 
 */

//===============================================================================================
/* This program demonstrate how to read the values of the input on of the analog-digital converter
 * This ADC has several automated or manual mode, we will see only the manual mode. 
 * 
 * Once the values are read they are converted in amps and degrees and printed on the serial port.
 * Note that the current measurment can be unreliable when the current is really small. This is 
 * due to the value of the measuring resistor value being very small.
 */
//===============================================================================================



#include "ADC.h"


void setup() {
  
  Serial.begin(9600);
  SPI.begin();
  Serial.println("\nHELLO WORLD!  Initializing...\n");

  //setting up pins for the analog-digital converter
  ADCinit(); 
  Serial.println("Analog-Digital Converter \tDONE!");

   pinMode(A1, OUTPUT);
   digitalWrite(A1,HIGH);

}

void loop() {


  for (uint16_t channel =0; channel < 16; channel++){ // We are going to loop through all the 16 channels of the ADC
    
    //Let's create a variable that will store the returned channel number
    uint8_t rtrnChnl;
    //Let's create a varibale that will store the raw value of the returned channel
    float rtrnVolt;

    //Now let's ask the ADC for the value of channel N et receive value of channel N-2
    //Because of the way the ADC works, you will receive the value of channel N only two cycles later. Please Refer to the datasheet for that. 
    ADCread(channel, &rtrnChnl, &rtrnVolt);
    //Serial.println(rtrnVolt);

    //The return voltage needs to be converted in values that human will understand for what they are measuring: current and temperature
    float result = ADCconvert(rtrnChnl, rtrnVolt);
    
    //Then these values are printed on the serial port.
    if ( (rtrnChnl >=3) && (rtrnChnl <=6) ){
       Serial.println("Channel #" + String(rtrnChnl) + "\t" + String(result) + " °C");
    }
    else {
       Serial.println("Channel #" + String(rtrnChnl) + "\t" + String(result) + " *10^-3 Amps");
    }
       
  }
  Serial.println();
  delay(2000);
  
}

#include "ADC.h"



void ADCinit(){
  //Sets the chip select pin as OUPUT
  ADC_CTRL_PORT_DIR |= CS_ADC_Pwr;

  //Sets alarm pin as INPUT
  ADC_ALM_PORT_DIR &= ~ALM_ADC_Pwr; 

  //Sets chip select to HIGH
  ADC_CTRL_PORT |= CS_ADC_Pwr;
}





/*
 * This function sends the IMU the N+2 channel number to be read, recovers number of channel N currently being read and its raw value, then converts it to volts 
 * 
 * uint16_t channel: two bytes of commands to be sent, including the channel number. Please refer to the datasheet.
 * uint8_t *rtrnChnl: address of the byte storing the number of the channel being read.
 * float *rtrnVolt: address of the variable storing the voltage value of the channel being read.
 */
void ADCread(uint16_t channel, uint8_t* rtrnChnl, float *rtrnVolt){

  //Let's create a command with paramters found in the datasheet
  uint16_t command = 0b0001100001000000; 
  //Now let's insert the channel number in that command
  command |= (channel <<7); 

  //Sending the command over SPI, and storing the anser in a two bytes variable 
  ADC_CTRL_PORT &= ~CS_ADC_Pwr; 
  uint16_t output = SPI.transfer16(command);
  ADC_CTRL_PORT |= CS_ADC_Pwr;

  //Let's extract the channel from the output 
  *rtrnChnl =(output & 0xF000) >> 12;
  //Let's now extract the raw value from the output
  output &= 0x0FFF;
  //Serial.println(output);
  
  //Finally, let's convert this raw value in voltage 
  *rtrnVolt = (float(output)*5.0/4096.0)*1000;
  
}


/*
 * This function converts the voltage value of a channel to amps or degrees, based on the channel number
 */
float ADCconvert(uint8_t channel, float voltage){

  if (((channel >=0) && (channel <= 2)) || ((channel >=7) && (channel <= 15))){ // if it is a current feedback channel
     // let's convert the voltage into the sensed voltage by the current sensor
     
     float Vsense = voltage /50; // this equation comes fromt the datasheet of the ZXCT1086
     float Isense=0;
     
     // finally, let's convert the sensed volateg into current
     if ((channel >=0) && (channel <= 2)){
        Isense = Vsense / 0.033; // simple conversion to amperage, 0.033 is the value of the sense resistor
        
     }
     else if ((channel >=7) && (channel <= 15)){
        float Isense = Vsense / 0.43; // simple conversion to amperage, 0.43 is the value of the sense resistor
     }
     
     return Isense;
  }
  
  else if ((channel >=3) && (channel <= 5)){ // if it is a battery temperature feedback
     //Let's convert that into ohms, the actual value of the thermistor

     voltage=voltage/1000;
     float Rsense = ((3.3/voltage)-1)*10000; //simple voltage divider equation

     float Temp = 0;
     Rsense = Rsense/1000;
     if(Rsense >= 53.41 & Rsense <=329.5){
        Temp=(log(Rsense/23.869))/(-0.052); //approximated function base on datasheet
     }
     else if(Rsense >= 10 & Rsense < 53.41){
        Temp=(log(Rsense/27.52))/(-0.041); //approximated function base on datasheet
     }
     else if(Rsense >= 2.588 & Rsense < 10){
        Temp=(log(Rsense/22.238))/(-0.033); //approximated function base on datasheet
     }
     else if(Rsense >= 0.7576 & Rsense < 2.588){
        Temp=(log(Rsense/14.481))/(-0.027); //approximated function base on datasheet
     }
     return Temp;
     //return 56;
     
  }
  
  else if (channel ==6){ // if it is the board temperature feedback
     //Let's convert that into ohms, the actual value of the thermistor

     voltage = voltage /1000;
     float Rsense = ((3.3/voltage)-1)*10000; //simple voltage divider equation

     //And finally convert that into degrees
     float Temp = 0;
     Rsense = Rsense/10000;
     if(Rsense >= 2.764 & Rsense < 20.52){
        Temp=(log(Rsense/2.6602))/(-0.05); //approximated function base on datasheet
     }
     else if(Rsense >= 0.5828 & Rsense < 2.764){
        Temp=(log(Rsense/2.725))/(-0.039); //approximated function base on datasheet
     }
     else if(Rsense >= 0.1672 & Rsense < 0.5828){
        Temp=(log(Rsense/1.9928))/(-0.031); //approximated function base on datasheet
     }
     else if(Rsense >= 0.04986 & Rsense < 0.1672){
        Temp=(log(Rsense/1.4248))/(-0.027); //approximated function base on datasheet
     }
     return Temp;
  }
  
}

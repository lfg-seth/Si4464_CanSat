
/*
 * Interorbital Systems' satellite kit demo program
 * Demo-Lab
 * Version 1.0, July 2020
 * Author: Sylvain BURDOT (SGLB)
 * 
 * This is a demo program to get you started with you satellite. It is neither complete nor optimized to access the full functions of the kit. 
 * 
 * 
 */

//===============================================================================================
/* This program will set the radio in a constant carrier wave mode to check the emitting 
 * frequency and power in a lab. 
 * 
 * In this example, communication is only one way. The satellite is acting either as a receiver
 * or a transmitter but ot both.
 * 
 * radio_config_Si4464.h is the configuration file for the transceiver. 
 * In this file are located all the parameters of the different register of the transceiver that 
 * are loaded on boot up. You will have to create your one for your own application. 
 * 
 * WARNING: when setting the output power of the transceiver up, do not go over 0x10.
 * Otherwise the power received at the input of the power amplifier will exceed the maximum value 
 * from the datasheet.
 */
//===============================================================================================


#include "radio.h"




/*
 * This fonction is here for debugging purpose.
 * Its only job is to display on the serial port how the tranceiver was configured.
 */
void printRadioConfig(){
  Serial.println("\n========== RADIO CONFIGURATION DATA ARRAY ==========");
  int byteNumber = 0;
  int cmdNumber =1;

  while(cfg[byteNumber] != 0x00){ // while the length of the command is different of zero (while there is any command left) 

    
    Serial.print("Command #" + String(cmdNumber,DEC) + " ");
    int cmdLength = cfg[byteNumber]; //the first byte is the length of the command
    Serial.print("\t(Length " + String(cmdLength,DEC) + " Bytes): \t");
    
    uint8_t cmd[cmdLength]; //creates a table to save the current command
    byteNumber++;
    
    memcpy(cmd, &cfg[byteNumber], cmdLength);

    for (int i=0; i<cmdLength; i++){
      Serial.print(cmd[i],HEX);
      Serial.print(" ");
    }
    byteNumber += cmdLength;
    cmdNumber ++;
    Serial.println();
   
  }
  Serial.println("====================================================");
}


/*
 * This fonction is here for debugging purpose.
 * Its only job is to display on the info of the transceiver
 */
void printRadioInfo(){
      //Reading radio part info
   if (radioReady()==true){
      uint8_t cmd[]={0x01};
      uint8_t info[8];
      radioCommand(cmd, sizeof(cmd), info, sizeof(info));
      Serial.println("\n========= RADIO INFO =========");
      Serial.println("Model\t\tSi" +String(info[1],HEX) + String(info[2],HEX));

      Serial.println("Revision\t" + String(info[0],HEX));
      Serial.println("PBuild\t\t" + String(info[3],HEX));
      Serial.println("ROM ID\t\t" + String(info[7],HEX));
      
      Serial.println("ID \t\t" + String(info[4],HEX) + String(info[5],HEX));
      Serial.println("Customer ID\t" + String(info[6],HEX));
      Serial.println("==============================");
    }
  else Serial.println("Radio not ready");
  float radioVoltage=0;
  float radioTemp=0;
  radioBatTemp(&radioVoltage, &radioTemp);
  Serial.println( "Radio Voltage: \t\t" +String(radioVoltage) + "V");
  Serial.println("Radio Temperature: \t" + String(radioTemp) + "°C");
}



//===============================================================================================

void setup() {
  
  pinMode(49,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  Serial.println("\nHELLO WORLD!  Initializing...\n");

  // displays the configuration of the radio
  printRadioConfig(); 

  //Let's initialize the transceiver
  int cfgResult = radioInit(); 

  //Now let's make sure the radio was initialized properly
  if (cfgResult == 0){ 
    uint8_t cmd[]= {REQUEST_DEVICE_STATE};
    uint8_t ans[2];
    if (radioCommand(cmd, sizeof(cmd), ans, sizeof(ans))){
      if ((ans[0] == 3) || (ans[0] ==4)){
         Serial.println("Radio init \t\t\tDONE!");
         Serial.println("Radio current channel: \t\t" + String(ans[1]));
         printRadioInfo(); //displays the part info
      }
    }
    
  }
  else{
    Serial.println("ERROR DURING CONFIG: " + String(cfgResult, DEC));
  }
  
 Serial.println();

 
  uint8_t intStatus[]= {GET_INT_STATUS, 0xFB, 0x7F, 0x7F}; //This is the command for reading the interruption status without clearing them
  uint8_t intStatusClear[]= {GET_INT_STATUS, 0x00, 0x00, 0x00}; //This command is the same as the previous one but will clear the interruption status
  uint8_t interr[8];

  //We will now read and print on the serial port the interruptions status
  //This is use to make sure one more time that the transceiver was initialized correctly
  Serial.print("Checking interrupt status: \t");
  if (radioCommand(intStatus, sizeof(intStatus), interr, sizeof(interr))){
    for (int i = 0; i< sizeof(interr); i++){
      Serial.print(interr[i],HEX);
      Serial.print("\t");  
    }
    Serial.println();
  }

  //Now let's check the chip status
  //It is not strictly necessary but gets a bit more  info on the chip status that are not given by the interruption status
  uint8_t cmd2[] = {GET_CHIP_STATUS};
  uint8_t ans[4];
  Serial.print("Checking chip status: \t\t");
  if (radioCommand(cmd2, sizeof(cmd2), ans, sizeof(ans))){
    for (int i = 0; i< sizeof(ans); i++){
      Serial.print(ans[i],HEX);
      Serial.print("\t");  
    }
    Serial.println();
  }

  Serial.println();

  
  Serial.println("End of setup");


  //The transceiver will be emmiting on channel 3
  uint8_t channel = 0x03;
  //Now the transceiver will start emitting permanently
  uint8_t txCmd[] = {START_TX, channel, B10000000, 0x00, 0x08};
  radioCommand(txCmd,sizeof(txCmd));
  //uint8_t chgState[]= {0x34, 0x07};
  //radioCommand(chgState,sizeof(chgState));
  
}


void loop() {

  // The operations done in the loop in this program have no other purpose but to verify that controller is not stuck

  //we are just going to check on the voltage and temperature of the radio every 2 seconds
  float radioVoltage=0;
  float radioTemp=0;
  
  radioBatTemp(&radioVoltage, &radioTemp);
  Serial.println( "Radio Voltage: \t\t" +String(radioVoltage) + "V");
  Serial.println("Radio Temperature: \t" + String(radioTemp) + "°C\n");

  delay(2000);
}

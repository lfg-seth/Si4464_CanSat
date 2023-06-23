
/*
 * Interorbital Systems' satellite kit demo program
 * Demo-FRAM
 * Version 1.0, July 2020
 * Author: Sylvain BURDOT (SGLB)
 * 
 * This is a demo program to get you started with you satellite. It is neither complete nor optimized to access the full functions of the kit. 
 */

//===============================================================================================
/* This program demonstrate how to access the external FRAM memory to read and write data.
 * Please be aware that even if this type of memory is much better that others, it still has 
 * a munber of W/R cyles. In the case of the chip present on the board it is 100 trillion cycles.
 */
//===============================================================================================




#include "FRAM.h"


void setup() {
  
  Serial.begin(9600);
  SPI.begin();
  Serial.println("\nHELLO WORLD!  Initializing...\n");

  // setting up pins for the FRAM memory
  FRAMinit();   
  Serial.println("FRAM Memory \t\t\tDONE!");

}

void loop() {

  Serial.println("FRAM Memory");

  // We are setting up a buffer in which are store the data that we want to write to the memory
  // In this example, these are just random bytes.
  uint8_t write_buf[]={0x1A,0X2A,0X3A,0xA3,0xA2,0xA1};

  // Then we store the address of the memory at which we want to write the data. 
  uint8_t addr[3]={0xBC,0xBC,0xBC};

  // Letting you know on the serial port what is being written and where
  Serial.print("WRITING: ");
  for (int i=0; i < sizeof(write_buf); i++){
    Serial.print("0x" + String(write_buf[i],HEX) + " ");
  }
  Serial.print("@ address: ");
  for (int i=0; i<sizeof(addr); i++){
    Serial.print("0x" + String(addr[i],HEX) + " ");
  }
  Serial.println();

  // Writing the data to the FRAM memory
  FRAMwrite(addr, write_buf, sizeof(write_buf));
  
  // Creating a buffer of 6 bytes, to store the data that will be read from the memory
  uint8_t read_buf[]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  // Reading data from the memory
  FRAMread(addr, read_buf, sizeof(read_buf));

  //Letting you know what has been read from the memory.
  Serial.print("READING: ");
  for (int i=0; i < sizeof(read_buf); i++){
    Serial.print("0x" + String(write_buf[i],HEX) + " ");
  }
  Serial.print("@ address: ");
  for (int i=0; i<sizeof(addr); i++){
    Serial.print("0x" + String(addr[i],HEX) + " ");
  }
  Serial.println();

  delay(2000);
  
  
  
}

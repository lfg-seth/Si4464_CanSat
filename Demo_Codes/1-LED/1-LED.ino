
/*
 * Interorbital Systems' satellite kit demo program
 * Demo-LED
 * Version 1.0, July 2020
 * Author: Sylvain BURDOT (SGLB)
 * 
 * This is a demo program to get you started with you satellite. It is neither complete nor optimized to access the full functions of the kit.  
 */

//===============================================================================================
/*  
 * The LED is wired on the ATmega's pin 96: Analog 1 for Arduino.
 * This program blinks the on-board green LED at 0.5Hz and shows communication via the serial port.
 * 
 */
//===============================================================================================





int i= 0;
void setup() {

  
  Serial.begin(9600);
  pinMode(A1, OUTPUT);

  

  
}

void loop() {

  
  Serial.println("Hello World!");
  Serial.println(i);
  i++;
  
  digitalWrite(A1,HIGH);
  delay(1000);
  digitalWrite(A1,LOW);
  delay(1000);

  
}

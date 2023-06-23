#include "max17048.h"


max17048 battery;

void setup() {

  Serial.begin(9600);

  Serial.println("MAX17048 Example: reading voltage and SoC");
  Serial.println();
  
  battery.reset();
  battery.quickStart();
  delay(1000);

}

void loop() {   

     
  float cellVoltage = battery.getVcell();
  Serial.print("Voltage:\t\t");
  Serial.print(cellVoltage, 4);
  Serial.println("V");

  float stateOfCharge = battery.getSOC();
  Serial.print("State of charge:\t");
  Serial.print(stateOfCharge);
  Serial.println("%");

  Serial.println();

  delay(3000);
}

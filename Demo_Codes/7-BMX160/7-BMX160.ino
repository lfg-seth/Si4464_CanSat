/*!
  * file readAllData.ino
  *
  * Through the example, you can get the sensor data by using getSensorData:
  * get all data of magnetometer, gyroscope, accelerometer.
  *
  * With the rotation of the sensor, data changes are visible.
  *
  * Copyright   [DFRobot](http://www.dfrobot.com), 2016
  * Copyright   GNU Lesser General Public License
  *
  * version  V0.1
  * date  2019-6-25
  */

#include "libBMX160.h"

BMX160 bmx160;
void setup(){
  Serial.begin(9600);
  delay(100);
  Serial.println("Begin init");

  //make sure the other SPI slave are not selected
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //init the hardware bmx160
  if (bmx160.begin() != true){
    Serial.println("init false");
    while(1);
  }
  //bmx160.setLowPower();   //disable the gyroscope and accelerometer sensor
  //bmx160.wakeUp();        //enable the gyroscope and accelerometer sensor
  //bmx160.softReset();     //reset the sensor

  /** @typedef enum{eGyroRange_2000DPS,
   *                eGyroRange_1000DPS,
   *                eGyroRange_500DPS,
   *                eGyroRange_250DPS,
   *                eGyroRange_125DPS
   *                }eGyroRange_t;
   **/
  //bmx160.setGyroRange(eGyroRange_500DPS);

  /** @typedef enum{eAccelRange_2G,
   *                eAccelRange_4G,
   *                eAccelRange_8G,
   *                eAccelRange_16G
   *                }eAccelRange_t;
   */
  //bmx160.setAccelRange(eAccelRange_4G);
  delay(100);
  Serial.println("init done");
}

void loop(){
  bmx160SensorData Omagn, Ogyro, Oaccel;

  /* Get a new sensor event */
  bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  /* Display the magnetometer results (magn is magnetometer in uTesla) */
  Serial.print("M ");
  Serial.print("X: "); Serial.print(Omagn.x_f); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Omagn.y_f); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Omagn.z_f); Serial.print("  ");
  Serial.println("uT");

  /* Display the gyroscope results (gyroscope data is in g) */
  Serial.print("G ");
  Serial.print("X: "); Serial.print(Ogyro.x_f); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Ogyro.y_f); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Ogyro.z_f); Serial.print("  ");
  Serial.println("deg/s");

  /* Display the accelerometer results (accelerometer data is in m/s^2) */
  Serial.print("A ");
  Serial.print("X: "); Serial.print(Oaccel.x_f); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Oaccel.y_f); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Oaccel.z_f); Serial.print("  ");
  Serial.println("m/s^2");

  Serial.println("");

  delay(2000);
}

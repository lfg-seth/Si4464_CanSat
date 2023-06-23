/**
*  @file libBMX160.cpp
*  @date November 2, 2020
*  @author Aurelien Burdot <aurelien.burdot@gmail.com> for Interorbital Systems
*  @version 0.1
*  @bug Tested, no bug known.
*  @brief Lib for BMX160 IMU
*
*  @status WIP
*  @todo
*        - everything
*/

/*
===============================================
bmx160 magnetometer/accelerometer/gyroscope library for Intel(R) Curie(TM) devices.
Copyright (c) 2015 Intel Corporation.  All rights reserved.
Based on MPU6050 Arduino library provided by Jeff Rowberg as part of his
excellent I2Cdev device library: https://github.com/jrowberg/i2cdevlib
===============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "libBMX160.h"

BMX160::BMX160()
{

}

const uint8_t int_mask_lookup_table[13] = {
    BMX160_INT1_SLOPE_MASK,
    BMX160_INT1_SLOPE_MASK,
    BMX160_INT2_LOW_STEP_DETECT_MASK,
    BMX160_INT1_DOUBLE_TAP_MASK,
    BMX160_INT1_SINGLE_TAP_MASK,
    BMX160_INT1_ORIENT_MASK,
    BMX160_INT1_FLAT_MASK,
    BMX160_INT1_HIGH_G_MASK,
    BMX160_INT1_LOW_G_MASK,
    BMX160_INT1_NO_MOTION_MASK,
    BMX160_INT2_DATA_READY_MASK,
    BMX160_INT2_FIFO_FULL_MASK,
    BMX160_INT2_FIFO_WM_MASK
};

bool BMX160::begin()
{
#ifdef USE_SPI_CS
    SPI.begin();
    initSPICS();
    setCSHigh();
    uint8_t uselessBuf[1] = {0};
    readReg(BMX160_SPI_COMM_TEST_ADDR, uselessBuf, 1); // useless read to activate SPI communication
    softReset();
    delay(50);
    Serial.print("ID : ");
    uint8_t val[1] = {0xAA};
    readReg(BMX160_CHIP_ID_ADDR, val, 1);
    Serial.println(val[0]);
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x11);
    delay(50);
    /* Set gyro to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x15);
    delay(50);
    /* Set mag to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x19);
    delay(50);

    Serial.print("init check: ");
    readReg(0x03, val, 1);
    Serial.println(val[0],BIN); // should return 0bXX010101 for normal mode or 0bXX000000 for suspend mode
    Serial.println();

    setMagnConf();
#else
    Wire.begin();
    if (scan() == true){
        softReset();
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x11);
        delay(50);
        /* Set gyro to normal mode */
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x15);
        delay(50);
        /* Set mag to normal mode */
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x19);
        delay(50);
        setMagnConf();
        return true;
    }
    else
        return false;
#endif
  return true;
}

void BMX160::setLowPower(){
    softReset();
    delay(50);
    setMagnConf();
    delay(50);
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x12);
    delay(50);
    /* Set gyro to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x17);
    delay(50);
    /* Set mag to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x1B);
    delay(50);
}

void BMX160::wakeUp(){
    softReset();
    delay(50);
    setMagnConf();
    delay(50);
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x11);
    delay(50);
    /* Set gyro to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x15);
    delay(50);
    /* Set mag to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x19);
    delay(50);
}

bool BMX160::softReset()
{

    uint8_t data = BMX160_SOFT_RESET_CMD;
    writeBmxReg(BMX160_COMMAND_REG_ADDR, data);
    delay(BMX160_SOFT_RESET_DELAY_MS);
}

void BMX160::setMagnConf()
{
    writeBmxReg(BMX160_MAGN_IF_0_ADDR, 0x80);
    delay(50);
    // Sleep mode
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x01);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x4B);
    // REPXY regular preset
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x04);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x51);
    // REPZ regular preset
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x0E);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x52);

    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x02);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x4C);
    writeBmxReg(BMX160_MAGN_IF_1_ADDR, 0x42);
    writeBmxReg(BMX160_MAGN_CONFIG_ADDR, 0x08);
    writeBmxReg(BMX160_MAGN_IF_0_ADDR, 0x03);
    delay(50);
}

void BMX160::setGyroRange(eGyroRange_t bits){
    switch (bits){
        case eGyroRange_125DPS:
            _gyroRange = BMX160_GYRO_SENSITIVITY_125DPS;
            break;
        case eGyroRange_250DPS:
            _gyroRange = BMX160_GYRO_SENSITIVITY_250DPS;
            break;
        case eGyroRange_500DPS:
            _gyroRange = BMX160_GYRO_SENSITIVITY_500DPS;
            break;
        case eGyroRange_1000DPS:
            _gyroRange = BMX160_GYRO_SENSITIVITY_1000DPS;
            break;
        case eGyroRange_2000DPS:
            _gyroRange = BMX160_GYRO_SENSITIVITY_2000DPS;
            break;
        default:
            _gyroRange = BMX160_GYRO_SENSITIVITY_250DPS;
            break;
    }
}

void BMX160::setAccelRange(eAccelRange_t bits){
    switch (bits){
        case eAccelRange_2G:
            _accelRange = BMX160_ACCEL_MG_LSB_2G * 10;
            break;
        case eAccelRange_4G:
            _accelRange = BMX160_ACCEL_MG_LSB_4G * 10;
            break;
        case eAccelRange_8G:
            _accelRange = BMX160_ACCEL_MG_LSB_8G * 10;
            break;
        case eAccelRange_16G:
            _accelRange = BMX160_ACCEL_MG_LSB_16G * 10;
            break;
        default:
            _accelRange = BMX160_ACCEL_MG_LSB_2G * 10;
            break;
    }
}

void BMX160::getAllData(struct bmx160SensorData *magn, struct bmx160SensorData *gyro, struct bmx160SensorData *accel){

    uint8_t data[23] = {0};

    readReg(BMX160_MAG_DATA_ADDR, data, 23);

    Serial.print("data: ");
    for (int i=0; i<=23; i++){
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();

    if(magn){
        magn->x = (int16_t) ((data[1] << 8) | data[0]);
        magn->y = (int16_t) ((data[3] << 8) | data[2]);
        magn->z = (int16_t) ((data[5] << 8) | data[4]);
        magn->x_f = (float)(magn->x) * BMX160_MAGN_UT_LSB;
        magn->x = (uint16_t)magn->x_f;
        magn->y_f = (float)(magn->y) * BMX160_MAGN_UT_LSB;
        magn->y = (uint16_t)magn->y_f;
        magn->z_f = (float)(magn->z) * BMX160_MAGN_UT_LSB;
        magn->z = (uint16_t)magn->z_f; 

        magn->sensortime = (uint32_t)(data[22] << 16 | data[21] << 8 | data[20]);
        magn->sensortime *= BMX160_TIME_RANGE_US; //get time in μS
    }
    if(gyro){
        gyro->x = (int16_t) ((data[9] << 8) | data[8]);
        gyro->y = (int16_t) ((data[11] << 8) | data[10]);
        gyro->z = (int16_t) ((data[13] << 8) | data[12]);
        gyro->x_f = (float)(gyro->x) * _gyroRange;
        gyro->x = (int16_t)gyro->x_f;
        gyro->y_f = (float)(gyro->y) * _gyroRange;
        gyro->y =  (int16_t)gyro->y_f;
        gyro->z_f = (float)(gyro->z) * _gyroRange;
        gyro->z = (int16_t)gyro->z_f;

        gyro->sensortime = (uint32_t)(data[22] << 16 | data[21] << 8 | data[20]);
        gyro->sensortime *= BMX160_TIME_RANGE_US; //get time in μS
    }
    if(accel){
        accel->x = (int16_t) ((data[15] << 8) | data[14]);
        accel->y = (int16_t) ((data[17] << 8) | data[16]);
        accel->z = (int16_t) ((data[19] << 8) | data[18]);
        accel->x_f = (float)(accel->x) * _accelRange;
        accel->x = (uint16_t)accel->x_f;
        accel->y_f = (float)(accel->y) * _accelRange;
        accel->y = (uint16_t)accel->y_f;
        accel->z_f = (float)(accel->z) * _accelRange;
        accel->z = (uint16_t)accel->z_f;


        accel->sensortime = (uint32_t)((data[22] << 16 | data[21] << 8 | data[20]) & 0x00FFFFFF);
        accel->sensortime *= BMX160_TIME_RANGE_US; //get time in μS
    }
}

float BMX160::getTemperature()
{
  uint8_t data[2] = {0};
  int16_t temp = 0;
  float tempC = 0;

  readReg(BMX160_TEMP_DATA_ADDR, data, 2);
  temp = (int16_t)(data[1] << 8 | data[0]);
  tempC = (float)(temp >> 9) + 23; // temp / 512 + 23
  return tempC;
}



#ifdef USE_SPI_CS
void BMX160::writeReg(uint8_t regAddr, uint8_t value) {
  // now combine the register address and the command into one byte:
  regAddr = regAddr & BMX160_SPI_WR_MASK; //WRITE bit 7 : 0
  setCSLow();
  SPI.transfer(regAddr); //Send register location
  SPI.transfer(value);  //Send value to record into register
  setCSHigh();
}

void BMX160::writeReg(uint8_t regAddr, uint8_t *pBuf, uint16_t len)
{
    // now combine the register address and the command into one byte:
  regAddr = regAddr & BMX160_SPI_WR_MASK; //WRITE bit 7 : 0
  setCSLow();
  SPI.transfer(regAddr); //Send register location
  SPI.transfer(*pBuf);  //Send value to record into register
  setCSHigh();
}


void BMX160::readReg(uint8_t regAddr, uint8_t *pBuf, uint16_t len)
{
  regAddr = regAddr | BMX160_SPI_RD_MASK; // Create the read frame
  setCSLow();
  SPI.transfer(regAddr); // Send register location
  for(uint16_t i = 0; i < len; i ++) {
      pBuf[i] = SPI.transfer(0x00); //Receive data
  }
  setCSHigh();
}
#else
void BMX160::writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
    Wire.begin();
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    for(uint16_t i = 0; i < len; i ++)
        Wire.write(pBuf[i]);
    Wire.endTransmission();
}

void BMX160::readReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
    Wire.begin();
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    if(Wire.endTransmission() != 0)
        return;
    Wire.requestFrom(_i2c_addr, (uint8_t) len);
    for(uint16_t i = 0; i < len; i ++) {
        pBuf[i] = Wire.read();
    }
    Wire.endTransmission();
}
#endif

bool BMX160::scanI2C()
{
    Wire.beginTransmission(_i2c_addr);
    if (Wire.endTransmission() == 0){
        return true;
    }
    return false;
}

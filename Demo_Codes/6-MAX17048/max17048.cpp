/**
*  @file max17048.cpp
*  @date 21 October 2020
*  @author Aurelien Burdot <aurelien.burdot@gmail.com> for Interorbital Systems
*  @version 0.1
*  @bug Tested, no bug known.
*  @brief Lib for MAX17048 battery level sensor
*
*  @status WIP
*  @todo
*        - everything
*/
#include "max17048.h"

void max17048::writeRegister(uint8_t address, uint8_t msb_val, uint8_t lsb_val)
{
  Wire.beginTransmission(MAX17048_I2C_ADDRESS);
  Wire.write(address);
  Wire.write(msb_val);
  Wire.write(lsb_val);
  Wire.endTransmission();
}

void max17048::writeRegister(uint8_t address, uint16_t val)
{
  Wire.beginTransmission(MAX17048_I2C_ADDRESS);
  Wire.write(address);
  Wire.write(getMsb(val));
  Wire.write(getLsb(val));
  Wire.endTransmission();
}

uint16_t max17048::readRegister(uint8_t address)
{
  uint8_t msb= 0;
  uint8_t lsb = 0;

  Wire.beginTransmission(MAX17048_I2C_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(MAX17048_I2C_ADDRESS, 2);
  msb = Wire.read();
  lsb = Wire.read();

  return uint8ToUint16(msb, lsb);
}


float max17048::getVcell()
{
  uint16_t value = 0;

	value = readRegister(MAX17048_REG_VCELL);
  //Return Vcell in volts
  return toVolt(value); //(float)
}

float max17048::getSOC()
{
	uint16_t value = 0;

	value = readRegister(MAX17048_REG_SOC);
	return toPercentage(value);	//in %
}

uint8_t max17048::getCompensateValue() {
  uint16_t value = 0;

	value = readRegister(MAX17048_REG_CONFIG);
  return getMsb(value);
}


void max17048::reset() {
	writeRegister(MAX17048_REG_CMD, MAX17048_RESET);
}

void max17048::quickStart() {
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_MODE);

  //Set the QuickStart bit
  bitSet(value, MAX17048_SHIFT_QUICKSTART);
  //Write the value back out
  writeRegister(MAX17048_REG_MODE, value);
}

void max17048::setSleepEnable(bool enabled)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_MODE);

    bitWrite(value, MAX17048_SHIFT_ENSLEEP, enabled);
    //Write the value back out
    writeRegister(MAX17048_REG_MODE, value);
}

bool max17048::isHibernating()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_MODE);

    //Return the status of the HibStat bit
  return bitRead(value, MAX17048_SHIFT_HIBSTAT);
}

void max17048::setSocChangeAlert(bool enabled)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Set or clear the ALSC bit
    bitWrite(value, MAX17048_SHIFT_ALSC, enabled);

    //Write the value back out
    writeRegister(MAX17048_REG_MODE, value);
}

bool max17048::isSocChangeAlertEnabled()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

  return bitRead(value, MAX17048_SHIFT_ALSC);
}

bool max17048::isAlerting()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

  return bitRead(value, MAX17048_SHIFT_ALRT);
}


void max17048::clearAlert()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Clear the ALRT bit
    bitClear(value, MAX17048_SHIFT_ALRT);

    //Write the value back out
    writeRegister(MAX17048_REG_CONFIG, value);
}


bool max17048::isSleeping()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_MODE);

  return bitRead(value, MAX17048_SHIFT_SLEEP);
}

void max17048::setSleep(bool sleep)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Set or clear the SLEEP bit
    bitWrite(value, MAX17048_SHIFT_SLEEP, sleep);

    //Write the value back out
    writeRegister(MAX17048_REG_CONFIG, value);
}

char max17048::getAlertFlags()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_STATUS);

    //Return only the flag bits
    return getMsb(value) & MAX17048_MASK_ALRT;
}

void max17048::clearAlertFlags(char flags)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_STATUS);

    //Clear the specified flag bits
    bitClear(value, MAX17048_SHIFT_RI);
    bitClear(value, MAX17048_SHIFT_VH);
    bitClear(value, MAX17048_SHIFT_VL);
    bitClear(value, MAX17048_SHIFT_VR);
    bitClear(value, MAX17048_SHIFT_HD);
    bitClear(value, MAX17048_SHIFT_SC);

    //Write the value back out
    writeRegister(MAX17048_REG_STATUS, value);
}

void max17048::setVResetAlert(bool enabled)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_STATUS);


    //Set or clear the EnVR bit
    bitWrite(value, MAX17048_SHIFT_ENVR, enabled);

    //Write the value back out
    writeRegister(MAX17048_REG_STATUS, value);
}

bool max17048::isVResetAlertEnabled()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_STATUS);


    //Return the status of the EnVR bit
    return bitRead(value, MAX17048_SHIFT_ENVR );
}

uint16_t max17048::getVersion()
{
  return readRegister(MAX17048_REG_VERSION);
}

bool max17048::isComparatorEnabled()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VRESET_ID);

  return bitRead(value, MAX17048_SHIFT_RI);
}

void max17048::setComparator(bool enabled)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VRESET_ID);

  bitWrite(value, MAX17048_SHIFT_RI, enabled);

  writeRegister(MAX17048_REG_VRESET_ID, value);
}

uint8_t max17048::getId()
{
  return getLsb(readRegister(MAX17048_REG_VRESET_ID));
}

float max17048::getHibernateThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_HIBRT);

  return (getMsb(value) * 0.208);
}


void max17048::setHibernateThreshold(float threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_HIBRT);

    //Mask off the old value
    value &= (~MAX17048_MASK_HIB_THOLD);

    //Do a smart update
    if (threshold > 0.0) {
        if (threshold < 53.04)
            value |= (unsigned short)(threshold / 0.208) << MAX17048_SHIFT_HIB_THOLD;
        else
            value |= MAX17048_MASK_HIB_THOLD;
    }

    //Write the 16-bit register
    writeRegister(MAX17048_REG_HIBRT, value);
}

float max17048::getActiveThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_HIBRT);

    //Extract the active threshold
    return (getLsb(value)) * 0.00125;
}

void max17048::setActiveThreshold(float threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_HIBRT);

    //Mask off the old value
    value &= (~MAX17048_MASK_ACT_THOLD);

    //Do a smart update
    if (threshold > 0.0) {
        if (threshold < 0.31875)
            value |= (uint8_t)(threshold / 0.00125);
        else
            value |= MAX17048_MASK_ACT_THOLD;
    }

    //Write the 16-bit register
    writeRegister(MAX17048_REG_HIBRT, value);
}

void max17048::setCompensation(char rcomp)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Update the register value
    value &= (~MAX17048_MASK_RCOMP);
    value |= rcomp << MAX17048_SHIFT_RCOMP;

    //Write the value back out
    writeRegister(MAX17048_REG_CONFIG, value);
}

void max17048::setTempCompensation(float temp)
{
    //Calculate the new RCOMP value
    char rcomp;
    if (temp > 20.0) {
        rcomp = MAX17048_RCOMP0 + (temp - 20.0) * -0.5;
    } else {
        rcomp = MAX17048_RCOMP0 + (temp - 20.0) * -5.0;
    }

    //Update the RCOMP value
    setCompensation(rcomp);
}

uint8_t max17048::getEmptyAlertThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Extract the threshold
    return 32 - (value & MAX17048_MASK_ALRT_THOLD);
}


//TODO: change values and mask
void max17048::setEmptyAlertThreshold(uint8_t threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_CONFIG);

    //Range check threshold
    if (threshold < 1)
        threshold = 1;
    else if (threshold > 32)
        threshold = 32;

    //Update the register value
    value &= (~MAX17048_MASK_ALRT_THOLD);
    value |= 32 - threshold;

    //Write the 16-bit register
    writeRegister(MAX17048_REG_CONFIG, value);
}

float max17048::getVAlertMinThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VALRT);

    //Extract the alert threshold
    return (getMsb(value) * 0.02);
}

//todo: need to change mask and value
void max17048::setVAlertMinThreshold(float threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VALRT);

    //Mask off the old value
    value &= (~MAX17048_MASK_VALRT_MIN);

    //Do a smart update
    if (threshold > 0.0) {
        if (threshold < 5.1)
            value |= (unsigned short)(threshold / 0.02) << MAX17048_SHIFT_VALRT_MIN;
        else
            value |= MAX17048_MASK_VALRT_MIN;
    }

    //Write the 16-bit register
    writeRegister(MAX17048_REG_VALRT, value);
}


//TODO: change mask
float max17048::getVAlertMaxThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VALRT);

    //Extract the active threshold
    return (getLsb(value) * 0.02);
}

//TODO: change mask
void max17048::setVAlertMaxThreshold(float threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VALRT);

    //Mask off the old value
    value &= (~MAX17048_MASK_VALRT_MAX);

    //Do a smart update
    if (threshold > 0.0) {
        if (threshold < 5.1)
            value |= (char)(threshold / 0.02);
        else
            value |= MAX17048_MASK_VALRT_MAX;
    }

    //Write the 16-bit register
    writeRegister(MAX17048_REG_VALRT, value);
}


//change mask
float max17048::getVResetThreshold()
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VRESET_ID);

    //Extract the threshold
    return (value >> MAX17048_SHIFT_COMP_THOLD) * 0.04;
}


//Todo change mask
void max17048::setVResetThreshold(float threshold)
{
  //Read the current 16-bit register value
  uint16_t value = 0;
  value = readRegister(MAX17048_REG_VRESET_ID);

    //Mask off the old value
    value &= (~MAX17048_MASK_COMP_THOLD);

    //Do a smart update
    if (threshold > 0.0) {
        if (threshold < 5.08)
            value |= (unsigned short)(threshold / 0.04) << MAX17048_SHIFT_COMP_THOLD;
        else
            value |= MAX17048_MASK_COMP_THOLD;
    }

    //Write the 16-bit register
    writeRegister(MAX17048_REG_VRESET_ID, value);
}

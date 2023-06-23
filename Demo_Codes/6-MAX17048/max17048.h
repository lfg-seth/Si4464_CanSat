/**
*  @file max17048.h
*  @date 21 October 2020
*  @author Aurelien Burdot <aurelien.burdot@gmail.com> for Interorbital Systems
*  @version 0.1
*  @bug Tested, no bug known.
*  @brief Lib for MAX17048 battery level sensor
*
*  @status WIP
*  @todo
*        - test, check all settings, values
*        - define in #define all magic number (threshold etc)
*/

#pragma once

#include <Wire.h>
#include <Arduino.h>

#define MAX17048_I2C_ADDRESS      0x36

#define MAX17048_RESET            0x5400

#define MAX17048_RCOMP0           0x97

#define MAX17048_SHIFT_QUICKSTART 14
#define MAX17048_SHIFT_ENSLEEP    13
#define MAX17048_SHIFT_HIBSTAT    12
#define MAX17048_SHIFT_SLEEP      7
#define MAX17048_SHIFT_ALSC       6
#define MAX17048_SHIFT_ALRT       5

#define MAX17048_SHIFT_ENVR       14
#define MAX17048_SHIFT_SC         13
#define MAX17048_SHIFT_HD         12
#define MAX17048_SHIFT_VR         11
#define MAX17048_SHIFT_VL         10
#define MAX17048_SHIFT_VH         9
#define MAX17048_SHIFT_RI         8
#define MAX17048_SHIFT_COMP_THOLD 9
#define MAX17048_SHIFT_VALRT_MIN  8
#define MAX17048_SHIFT_VALRT_MAX  8
#define MAX17048_SHIFT_RCOMP      8
#define MAX17048_SHIFT_HIB_THOLD  8

#define MAX17048_MASK_RCOMP       0xFF00
#define MAX17048_MASK_VALRT_MIN   0xFF00
#define MAX17048_MASK_VALRT_MAX   0x00FF
#define MAX17048_MASK_ALRT        0x003F
#define MAX17048_MASK_ALRT_THOLD  0x001F
#define MAX17048_MASK_COMP_THOLD  0xFE00
#define MAX17048_MASK_HIB_THOLD   0xFF00
#define MAX17048_MASK_ACT_THOLD   0x00FF


#define MAX17048_REG_VCELL        0x02
#define MAX17048_REG_SOC          0x04
#define MAX17048_REG_MODE         0x06
#define MAX17048_REG_VERSION      0x08
#define MAX17048_REG_HIBRT        0x0A
#define MAX17048_REG_CONFIG       0x0C
#define MAX17048_REG_VALRT        0x14
#define MAX17048_REG_CRATE        0x16
#define MAX17048_REG_VRESET_ID    0x18
#define MAX17048_REG_STATUS       0x1A
#define MAX17048_REG_CMD          0xFE

/**
 * @brief Convert raw cel value from the sensor to voltage
 * @param value    raw value of the cell
 * @return       voltage in float
 */
static inline float toVolt(uint16_t value)
{
  return value * 0.000078125;
}

/**
 * @brief convert the rax value of state of charge to percentages
 * @param value    raw state of charge value
 * @return       state of charge un percentages
 */
static inline float toPercentage(uint16_t value)
{
  return value * 0.00390625;
}

/**
 * @brief Get the MSB from a 16bits data
 * @param data    16bits data
 * @return       8 bits MSB
 */
static inline uint8_t getMsb(uint16_t data)
{
  return (uint8_t)((data >> 8) & 0x00FF);
}

/**
 * @brief Get the LSB from a 16 bits data
 * @param data    16 bits data
 * @return       8 bits LSB
 */
static inline uint8_t getLsb(uint16_t data)
{
  return (uint8_t)(data & 0x00FF);
}


/**
 * @brief merge two uint8_t into a uint16_t

 * @param msb    the uint8_t which will be the MSB
 * @param lsb    The uint8_t which will be the LSB
 * @return       The merged uint16_t
 */
static inline uint16_t uint8ToUint16(uint8_t msb, uint8_t lsb)
{
  return ((msb << 8) | lsb);
}

class max17048 {
public:

/**
 * @brief Read the voltage of a cell
 * @return       Voltage in Volt
 */
  float getVcell();

  /**
 * @brief Read the state of charge
 * @return       State of charge in percentages
 */
  float getSOC();

  /**
 * @brief Read the RCOMP value
 * @details
 *  Read the temprature compensate value set in the register
 *
 * @return       RCOMP value
 */
  uint8_t getCompensateValue();

  /**
 * @brief Reset the device
 * @details
 *  Write the reset commande to the register
 */
  void reset();

  /**
 * @brief Device quick start
 * @details
 *  This function can send and receive a single byte to a specific slave
 *  Can be used only by the master
 */
  void quickStart();

  /**
 * @brief enable or desable the sleep mode
 * @details
 *  This function set the EnSleep bit in the register, this allow the device to go on sleep
 * @param enable True or false
 */
  void setSleepEnable(bool enabled);

  /**
 * @brief Check if the device is hibernating
 *
 * @return       True or false
 */
  bool isHibernating();

  /**
 * @brief Set the value to the ALSC register
 * @details
 *  If ALSC is set, then the device alert each time the SoC drop by 1%
 *
 * @param enable    True of false

 */
  void setSocChangeAlert(bool enabled);

  /**
 * @brief Check if the SoC alert is enabled
 *
 * @return       True or false
 */
  bool isSocChangeAlertEnabled();

  /**
 * @brief Get the value of the ALRT register
 * @details
 *  If the ALRT bit is set, then read the STATUS register to know what kind of alert
 *
 * @return       True or false
 */
  bool isAlerting();

  /**
 * @brief Clear the alert bit after reading
 */
  void clearAlert();

  /**
 * @brief Check if the device is forced to sleep
 * @return       True or False
 */
  bool isSleeping();

  /**
 * @brief Set or Clear the sleep bit
 * @details
 *  Force the device to sleep or wake up
 * @param sleep      true or false
 */
  void setSleep(bool sleep);

  /**
 * @brief Read the alert flags to detect what is the alert
 * @details
 *  • VH (voltage high) is set when VCELL has been above ALRT.VALRTMAX.
    • VL (voltage low) is set when VCELL has been below ALRT.VALRTMIN.
    • VR (voltage reset) is set after the device has been reset if EnVr is set.
    • HD (SOC low) is set when SOC crosses the value in CONFIG.ATHD.
    • SC (1% SOC change) is set when SOC changes by at least 1% if CONFIG.ALSC is set.
*
 * @return       Byte with the flags
 */
  char getAlertFlags();

  /**
   * @brief Clear all the alert flags
   */
  void clearAlertFlags(char flags);

  /**
   * @brief Set or unset the voltage reset alert
   *
   * @param enabled   True or false
   */
  void setVResetAlert(bool enabled);

  /**
   * @brief Read the value set in the voltage reset register, enable or desable
   * @return       True or false
   */
  bool isVResetAlertEnabled();
  /**
   * @brief merge two uint8_t into a uint16_t

   * @param msb    the uint8_t which will be the MSB
   * @param lsb    The uint8_t which will be the LSB
   * @return       The merged uint16_t
   */

   /**
    * @brief Read the product version of the IC
    * @return       Product version pf the IC
    */
  uint16_t getVersion();

  /**
   * @brief Read if the comparator is enable or desable
   * @return       True or false
   */
  bool isComparatorEnabled();

  /**
   * @brief Enable or Desable the comparator
   * @param enabled    True or false
   */
  void setComparator(bool enabled);

  /**
   * @brief Read the product ID
   * @details ID is an 8-bit read-only value that is one-time programmable at the factory,
      which can be used as an identifier to distinguish multiple cell types in production.
      Writes to these bits are ignored.
   * @return       The ID
   */
  uint8_t getId();

  /**
   * @brief Read the hibernate threshold
   * @return       The hibernate threshold value (float)
   */
  float getHibernateThreshold();

  /**
   * @brief Set the hibernate threshold value

   * @param threshold    The hibernate threshold
   */
  void setHibernateThreshold(float threshold);

  /**
   * @brief Read the active threshold set in the register
   *
   * @return       threshold
   */
  float getActiveThreshold();

  /**
   * @brief Set the threshold that is going to get the IC out of hibernate state

   * @param threshold    Active threshold (1 lsb = 1,25mV)
   */
  void setActiveThreshold(float threshold);

  /**
   * @brief Store the temprature compensation value into the correct register

   * @param rcomp    temperature compensation setting
   */
  void setCompensation(char rcomp);

  /**
   * @brief Set the value for the temprature compensation

   * @param temp    temp of the cell
   */
  void setTempCompensation(float temp);

  /**
   * @brief Get the threshold of the empty battery alert
   * @return       Empty battery threshold
   */
  uint8_t getEmptyAlertThreshold();

  /**
   * @brief Set the low SOC battery threshold alert
   * @param data   threshold
   */
  void setEmptyAlertThreshold(uint8_t threshold);

  /**
   * @brief Get the undervoltage threshold alert
   * @return       undervoltage threshold
   */
  float getVAlertMinThreshold();

  /**
   * @brief Get the overvoltage threshold alert
   * @return      overvoltage
   */
  float getVAlertMaxThreshold();

  /**
   * @brief Set the undervoltage threshold alert
   * @details This function will set the threshold which will alert through
      the device alert when the battery is undervoltage.
    1 Lsb = 20mV
   * @param threshold    undervoltage threshold
   */
  void setVAlertMinThreshold(float threshold);

  /**
   * @brief Set the overvoltage threshold alert
   * @details This function will set the threshold which will alert through
      the device alert when the battery is overvoltage.
    1 Lsb = 20mV
   * @param threshold    overvoltage threshold
   */
  void setVAlertMaxThreshold(float threshold);

  /**
   * @brief Get the threshold set for ther battery removal detection
   * @return Vreset threshold value
   */
  float getVResetThreshold();
  /**
   * @brief Set the battery removal detection
   * @details adjusts a fast analog comparator and a
 slower digital ADC threshold to detect battery removal
 and reinsertion. For captive batteries, set to 2.5V. For
 removable batteries, set to at least 300mV below the
 application’s empty voltage, according to the desired
 reset threshold for your application. If the comparator
 is enabled, the IC resets 1ms after VCELL rises above
 the threshold. Otherwise, the IC resets 250ms after the
 VCELL register rises above the threshold.
   * @param threshold    VRESET threshold
   */

  void setVResetThreshold(float threshold);

private:
  void writeRegister(uint8_t address, uint8_t msb_val, uint8_t lsb_val);
  void writeRegister(uint8_t address, uint16_t value);
  uint16_t readRegister(uint8_t address);

};

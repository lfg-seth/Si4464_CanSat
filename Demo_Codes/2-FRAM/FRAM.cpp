#include "FRAM.h"


void FRAMinit(){
  //Let set the CS_FRAM pin, HOLD and WP as output
  FRAM_CTRL_PORT_DIR |= CS_FRAM;
  FRAM_SAFE_PORT_DIR |= (HOLD | WP);

  //Now let's set CS_FRAM, hold and WP high
  FRAM_CTRL_PORT |= CS_FRAM;
  FRAM_SAFE_PORT |= (HOLD | WP);
  
}



/*
 * This function copies the data from a buffer a variable size to a given address
 * 
 * uint8_t addr[3]: array coontaining the 3 bytes of the address to write to
 * uint8_t write_buff[]: Array of variable size containing the bytes to copy
 * int write_length: number of bytes to copy
 */
void FRAMwrite(uint8_t addr[3], uint8_t write_buff[], int write_length ){

  //Before being able to write to the memory, the lock preventing to write must be disable
  FRAM_CTRL_PORT &= ~CS_FRAM; 
  SPI.transfer(WREN);
  FRAM_CTRL_PORT |= CS_FRAM;
  
  //Since the lock has been disabled, one can now write to the memory
  FRAM_CTRL_PORT &= ~CS_FRAM; 
  SPI.transfer(WRITE); 
  for (int i=0; i<3; i++){ //Let's first trasnfer the address
    SPI.transfer(addr[i]);
  }
  for (int i=0; i<write_length; i++){ //Then let's transfer the actual data without setting the Chip Select pin back up
    SPI.transfer(write_buff[i]);
  }
  FRAM_CTRL_PORT |= CS_FRAM;
}

/*
 * This function copies the data from the memory at a given address into a buffer of variable size 
 * 
 * uint8_t addr[3]: array coontaining the 3 bytes of the address to write to
 * uint8_t *read_buff: pointer to the buffer to which the bytes will be copied.
 * int read_length: number of bytes to read
 */
void FRAMread(uint8_t addr[3], uint8_t *read_buff, int read_length[]){

  byte value=0x00; //temporary variable to store the read data

  FRAM_CTRL_PORT &= ~CS_FRAM;
  SPI.transfer(READ);
  for (int i=0; i<3; i++){ //Let's transfer the address first
    SPI.transfer(addr[i]);
  }
  for (int i=0; i<read_length; i++){ //Then receive the bytes 
    read_buff[i]=SPI.transfer(0x00);
  }
  FRAM_CTRL_PORT |= CS_FRAM;

}

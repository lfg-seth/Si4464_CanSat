#include <SPI.h>



#define CS_FRAM               0b00000001
#define FRAM_CTRL_PORT        PORTH
#define FRAM_CTRL_PORT_DIR    DDRH

#define HOLD                  0b00001000
#define WP                    0b00000100
#define FRAM_SAFE_PORT        PORTE
#define FRAM_SAFE_PORT_DIR    DDRE


#define WREN    0x06 // set write enable latch command
#define READ    0x03 // read memory data command
#define WRITE   0x02 // write memory data command

void FRAMinit();
void FRAMwrite(uint8_t addr[3], uint8_t write_buff[], int write_length );
void FRAMread(uint8_t addr[3], uint8_t *read_buff, int read_length[]);

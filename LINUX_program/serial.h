#ifndef SERIAL_H
#define SERIAL_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>

/* Defines */
#define FILENAME_SIZE 1024
#define TTYLINE_SIZE 256
#define CFGSTR_SIZE 64
#define GECKO_MESSAGE_NUMBER 2
#define GECKO_SERIAL_PORTNAME "/dev/ttyACM0"


/* Structs and Typedefs */
struct BNUM_speed {
	uint32_t speed;
	uint32_t bnum;
};

typedef enum{
  START = 0
}geckoMessage_t;

typedef struct
{
  geckoMessage_t name;
  char code;
} messageTable_t;

/* Variables */
extern struct BNUM_speed gSpeedTable[31];
extern messageTable_t gMessageTable[GECKO_MESSAGE_NUMBER];
extern uint32_t gSerialSpeed;
extern uint32_t gTermioSpeed;
extern char gInfile[FILENAME_SIZE];
extern char gOutfile[FILENAME_SIZE];
extern char gSerialDevice[CFGSTR_SIZE];

/* Function prototypes */
bool setGlobalSpeed(int speed);
bool initGeckoSerial(int *fd);
void sendGeckoSerial(int fd, geckoMessage_t msg);
void receiveGeckoSerial(int fd, char *buffer);


#endif

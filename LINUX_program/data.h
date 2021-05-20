#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "serial.h"

/* Constants */
#define DATA_FILE_MAX_LINES 256

/* Structures */
typedef struct
{
  uint16_t responseTime;
  uint16_t luminosity;
} dataPair_t;

/* Function Prototypes */
bool getDataFromBuffer(uint16_t *rtime, uint16_t *lum, char *buf);
void writeDataToFile(void);
bool readFromFile(void);
void processData (char *buffer);
void flushData (void);
uint32_t dataAverage(void);
void addNewData(char *buf);

#endif

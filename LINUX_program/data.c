#include "data.h"
#include "serial.h"

/* Variables */
dataPair_t dataTable[DATA_FILE_MAX_LINES];
uint8_t dataCount = 0;

/* Function Definitions */
bool getDataFromBuffer(uint16_t *rtime, uint16_t *lum, char *buf)
{
  uint8_t i;
  char rtimeBuf[7], lumBuf[7];

  if (strlen(buf) != SERIAL_DATA_LENGTH) {
    return false;
  }

  rtimeBuf[6] = lumBuf[6] = '\0';
  for(i = 0; i < 5; i++) {
    rtimeBuf[i] = buf[i];
    lumBuf[i] = buf[i+6];
  }

  *rtime = (uint16_t) atoi(rtimeBuf);
  *lum   = (uint16_t) atoi(lumBuf);

  return true;
}

void writeDataToFile(void)
{
  int i;
  FILE *fd = fopen("data.txt", "r");

  for (i = 0; i < dataCount; i++){
    fprintf(fd, "%u,%u\n", dataTable[i].responseTime, dataTable[i].luminosity);
  }
}

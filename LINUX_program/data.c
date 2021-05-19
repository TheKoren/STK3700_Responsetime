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
  FILE *fd = fopen("data.txt", "w");

  for (i = 0; i < dataCount; i++){
    fprintf(fd, "%u,%u\n", dataTable[i].responseTime, dataTable[i].luminosity);
  }
}

bool readFromFile(void)
{
  FILE *fd = fopen("data.txt", "r");
  if(fp == NULL)
  {
    perror("Failed to open file 'data.txt'");
    return false;
  }

  uint16_t data1 = 0;
  uint16_t data2 = 0;
  dataCount = 0;
  while(fscanf(fd,"%u,%u\n", &data1, &data2) != 0)
  {
      dataTable[dataCount].responseTime = data1;
      dataTable[dataCount].luminosity   = data2;
      dataCount = dataCount + 1;
      /*Szerintem ha megjavítjuk a programot, akkor jobb lesz. - Dani 05.19. */
  }
  fclose(fd);
  return true;
}

void processData (char * buffer)
{
  uint16_t rtime;
  uint16_t lum;
  getDataFromBuffer(&rtime, &lum, buffer);
  dataTable[dataCount].responseTime = rtime;
  dataTable[dataCount].luminosity   = lum;

  writeDataToFile();

  /*
  *TODO: dataCount most index, vagy konkrétan dataCount?
  * Ha csak 1 érték van, akkor dataCount 1 vagy 0?
  * Általam írt érintett függvények: processData, cli_caseMeanTime, readFromFile
  */
}

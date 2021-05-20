#include "cli.h"

#define CLI_HELP_MESSAGE "Response Time Measurement Application\n\n \
Options:\n \
\t -h ........... This help message.\n \
\t -s <speed>.... Set serial speed.\n  \
\t -m ........... Start reaction time measurement.\n \
\t -a ........... Print out the average reaction time.\n \
\t -c ........... Delete all measurement data.\n \
\n"

//TODO: -a kiírja a reakcióidők átlagát
//      -c törli a data.txt fájl tartalmát kész
int commandInterpreter(int argc, char *const *argv)
{
  int opt = 0;

  while((opt = getopt(argc, argv, "hs:mac")) != -1){
    switch(opt){
      case 'h':
        cli_caseHelp();
        break;
      case 's':
        cli_caseSpeed(optarg);
        break;
      case 'm':
        cli_caseMeasure();
        break;
      case 'a':
        cli_caseMeanTime();
        break;
      case 'c':
        cli_caseDeleteData();
        break;
    }
  }
}

void cli_caseHelp(void)
{
  printf(CLI_HELP_MESSAGE);
  exit(EXIT_SUCCESS);
}

void cli_caseSpeed(char* optarg)
{
  int fd;
  int speed = atoi(optarg);
  if (!(setGlobalSpeed(speed))) {
    printf("Can't set speed to %d\n", speed);
    exit(EXIT_FAILURE);
  } else {
    if (initGeckoSerial(&fd)){
      printf("Speed is now %d\n", gSerialSpeed);    
    }
  }
}

void cli_caseMeasure(void)
{
  char buffer[SERIAL_DATA_LENGTH];
  int fd = open(gSerialDevice, O_RDWR);
  sendGeckoSerial(fd, START);
  receiveGeckoSerial(fd, buffer);
  addNewData(buffer);
}

void cli_caseMeanTime(void)
{
  readFromFile();
  uint32_t mean = dataAverage();
  printf("Mean of response times: %u\n", mean);

}
void cli_caseDeleteData()
{
  flushData();
}

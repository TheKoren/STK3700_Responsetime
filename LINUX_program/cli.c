#include "cli.h"
#include "serial.h"

#define CLI_HELP_MESSAGE "Response Time Measurement Application\n\n \
Options:\n \
\t -h ........... This help message.\n \
\t -s <speed>.... Set serial speed.\n  \
\t -m ........... Start responsetime measurement.\n \
\n"


int commandInterpreter(int argc, char *const *argv)
{
  int opt = 0;

  while((opt = getopt(argc, argv, "hs:m")) != -1){
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
    initGeckoSerial(&fd);
    printf("Speed is now %d\n", gSerialSpeed);
  }
}

void cli_caseMeasure(void)
{
  char buffer[TTYLINE_SIZE];
  int fd = open(gSerialDevice, O_RDWR);
  sendGeckoSerial(fd, START);
  receiveGeckoSerial(fd, buffer);
  //TODO: handle data received
}

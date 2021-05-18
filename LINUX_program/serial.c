#include "serial.h"

/* Variable definitions */
char gSerialDevice[CFGSTR_SIZE] = GECKO_SERIAL_PORTNAME;
uint32_t gSerialSpeed = 0;
uint32_t gTermioSpeed = 0;
char gInfile[FILENAME_SIZE] = "stdin";
char gOutfile[FILENAME_SIZE] = "stdout";

struct BNUM_speed gSpeedTable[] = {
	{ 50 , B50 },
  { 75 , B75 },
	{ 110 , B110 },
	{ 134 , B134 },
	{ 150 , B150 },
	{ 200 , B200 },
	{ 300 , B300 },
	{ 600 , B600 },
	{ 1200 , B1200 },
	{ 1800 , B1800 },
	{ 2400 , B2400 },
	{ 4800 , B4800 },
	{ 9600 , B9600 },
	{ 19200 , B19200 },
	{ 38400 , B38400 },
	{ 57600 , B57600 },
	{ 115200, B115200 },
	{ 230400 , B230400 },
	{ 460800 , B460800 },
	{ 500000 , B500000 },
	{ 576000 , B576000 },
	{ 921600 , B921600 },
	{ 1000000 , B1000000 },
	{ 1152000 , B1152000 },
	{ 1500000 , B1500000 },
	{ 2000000 , B2000000 },
	{ 2500000 , B2500000 },
	{ 3000000 , B3000000 },
	{ 3500000 , B3500000 },
	{ 4000000 , B4000000 },
	{ 0 , 0 } // Null termination, keep as the last element...
};

messageTable_t gMessageTable[GECKO_MESSAGE_NUMBER] =
{
  { HELLO, 'H'},
  { START, 'S'}
};


/* Functions */
bool setGlobalSpeed(int speed)
{
  int i;
  for (i = 0; gSpeedTable[i].speed != speed; i++){
    if (gSpeedTable[i].speed == 0){
      return false;
    }
  }
  gTermioSpeed = gSpeedTable[i].bnum;
  gSerialSpeed = gSpeedTable[i].speed;
  return true;
}

bool initGeckoSerial(int *fd)
{
  struct termios geckoSerial;
  int local_fd = 0;
  memset(&geckoSerial, 0, sizeof(geckoSerial));

  geckoSerial.c_iflag = 0;
  geckoSerial.c_oflag = 0;
  geckoSerial.c_cflag = CS8|CREAD|CLOCAL;
  geckoSerial.c_lflag = 0;
  geckoSerial.c_cc[VMIN] = 1;
  geckoSerial.c_cc[VTIME] = 5;

  local_fd = open(gSerialDevice, O_RDWR);
  if (local_fd < 0){
    perror("Serial device open failed\n");
    return false;
  }

  cfsetospeed(&geckoSerial, gTermioSpeed);
  cfsetispeed(&geckoSerial, gTermioSpeed);

  tcsetattr(local_fd, TCSANOW, &geckoSerial);

  *fd = local_fd;
  return true;
}

void sendGeckoSerial(int fd, geckoMessage_t msg)
{
  write(fd, &(gMessageTable[msg].code), 1);
  usleep(100);
}

void receiveGeckoSerial(int fd, char *buffer)
{
	read(fd, bufffer, TTYLINE_SIZE);
}

#ifndef CLI_H
#define CLI_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

#include "data.h"

/* Functions */
int commandInterpreter(int argc, char *const *argv);
void cli_caseHelp(void);
void cli_caseSpeed(char* optarg);
void cli_caseMeasure(void);



#endif

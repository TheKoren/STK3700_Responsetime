#ifndef MYSOURCEFILES_INC_UART_H_
#define MYSOURCEFILES_INC_UART_H_

#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "stdbool.h"
#include "kijelzo.h"
#include <stdio.h>

char gUartBuffer[256];

void uartInit(void);
void UART0_RX_IRQHandler(void);

_Bool uartGetLine(char *buffer);
void uartSendLine(char *line, uint8_t length);
void uartSendChar(char c);

void uartCommandInterpreter(char c);

#endif /* MYSOURCEFILES_INC_UART_H_ */

#include "uart.h"


void uartInit(void)
{
    CMU_ClockEnable(cmuClock_UART0, true);
    USART_InitAsync_TypeDef uartInit = USART_INITASYNC_DEFAULT;
    USART_InitAsync(UART0, &uartInit);

    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);

    UART0->ROUTE |= (USART_ROUTE_TXPEN | USART_ROUTE_RXPEN);
    UART0->ROUTE |= (USART_ROUTE_LOCATION_LOC1);

    USART_IntEnable(UART0,UART_IF_RXDATAV);
    NVIC_EnableIRQ(UART0_RX_IRQn);
    startcode = '0';

}


void uartSendLine(char *line, uint8_t length)
{
    uint8_t i;
    for(i = 0; i < length; i++){
        USART_Tx(UART0, line[i]);
    }
}

void uartSendChar(char c)
{
    USART_Tx(UART0, c);
}

void uartCommandInterpreter(char c)
{
    //TODO: Perform actions based on char received, delete next lines
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    kijelzoPrint(buf);
    uartSendChar(c);
    if ('\r' == c)
        uartSendChar('\n');

}

void UART0_RX_IRQHandler(void)
{
    startcode = (char) USART_RxDataGet(UART0);

    //uartCommandInterpreter(c);

}

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#include "em_device.h"
#include "sl_iostream.h"
#include "sl_iostream_uart.h"
#include "sl_iostream_usart.h"
// Include instance config 
 #include "sl_iostream_uart_vcom_config.h"

// MACROs for generating name and IRQ handler function  
#define SL_IOSTREAM_UART_CONCAT_PASTER(first, second, third)        first ##  second ## third
 




#define SL_IOSTREAM_UART_TX_IRQ_NUMBER(periph_nbr)     SL_IOSTREAM_UART_CONCAT_PASTER(UART, periph_nbr, _TX_IRQn)        
#define SL_IOSTREAM_UART_RX_IRQ_NUMBER(periph_nbr)     SL_IOSTREAM_UART_CONCAT_PASTER(UART, periph_nbr, _RX_IRQn)        
#define SL_IOSTREAM_UART_TX_IRQ_HANDLER(periph_nbr)    SL_IOSTREAM_UART_CONCAT_PASTER(UART, periph_nbr, _TX_IRQHandler)  
#define SL_IOSTREAM_UART_RX_IRQ_HANDLER(periph_nbr)    SL_IOSTREAM_UART_CONCAT_PASTER(UART, periph_nbr, _RX_IRQHandler)  
#define SL_IOSTREAM_UART_CLOCK_REF(periph_nbr)         SL_IOSTREAM_UART_CONCAT_PASTER(cmuClock_, UART, periph_nbr)       


sl_status_t sl_iostream_uart_init_vcom(void);


// Instance(s) handle and context variable 
static sl_iostream_uart_t sl_iostream_vcom;
sl_iostream_t *sl_iostream_vcom_handle = &sl_iostream_vcom.stream;
sl_iostream_uart_t *sl_iostream_uart_vcom_handle = &sl_iostream_vcom;
static sl_iostream_usart_context_t  context_vcom;
static uint8_t  rx_buffer_vcom[SL_IOSTREAM_UART_VCOM_RX_BUFFER_SIZE];
sl_iostream_instance_info_t sl_iostream_instance_vcom_info = {
  .handle = &sl_iostream_vcom.stream,
  .name = "vcom",
  .type = SL_IOSTREAM_TYPE_UART,
  .periph_id = SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO,
  .init = sl_iostream_uart_init_vcom,
};



sl_status_t sl_iostream_uart_init_vcom(void)
{
  sl_status_t status;
  USART_InitAsync_TypeDef init_vcom = USART_INITASYNC_DEFAULT;
  init_vcom.baudrate = SL_IOSTREAM_UART_VCOM_BAUDRATE;
  init_vcom.parity = SL_IOSTREAM_UART_VCOM_PARITY;
  init_vcom.stopbits = SL_IOSTREAM_UART_VCOM_STOP_BITS;

  sl_iostream_usart_config_t config_vcom = { 
    .usart = SL_IOSTREAM_UART_VCOM_PERIPHERAL,
    .clock = SL_IOSTREAM_UART_CLOCK_REF(SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO),
    .tx_port = SL_IOSTREAM_UART_VCOM_TX_PORT,
    .tx_pin = SL_IOSTREAM_UART_VCOM_TX_PIN,
    .rx_port = SL_IOSTREAM_UART_VCOM_RX_PORT,
    .rx_pin = SL_IOSTREAM_UART_VCOM_RX_PIN,
#if defined(GPIO_USART_ROUTEEN_TXPEN)
    .usart_index = SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO,
#elif defined(USART_ROUTEPEN_RXPEN)
    .usart_tx_location = SL_IOSTREAM_UART_VCOM_TX_LOC,
    .usart_rx_location = SL_IOSTREAM_UART_VCOM_RX_LOC,
#if defined(SL_IOSTREAM_UART_VCOM_CTS_PORT)
    .usart_cts_location = SL_IOSTREAM_USART_VCOM_CTS_LOC,
#endif
#if defined(SL_IOSTREAM_UART_VCOM_RTS_PORT)
    .usart_rts_location = SL_IOSTREAM_UART_VCOM_RTS_LOC,
#endif
#else
    .usart_location = SL_IOSTREAM_UART_VCOM_ROUTE_LOC,
#endif
  };
  sl_iostream_uart_config_t uart_config_vcom = {
    .tx_irq_number = SL_IOSTREAM_UART_TX_IRQ_NUMBER(SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO),
    .rx_irq_number = SL_IOSTREAM_UART_RX_IRQ_NUMBER(SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO),
    .rx_buffer = rx_buffer_vcom,
    .rx_buffer_length = SL_IOSTREAM_UART_VCOM_RX_BUFFER_SIZE,
    .lf_to_crlf = SL_IOSTREAM_UART_VCOM_CONVERT_BY_DEFAULT_LF_TO_CRLF,
    .rx_when_sleeping = SL_IOSTREAM_UART_VCOM_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION,
  };
  // Instantiate usart instance 
  status = sl_iostream_usart_init(&sl_iostream_vcom,
                                  &uart_config_vcom,
                                  &init_vcom,
                                  &config_vcom,
                                  &context_vcom);
  EFM_ASSERT(status == SL_STATUS_OK);

  return status;
}



void sl_iostream_uart_init_instances(void)
{
  // Instantiate uart instance(s) 
  
  sl_iostream_uart_init_vcom();
  
}

 
// VCOM IRQ Handler
void SL_IOSTREAM_UART_TX_IRQ_HANDLER(SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO)(void)
{
  sl_iostream_usart_irq_handler(sl_iostream_vcom.stream.context);
}

void SL_IOSTREAM_UART_RX_IRQ_HANDLER(SL_IOSTREAM_UART_VCOM_PERIPHERAL_NO)(void)
{
  sl_iostream_usart_irq_handler(sl_iostream_vcom.stream.context);
}


#if defined(SL_CATALOG_POWER_MANAGER_PRESENT) && !defined(SL_CATALOG_KERNEL_PRESENT)
 
sl_power_manager_on_isr_exit_t sl_iostream_uart_vcom_sleep_on_isr_exit(void)
{
  return sl_iostream_uart_sleep_on_isr_exit(&sl_iostream_vcom);
}

#endif
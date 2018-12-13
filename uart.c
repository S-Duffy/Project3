/*
 * uart.c
 *
 *  Created on: Nov 22, 2018
 *      Author: Sean
 */
#include <MKL25Z4.H>
#include "stdio.h"
#include "stdint.h"
#include "board.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\fifo.h"
#include "fsl_clock_manager.h"
#include "fsl_smc_hal.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
//#include "fsl_lptmr_driver.h"
//#include "fsl_debug_console.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#if defined(UART_INSTANCE_COUNT)
#include "fsl_uart_hal.h"
#endif
#if defined(LPUART_INSTANCE_COUNT)
#include "fsl_lpuart_hal.h"
#endif
#if defined(UART0_INSTANCE_COUNT)
#include "fsl_lpsci_hal.h"
#endif
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#if (defined(USB_INSTANCE_COUNT) &&  (defined(BOARD_USE_VIRTUALCOM)))
  #include "usb_device_config.h"
  #include "usb.h"
  #include "usb_device_stack_interface.h"
  #include "usb_descriptor.h"
  #include "virtual_com.h"
#endif


//FIFO_TYPE* UARTRxFifoPtr = NULL;
FIFO_TYPE* UARTTxFifoPtr = NULL;
// 1. Hardware initialization including pin MUX control,
// clock configuration,
// baud rate and serial format set up
void UART_Init(void)
{

  // From FRMD-KL25Z.pdf:
  // 5.3.3 Serial Port
  // The primary serial port interface signals are PTA1
  // and PTA2. These signals are connected to both the
  // OpenSDA and to the J1 I/O Connector.
  // Therefore, use UART0
//  int baud = 115200;
//  register uint16_t sbr;

//  CLOCK_SYS_EnablePortClock(PORTA_IDX);
//  UARTRxFifoPtr = FIFO_Init(100);
  UARTTxFifoPtr = FIFO_Init(350);


  // PTA1 ALT2 UART0_RX
  // PTA2 ALT2 UART0_TX
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

  // Ensure Tx and Rx are disabled while configuring
  UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );

  UART0_C1 = 0;

  // ALT function 2 of pins 1 and 2
  PORTA_PCR1 |= PORT_PCR_MUX(0x02);
  PORTA_PCR2 |= PORT_PCR_MUX(0x02);

  (*(volatile uint32_t*)((uintptr_t)&(SIM->SOPT2) | BME_BFI_MASK(SIM_SOPT2_UART0SRC_SHIFT,SIM_SOPT2_UART0SRC_WIDTH)) = \
		  ((uint32_t)(0x1) << SIM_SOPT2_UART0SRC_SHIFT));

//  dbg_uart_init();
  DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleLPSCI);

  // Set up Interrupts
  UART0_C2 |= UART0_C2_RIE_MASK;
  UART0_C2 |= UART0_C2_TIE_MASK;  // UART0_C2_TCIE_MASK
  NVIC_EnableIRQ(UART0_IRQn);

  // Enable Transmit and Receive
  UART0_C2 |= UART0_C2_TE_MASK; //(UART0_C2_TE_MASK| UART0_C2_RE_MASK );
}

// 2. Function to check whether the transmitter is
// available to accept a new character for transmission
extern uint32_t peak;
extern void MAIN_PutFifoDigits(FIFO_TYPE* fifo, uint64_t value);
uint8_t UART_TxReady(void)
{
  return (UART0_S1 & UART0_S1_TDRE_MASK);
}

// 3. Function to transmit a character
// assuming transmitter is available.
void UART_Transmit(uint8_t dataByte)
{
  UART0_D = dataByte;
}

// 4. Function written in terms of the first two
// (2 and 3 above) wait (block) for the transmitter
// to be available and then once available
// transmits a character and returns
void UART_BlockOutChar(uint8_t dataByte)
{
  while(!UART_TxReady()){}

  UART_Transmit(dataByte);
}

// 5a.Complementary function to 2
// for receiving a character using the UART
uint8_t UART_RxReady(void)
{
  return (UART0_S1 & UART0_S1_RDRF_MASK);
}

// 5b. Complementary function to 3
// for receiving a character using the UART
void UART_Receive(uint8_t* dataByte)
{
  *dataByte = UART0_D;
}

// 5c. Complementary functions to 4
// for receiving a character using the UART
void UART_BlockInChar(uint8_t* dataByte)
{
  while(!UART_RxReady()){}

  UART_Receive(dataByte);
}

uint8_t newDataSemaphore = 0;
uint8_t printingBlock = 0;
uint64_t characterHistogram[256];
void UART0_IRQHandler(void)
{
  uint8_t dataByte;
  if((FIFO_NumElements(UARTTxFifoPtr) > 0))
  {
    if(UART_TxReady())
    {
      UART0_C2 &= ~UART0_C2_TIE_MASK;  // UART0_C2_TCIE_MASK
      UART0_C2 |= UART0_C2_TCIE_MASK;
	  FIFO_Get(UARTTxFifoPtr, &dataByte);
      UART_Transmit(dataByte);
    }
  }
  else
  {
	MAIN_PutFifoDigits(UARTTxFifoPtr, peak);
//	UART0_C2 &= ~UART0_C2_TCIE_MASK;
//    if(printingBlock == 0)
//    {
//	  UART0_C2 &= ~UART0_C2_TIE_MASK;
//    }
  }

//  if(UART_RxReady())
//  {
//	UART_Receive(&dataByte);
//	characterHistogram[dataByte] += 1;
//	newDataSemaphore = 1;
//  }

//  uint32_t pending;
//  pending = NVIC_GetPendingIRQ(UART0_IRQn);
//  pending = pending;
//  NVIC_ClearPendingIRQ(UART0_IRQn);

  return;
}




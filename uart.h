/*
 * uart.h
 *
 *  Created on: Nov 22, 2018
 *      Author: Sean
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\fifo.h"

void UART_Init(void);
void UART_BlockOutChar(uint8_t dataByte);
void UART_BlockInChar(uint8_t* dataByte);

//extern FIFO_TYPE* UARTRxFifoPtr;
extern FIFO_TYPE* UARTTxFifoPtr;
extern uint8_t newDataSemaphore;
extern uint64_t characterHistogram[256];

#endif /* INC_UART_H_ */

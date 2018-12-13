/************************************************************************
 *
 * File: main.c
 *
 * Author: Sean Duffy
 * Tools: 
 * Leveraged Code: See "#include"s
 * Links: NA
 *
***********************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\uart.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\fifo.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\adc.h"
#include "board.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\dma.h"

extern uint8_t printingBlock;

void MAIN_PutFifoDigits(FIFO_TYPE* fifo, uint64_t value)
{
  if(value >= 10)
  {
    MAIN_PutFifoDigits(UARTTxFifoPtr, value/10);
    value = value%10;
  }

  FIFO_Put(UARTTxFifoPtr, '0' + value);
}

void MAIN_UARTOutDigits(uint16_t value)
{
  if(value >= 10)
  {
    MAIN_PutFifoDigits(UARTTxFifoPtr, value/10);
    value = value%10;
  }

  FIFO_Put(UARTTxFifoPtr, '0' + value);
}



// 6. A main() function that uses the uart blocking
// functions to echo received characters one
// at a time back to the sender
#define BUFFER_PERIOD (100)
#define DECAY_RATE (1)
#define ALPHA (0.5)
#define ALPHA_DENOMINATOR (1000)
#define ALPHA_NUMERATOR (uint32_t)((ALPHA)*(ALPHA_DENOMINATOR))


uint32_t peak;
uint16_t samplesSincePeak;
int main(void)
{
//  uint8_t receivedCharacter;
  UART_Init();
  ADC_Init();
  DMA_Init((uint16_t*)ADC0_RA, (uint16_t*)doubleBuffer.firstBuffer);

  newDataSemaphore = 0;
  samplesSincePeak = 0;
//  uint16_t result = 0;
  while(1)
  {
	// Blocking Implementation - Echo Characters
//	  UART_BlockInChar(&receivedCharacter);
//    UART_BlockOutChar(receivedCharacter);
    if(newBufferReady)
    {
    	for(uint8_t i = 0; i <32; i +=1)
    	{
    		samplesSincePeak += 1;
    		if( DMA_Processing_Buffer[i] >= peak)
    		{
    			samplesSincePeak = 0;
    			peak = DMA_Processing_Buffer[i];
    		}

    		if(samplesSincePeak >=32)
    		{
    			samplesSincePeak = 0;
    			peak = (peak*ALPHA_NUMERATOR)/ALPHA_DENOMINATOR;
    		}
    	}
    }






//    for(uint32_t i = 0; i < 500000; i += 1)
//    {
//      // waste time
//    }
//    result = ADC_Read();
//    if((result & 0x8000) == 0x8000)
//    {
//      FIFO_Put(UARTTxFifoPtr, '-');
//      result = 0x8000 - (result & ~0x8000);
//    }
//    MAIN_PutFifoDigits(UARTTxFifoPtr, (uint64_t)result);
//    FIFO_Put(UARTTxFifoPtr, 0x0A); // New Line
//    FIFO_Put(UARTTxFifoPtr, 0x0D); // Carriage Return
//    UART0_C2 |= UART0_C2_TIE_MASK;


    // Non-Blocking Implementation
//	if(newDataSemaphore == 1)
//	{
//        newDataSemaphore = 0;
//        printingBlock = 1;
//        for(uint16_t i = '0'; i < 256; i+=1)
//        {
//          if(characterHistogram[i] != 0)
//          {
//        	FIFO_Put(UARTTxFifoPtr, i);
//        	FIFO_Put(UARTTxFifoPtr, '-');
//        	MAIN_PutFifoDigits(UARTTxFifoPtr, characterHistogram[i]);
//        	UART0_C2 |= UART0_C2_TIE_MASK;
//        	FIFO_Put(UARTTxFifoPtr, 0x0A); // New Line
//        	FIFO_Put(UARTTxFifoPtr, 0x0D); // Carriage Return
//          }
//        }
//        FIFO_Put(UARTTxFifoPtr, '-');
//        FIFO_Put(UARTTxFifoPtr, '-');
//        FIFO_Put(UARTTxFifoPtr, '-');
//        FIFO_Put(UARTTxFifoPtr, '-');
//        FIFO_Put(UARTTxFifoPtr, '-');
//    	FIFO_Put(UARTTxFifoPtr, 0x0A); // New Line
//    	FIFO_Put(UARTTxFifoPtr, 0x0D); // Carriage Return
//    	printingBlock = 0;
//	}
//	else
//	{
//      // toggle GPIO pin
//	}
  }

	
	return 0;
}

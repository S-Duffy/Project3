/*
 * dma.c
 *
 *  Created on: Dec 6, 2018
 *      Author: Sean
 */

#include "stdint.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\dma.h"
#include "board.h"


uint8_t newBufferReady;
uint16_t* DMA_Populating_Buffer;
uint16_t* DMA_Processing_Buffer;

DOUBLE_BUFFER doubleBuffer;

void DMA_Init(uint16_t* src, uint16_t* dest)
{
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

  DMAMUX0_CHCFG0 = 0x00;

  DMA_SAR0 = (uint32_t) src;
  DMA_DAR0 = (uint32_t) dest;
  DMA_DCR0 &= ~DMA_DCR_D_REQ_MASK;
  DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
  DMA_DCR0 &= ~DMA_DCR_SSIZE_MASK;
  DMA_DCR0 |= DMA_DCR_SSIZE(2); // 16-Bit source
  DMA_DCR0 &= ~DMA_DCR_DSIZE_MASK;
  DMA_DCR0 |= DMA_DCR_DSIZE(2); // 16-Bit destination
  DMA_DCR0 &= ~DMA_DCR_DMOD_MASK;
  DMA_DCR0 |= DMA_DCR_DMOD(3);  // 64 byte destination buffer
  DMA_DCR0 |= DMA_DCR_CS_MASK;
  DMA_DCR0 |= DMA_DCR_DINC_MASK;

  // For part 4
  // enable interrupt when half way through
  DMA_DCR0 |= DMA_DCR_EINT_MASK;

  newBufferReady = 0;

  DMA_Populating_Buffer = (uint16_t*) dest;

  DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;

  // Start DMA
  DMA_DCR0 |= DMA_DCR_START_MASK;

}

void DMA0_IRQHandler(void)
{
  DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

  uint16_t* temp;
  temp = DMA_Processing_Buffer;
  DMA_Processing_Buffer = DMA_Populating_Buffer;
  DMA_Populating_Buffer = temp;
  newBufferReady = 1;
}



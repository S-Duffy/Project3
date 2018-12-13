/*
 * dma.h
 *
 *  Created on: Dec 6, 2018
 *      Author: Sean
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

void DMA_Init(uint16_t* src, uint16_t* dest);

typedef struct
{
  uint16_t firstBuffer[32];
  uint16_t secondBuffer[32];
} DOUBLE_BUFFER;

extern DOUBLE_BUFFER doubleBuffer;
extern uint8_t newBufferReady;
extern uint16_t* DMA_Populating_Buffer;
extern uint16_t* DMA_Processing_Buffer;

#endif /* INC_DMA_H_ */

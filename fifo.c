/************************************************************************
 *
 * File: fifo.c
 *
 * Author: Sean Duffy
 * Tools: 
 * Leveraged Code: See "#include"s
 * Links: NA
 *
***********************************************************************/

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "../inc/fifo.h"

FIFO_TYPE* FIFO_Init(uint64_t sizeInBytes)
{
  if(sizeInBytes < 2)
  {
    return NULL;
  }
  // printf("sizeInBytes: %d\n", sizeInBytes);
  // printf("sizeof(FIFO_META_DATA_TYPE): %d\n", sizeof(FIFO_META_DATA_TYPE));
  FIFO_TYPE* newFifoPtr = malloc (sizeInBytes + sizeof(FIFO_META_DATA_TYPE));
  
  if(newFifoPtr == NULL)
  {
    return NULL;
  }
  newFifoPtr->metaData.putI = 0;
  newFifoPtr->metaData.getI = 0;
  newFifoPtr->metaData.sizeInBytes = sizeInBytes;
  
  return newFifoPtr;
}

FIFO_RESULT FIFO_Put(FIFO_TYPE* fifo, uint8_t dataByte)
{
  if((fifo->metaData.putI + 1) == (fifo->metaData.getI) ||
     ((fifo->metaData.putI == (fifo->metaData.sizeInBytes - 1)) && (fifo->metaData.getI == 0)))
  {
	// printf("here\n");
    return FIFO_RESULT_FIFO_FULL; 
  }
	
	// printf("aa\n");
  fifo->metaData.putI += 1;
  
  if(fifo->metaData.putI == fifo->metaData.sizeInBytes)
  {
	  // printf("here2\n");
    fifo->metaData.putI = 0;
  }
  // printf("bb\n");
  // printf("fifo->metaData.putI: %d\n", fifo->metaData.putI);
  fifo->data[fifo->metaData.putI] = dataByte;
  // printf("cc\n");
  return FIFO_RESULT_SUCCESS;
}

FIFO_RESULT FIFO_Get(FIFO_TYPE* fifo, uint8_t* dataByte)
{
  if((fifo->metaData.getI) == (fifo->metaData.putI))
  {
    return FIFO_RESULT_FIFO_EMPTY; 
  }
	
  fifo->metaData.getI += 1;
  
  if(fifo->metaData.getI == fifo->metaData.sizeInBytes)
  {
    fifo->metaData.getI = 0;
  }

  *dataByte = fifo->data[fifo->metaData.getI];
  
  return FIFO_RESULT_SUCCESS;
}

uint64_t FIFO_NumElements(FIFO_TYPE* fifo)
{
  if(fifo->metaData.putI >= fifo->metaData.getI)
  {
    return fifo->metaData.putI - fifo->metaData.getI;
  }
  else
  {
    return fifo->metaData.sizeInBytes - fifo->metaData.getI + fifo->metaData.putI;
  }
}
void FIFO_Destroy(FIFO_TYPE* fifo)
{
  free(fifo);
}

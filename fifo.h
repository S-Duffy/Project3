
#ifndef FIFO_H
#define FIFO_H
typedef struct {
  uint64_t putI;
  uint64_t getI;
  uint64_t sizeInBytes;
} FIFO_META_DATA_TYPE;

typedef struct {
  FIFO_META_DATA_TYPE metaData;
  uint8_t data[];
} FIFO_TYPE;

typedef enum {
  FIFO_RESULT_SUCCESS = 0,
  FIFO_RESULT_FIFO_FULL,
  FIFO_RESULT_FIFO_EMPTY,
} FIFO_RESULT;


FIFO_TYPE* FIFO_Init(uint64_t sizeInBytes);
FIFO_RESULT FIFO_Put(FIFO_TYPE* fifo, uint8_t dataByte);
FIFO_RESULT FIFO_Get(FIFO_TYPE* fifo, uint8_t* dataByte);
uint64_t FIFO_NumElements(FIFO_TYPE* fifo);
void FIFO_Destroy(FIFO_TYPE* fifo);

#endif // FIFO_H







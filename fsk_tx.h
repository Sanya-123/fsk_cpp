#ifndef FSK_TX_H
#define FSK_TX_H

#include "common_fsk.h"

//freq = -180 ... 180
MyComplex vco(float freq);

uint32_t getTx2FSK_size(uint32_t sizeData, uint16_t sps);
void tx2FSK(uint8_t *data, uint32_t sizeData, uint16_t sps, MyComplex *txData);

#endif // FSK_TX_H

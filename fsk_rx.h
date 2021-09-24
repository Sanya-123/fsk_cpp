#ifndef FSK_RX_H
#define FSK_RX_H

#include "common_fsk.h"

#define SAMPLES_BUFFER_RX_SIZE      16
#define DETECT_BIT_METOD            2   //0-2
/*
 * 0 - беру только средний отсчет
 * 1 - не учитываю крайние отсчеты
 * 2 - просумирую все отссчеты в посылке для принятия решения
 *
 */

//+-32768
float freqDetector(MyComplex in);
uint32_t rx2FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData);


#endif // FSK_RX_H

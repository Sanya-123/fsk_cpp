#ifndef FSK_RX_H
#define FSK_RX_H

#include "common_fsk.h"

//#define SAMPLES_BUFFER_RX_SIZE      16

//+-32768
float freqDetector(MyComplex in);

#if USE_VOLK
#include "volk/volk_complex.h"
typedef struct{
    lv_16sc_t * signal_i16;
    lv_32fc_t * signal;
    lv_32fc_t * signal_xConj;
    float * freq;
    uint32_t sizeSignal;
    lv_32fc_t oldData;
    float * power;
}FSK_RxStruct;

uint32_t rx2FSK(FSK_RxStruct *in, uint16_t sps, uint8_t *resData);
uint32_t rx4FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData);//TODO
#else
uint32_t rx2FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData);
uint32_t rx4FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData);
#endif


#endif // FSK_RX_H

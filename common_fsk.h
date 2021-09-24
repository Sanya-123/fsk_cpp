#ifndef COMMON_FSK_H
#define COMMON_FSK_H

#include <stdint.h>

typedef struct
{
    int64_t real;
    int64_t image;
}MyComplex;

typedef struct
{
    float real;
    float image;
}MyComplex_f;

typedef enum{
    _2FSK = 0,
    _4FSK = 1,
}Modulation;

//TODO add compensation to define for faster FFT
typedef enum{
    None,
    Compens,
    AddBit,
}CompensFP;

typedef enum{
    SyncTime,
    SyncCode,
    Resive,
}StateFreqRx;

#define FLOAT_COMPLEX_TO_INT_COMPLEX(f,i)       do{i.real = (int)f.real;i.image = (int)f.image;}while(0)
#define INT_COMPLEX_TO_FLOAT_COMPLEX(i,f)       do{f.real = (float)i.real;f.image = (float)i.image;}while(0)

//sync data is 4 sps
#define SYNC_FRAME_SIZE             32 /*1 byte*/
#define SYNC_CODE_SIZE              32 /*4 byte*/
#define ZEROS_DATA_SIZE             8
//extern const float syncFrame[];
#define SYN_TIME_DATA               0x55
#define SYN_TIME_DATA_SPS           0x0F0F0F0F
#define SYN_CODE                    0xAC8937BD

#define POROG_FIND_PREAMBLE         30/*6000000000*//*400000*/
#define POROG_DATA_END              3000//4000
#define DETECT_POWER                700000/*20000000*/
#define FULL_DETECT_PREAMBLE        1
#define POWER_MEAN_SIZE             16
#define DETECT_POWER_PREAMBLE       220000//200000   /*~320000*/

#define USE_FIXED_FRAME_SIZE        0
#define FIXED_FRAME_SIZE            1200  /*~1 WAKE frame with 1024 bytes data*/

#define TX_VCO_STEP                 90.0 /*Modulate freq step per symbols*/ /*optimal 60-90*/
#define MAX_SPS                     16

//4FSK
#define TX_VCO_4FSK_0               -90.0
#define TX_VCO_4FSK_1               -30.0
#define TX_VCO_4FSK_3               +30.0
#define TX_VCO_4FSK_2               +90.0
#define RX_POROG_RESHENIE           10000   /*значение куото определяет порог выбора бвайта*/


#endif // COMMON_FSK_H

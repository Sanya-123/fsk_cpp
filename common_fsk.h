#ifndef COMMON_FSK_H
#define COMMON_FSK_H

#include <stdint.h>

typedef struct
{
    int16_t real;
    int16_t image;
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
//синхро данные
#define SYN_TIME_DATA               0x55        /* первая преамбула испоьлзуться для временной синхронизации */
#define SYN_TIME_DATA_SPS           0x0F0F0F0F  /* тоже самое что и выше только с SPS 4 (она и отправляеться всегда не зависимо от sps) */
#define SYN_CODE                    0xAC8937BD  /* вторая преамбула (кодовая) ищиться как начало пакера, обрабатываеться вместе с обычными данными */

#define POROG_FIND_PREAMBLE         30 /* (Rx)минимальное количество бит которые дольжны совпасть во временной преамбуле */
#define POROG_DATA_END              6000//4000 /* (Rx) максимальная амплитуда частоты при которой считаеться что данные в пакете закончились */
#define DETECT_POWER                700000/*20000000*/ /* (Rx) максимальное значение мощность при котором данные не обрабатываються (учитываеться только в поиске временной преамбулы) */
#define FULL_DETECT_PREAMBLE        1 /* (Rx) уже не испоьлзуеться */
#define POWER_MEAN_SIZE             16 /* (Rx) коэффициент/размер буфера для расчета мощьности */
#define DETECT_POWER_PREAMBLE       220000//200000   /*~320000*/ /* (Rx) максимальное значение мощьности амплитуды частоты символа временной синххронизации (возможно можно обойтись без этого параметра) */

#define USE_FIXED_FRAME_SIZE        0 /* использовать пакеты с фиксированным размером */
#define FIXED_FRAME_SIZE            1200  /*~1 WAKE frame with 1024 bytes data*/ /* рахмер фиксированного пакета */

#define TX_VCO_STEP                 90.0 /*Modulate freq step per symbols*/ /*optimal 60-90*/ /* (Tx) аплитуда измернения фазы(частота=фазы/180*частота_дискретизации/2) за 1 отсчет */
#define MAX_SPS                     16 /* (Tx) максимально возможный SPS (защиты нигде нету просто учитываеться этот параметр в фубере кодовой синхронизации) */
//метод детектирования сомвола
#define DETECT_BIT_METOD            2   //0-2
/*
 * 0 - беру только средний отсчет
 * 1 - не учитываю крайние отсчеты
 * 2 - просумирую все отссчеты в посылке для принятия решения
 *
 */

//4FSK
/* (Tx) 4 параметра ниже эквивалентны TX_VCO_STEP */
#define TX_VCO_4FSK_0               -90.0
#define TX_VCO_4FSK_1               -30.0
#define TX_VCO_4FSK_3               +30.0
#define TX_VCO_4FSK_2               +90.0
#define RX_POROG_RESHENIE           10000   /* (Rx) значение куото определяет порог выбора бвайта */
#define POROG_DATA_END_4FSK         1000//4000 /* (Rx) эквивалент POROG_DATA_END */

//VOLK Vector-Optimized Library of Kernels
#define USE_VOLK                    1   //make sense if size > 1024
#define FAST_SYNC                   1


#endif // COMMON_FSK_H

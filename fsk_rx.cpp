#include "fsk_rx.h"
//#include <QtMath>
#include <QDebug>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif

float ApproxAtan(float z)
{
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;
    return (n1 + n2 * z * z) * z;
}

inline float fabsf(float a)
{
    return a > 0 ? a : -a;
}

float ApproxAtan2(float y, float x)
{
    if (x != 0.0f)
    {
        if (fabsf(x) > fabsf(y))
        {
            const float z = y / x;
            if (x > 0.0)
            {
                // atan2(y,x) = atan(y/x) if x > 0
                return ApproxAtan(z);
            }
            else if (y >= 0.0)
            {
                // atan2(y,x) = atan(y/x) + PI if x < 0, y >= 0
                return ApproxAtan(z) + M_PI;
            }
            else
            {
                // atan2(y,x) = atan(y/x) - PI if x < 0, y < 0
                return ApproxAtan(z) - M_PI;
            }
        }
        else // Use property atan(y/x) = PI/2 - atan(x/y) if |y/x| > 1.
        {
            const float z = x / y;
            if (y > 0.0)
            {
                // atan2(y,x) = PI/2 - atan(x/y) if |y/x| > 1, y > 0
                return -ApproxAtan(z) + M_PI_2;
            }
            else
            {
                // atan2(y,x) = -PI/2 - atan(x/y) if |y/x| > 1, y < 0
                return -ApproxAtan(z) - M_PI_2;
            }
        }
    }
    else
    {
        if (y > 0.0f) // x = 0, y > 0
        {
            return M_PI_2;
        }
        else if (y < 0.0f) // x = 0, y < 0
        {
            return -M_PI_2;
        }
    }
    return 0.0f; // x,y = 0. Could return NaN instead.
}

float _ApproxAtan2(float y, float x)
{
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;
    float result = 0.0f;
    if (x != 0.0f)
    {
        const union { float flVal; uint32_t nVal; } tYSign = { y };
        const union { float flVal; uint32_t nVal; } tXSign = { x };
        if (fabsf(x) >= fabsf(y))
        {
            union { float flVal; uint32_t nVal; } tOffset = { M_PI };
            // Add or subtract PI based on y's sign.
            tOffset.nVal |= tYSign.nVal & 0x80000000u;
            // No offset if x is positive, so multiply by 0 or based on x's sign.
            tOffset.nVal *= tXSign.nVal >> 31;
            result = tOffset.flVal;
            const float z = y / x;
            result += (n1 + n2 * z * z) * z;
        }
        else // Use atan(y/x) = pi/2 - atan(x/y) if |y/x| > 1.
        {
            union { float flVal; uint32_t nVal; } tOffset = { M_PI_2 };
            // Add or subtract PI/2 based on y's sign.
            tOffset.nVal |= tYSign.nVal & 0x80000000u;
            result = tOffset.flVal;
            const float z = x / y;
            result -= (n1 + n2 * z * z) * z;
        }
    }
    else if (y > 0.0f)
    {
        result = M_PI_2;
    }
    else if (y < 0.0f)
    {
        result = -M_PI_2;
    }
    return result;
}


float freqDetector(MyComplex in)
{
//    static float oldPhase = 0;//0 to 2PI

////    float Phase = qAtan2(in.image, in.real);
//    float Phase = _ApproxAtan2(in.image, in.real);
//    if(Phase < 0)
//        Phase = 2*M_PI + Phase;

//    float res = (Phase - oldPhase);
//    if((Phase < M_PI_2) && (oldPhase > 3*M_PI_2))//если произошол разрыв фазы в
//    {
//        res += 2*M_PI;
//    }
//    else if((Phase > 3*M_PI_2) && (oldPhase < M_PI_2))//если произошол разрыв фазы в другую сторону
//    {
//        res -= 2*M_PI;
//    }

//    oldPhase = Phase;

//    return res/M_PI*32768;

//    static MyComplex oldData = {0,0};
//    static float oldPhase = 0;//0 to 2PI

//    float res = qAtan2(in.image - oldData.image, in.real - oldData.real);

//    if((res < M_PI_2) && (oldPhase > 3*M_PI_2))//если произошол разрыв фазы в
//    {
//        res += 2*M_PI;
//    }
//    else if((res > 3*M_PI_2) && (oldPhase < M_PI_2))//если произошол разрыв фазы в другую сторону
//    {
//        res -= 2*M_PI;
//    }

//    float ress = res - oldPhase;

//    oldPhase = res;

//    oldData = in;


//    return (ress)/M_PI*32768;

    static MyComplex oldData = {0, 0};

    MyComplex tmp = {in.real*oldData.real + in.image*oldData.image,
                     in.image*oldData.real - in.real*oldData.image};

    float res = _ApproxAtan2(tmp.image, tmp.real);

    oldData = in;

    return res/M_PI*32768;
}

bool findFreamble(float *buffSync)
{
    uint8_t summ = 0;
    float power = 0;
    float header = 0;

    //проверяю что в начале 0 значения
    for(int j = 0; j < ZEROS_DATA_SIZE; j++)
    {
        header += buffSync[j];
    }
    header = header/ZEROS_DATA_SIZE;
    if(header > POROG_DATA_END)
        return false;

    for(uint32_t j = 0; j < SYNC_FRAME_SIZE; j++)
    {
//#if FULL_DETECT_PREAMBLE
//        summ += buffSync[j + ZEROS_DATA_SIZE] * syncFrame[j];
//#else

        summ += !((buffSync[j] > 0) & (SYN_TIME_DATA_SPS >> j));

#if DETECT_POWER_PREAMBLE > 0
        power += fabsf(buffSync[j]);
#endif
    }

#if DETECT_POWER_PREAMBLE == 0
    return summ > POROG_FIND_PREAMBLE;
#else
    return (summ > POROG_FIND_PREAMBLE) && (power > DETECT_POWER_PREAMBLE);
#endif
}

inline bool rxNFSK_SyncTime(float freq, float power)
{
    static uint32_t bytesBufferSync = 0;
    static float buffSync[SYNC_FRAME_SIZE + ZEROS_DATA_SIZE] = {0};


    //if buffer not full onli add to buff
    if(bytesBufferSync < (SYNC_FRAME_SIZE + ZEROS_DATA_SIZE))
    {
        buffSync[bytesBufferSync++] = freq;
    }
    else//else offset buffer
    {
        //offset buffer
        for(uint32_t j = 1; j < (SYNC_FRAME_SIZE + ZEROS_DATA_SIZE); j++)
            buffSync[j - 1] = buffSync[j];

        buffSync[(SYNC_FRAME_SIZE + ZEROS_DATA_SIZE) - 1] = freq;
    }

    if((bytesBufferSync == (SYNC_FRAME_SIZE + ZEROS_DATA_SIZE)) && (power > DETECT_POWER))
    {
        if(findFreamble(buffSync))
        {
            bytesBufferSync = 0;
            return true;
        }
    }

    return false;
}

#define CODE_SYNC_PATH      do { rechenie = tmpSumm > 0; /*если частота > 0 то значить передовалась 1*/ \
                                static uint32_t syncCoteRes = 0; \
                                syncCoteRes = (syncCoteRes >> 1) | (rechenie << 31); \
                                if(syncCoteRes == SYN_CODE) state = Resive;  \
                            } while(0);

uint32_t rx2FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData)
{
    static StateFreqRx state = SyncTime;
    static float val;
    uint32_t numButes = 0;

#if USE_FIXED_FRAME_SIZE
    static uint32_t _numButes = 0;
#endif

    for(uint32_t i = 0; i < sizeIn; i++)
    {
        val = freqDetector(in[i]);//detect freq

        if(state == SyncTime)
        {
            static float power = 0;
            power = (in[i].real*in[i].real + in[i].image*in[i].image);

            if(rxNFSK_SyncTime(val, power))
            {
                state = SyncCode;
#if USE_FIXED_FRAME_SIZE
                _numButes = 0;
#endif
            }
        }
        else
        {

            static uint16_t spsR = 0;
            static uint8_t rechenie;
            static uint8_t numBite = 0;
            static uint8_t byte = 0;
            static float tmpSumm;

            static float powerSymbols = 0;


#if DETECT_BIT_METOD == 0
            //беру только средний отсчет
            if(spsR == sps/2)
                tmpSumm = val;
#endif

#if DETECT_BIT_METOD == 1
            //не учитываю крайние отсчеты
            if(spsR == 1)
                tmpSumm = val;
            else if((spsR > 1) && (spsR < (sps - 1)))
                tmpSumm += val;
#endif

#if DETECT_BIT_METOD == 2
            //просумирую все отссчеты в посылке для принятия решения
            if(spsR == 0)
                tmpSumm = val;
            else
                tmpSumm += val;
#endif

            if(spsR == 0)
                powerSymbols = fabsf(val);
            else
                powerSymbols += fabsf(val);


            spsR++;

            if(spsR == sps)
            {
                spsR = 0;

                if((powerSymbols/sps) < POROG_DATA_END)//если принятое значения меньше какогото погрога то значить посылка закончилась
                {
                    state = SyncTime;
                    numBite = 0;
                    continue;
                }

                if(state == SyncCode)
                {
//                    rechenie = tmpSumm > 0;//если частота > 0 то значить передовалась 1
//                    static uint32_t syncCoteRes = 0;
//                    syncCoteRes = (syncCoteRes >> 1) | (rechenie << 31);

//                    if(syncCoteRes == SYN_CODE)
//                    {
//                        state = Resive;
//                    }
                    CODE_SYNC_PATH
                }
                else
                {
                    rechenie = tmpSumm > 0;//если частота > 0 то значить передовалась 1
                    byte |= rechenie << numBite;

                    numBite++;

                    if(numBite == 8)
                    {
                        resData[numButes++] = byte;
                        numBite = 0;
                        byte = 0;

#if USE_FIXED_FRAME_SIZE
                        _numButes++;

                        if(_numButes == FIXED_FRAME_SIZE)
                        {
                            state = Sync;
                            _numButes = 0;
                        }
#endif
                    }
                }
            }


        }
    }

    return numButes;
}

uint32_t rx4FSK(MyComplex *in, uint32_t sizeIn, uint16_t sps, uint8_t *resData)
{
    static StateFreqRx state = SyncTime;
    static float val;
    uint32_t numButes = 0;

#if USE_FIXED_FRAME_SIZE
    static uint32_t _numButes = 0;
#endif

    for(uint32_t i = 0; i < sizeIn; i++)
    {
        val = freqDetector(in[i]);//detect freq

        if(state == SyncTime)
        {
            static float power = 0;
            power = (in[i].real*in[i].real + in[i].image*in[i].image);

            if(rxNFSK_SyncTime(val, power))
            {
                state = SyncCode;
#if USE_FIXED_FRAME_SIZE
                _numButes = 0;
#endif
            }
        }
        else
        {

            static uint16_t spsR = 0;
            static uint8_t rechenie;
            static uint8_t numBite = 0;
            static uint8_t byte = 0;
            static float tmpSumm;

            static float powerSymbols = 0;


#if DETECT_BIT_METOD == 0
            //беру только средний отсчет
            if(spsR == sps/2)
                tmpSumm = val;
#endif

#if DETECT_BIT_METOD == 1
            //WARNING workc only with sps > 3
            //не учитываю крайние отсчеты
            if(spsR == 1)
                tmpSumm = val;
            else if((spsR > 1) && (spsR < (sps - 1)))
                tmpSumm += val;
#endif

#if DETECT_BIT_METOD == 2
            //просумирую все отссчеты в посылке для принятия решения
            if(spsR == 0)
                tmpSumm = val;
            else
                tmpSumm += val;
#endif

            if(spsR == 0)
                powerSymbols = fabsf(val);
            else
                powerSymbols += fabsf(val);


            spsR++;

            if(spsR == sps)
            {
                spsR = 0;

                if((powerSymbols/sps) < POROG_DATA_END)//если принятое значения меньше какогото погрога то значить посылка закончилась
                {
                    state = SyncTime;
                    numBite = 0;
                    continue;
                }

                if(state == SyncCode)
                {
                    CODE_SYNC_PATH;
                }
                else
                {
#if DETECT_BIT_METOD == 0
//                    tmpSumm = tmpSumm;
#elif DETECT_BIT_METOD == 1
                    tmpSumm = tmpSumm/(sps - 2);//WARNING workc only with sps > 3
#else
                    tmpSumm = tmpSumm/sps;
#endif
                    //принятие решение
                    //TODO
//                    static const uint8_t gray[4] = {0, 1, 3, 2};

//                    tmpSumm += 16375 + 5470;
//                    rechenie = tmpSumm/12000;
//                    rechenie = gray[rechenie];


                    rechenie = tmpSumm > 0 ? 2 : 0;
                    rechenie |= fabsf(tmpSumm) > RX_POROG_RESHENIE ? 0 : 1;

//                    if(tmpSumm > RX_POROG_RESHENIE)
//                        rechenie = 2;
//                    else if(tmpSumm > 0)
//                        rechenie = 3;
//                    else if(tmpSumm > (-RX_POROG_RESHENIE))
//                        rechenie = 1;
//                    else
//                        rechenie = 0;


                    byte |= rechenie << numBite;

                    numBite+=2;

                    if(numBite == 8)
                    {
                        resData[numButes++] = byte;
                        numBite = 0;
                        byte = 0;

#if USE_FIXED_FRAME_SIZE
                        _numButes++;

                        if(_numButes == FIXED_FRAME_SIZE)
                        {
                            state = Sync;
                            _numButes = 0;
                        }
#endif
                    }
                }
            }


        }
    }

    return numButes;
}

#undef CODE_SYNC_PATH

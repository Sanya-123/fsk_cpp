#include "fsk_tx.h"
#include <QtMath>


//randi([2 32], [1 32])*600
//const float syncFrame[] = {-17000, 17000, -17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, -17000, 17000, 17000, -17000, \
//                                    17000, -17000, 17000, 17000, 17000, 17000, 17000, -17000, 17000, -17000, 17000, 17000, 17000, -17000, 17000, \
//                                    -17000, 17000, -17000, 17000, 17000, 17000, -17000, -17000, 17000, 17000, 17000, 17000, -17000, 17000, -17000, \
//                                    17000, 17000, -17000, 17000, -17000, 17000, -17000, -17000, -17000, -17000, 17000, -17000, -17000, -17000, -17000, \
//                                    -17000, 17000, 17000, -17000};

//const float syncFrame[] = {17000, -17000, 17000, 17000, -17000, -17000, 17000, -17000, 17000, -17000, -17000, -17000, 17000, -17000, -17000, \
//                           17000, 17000, 17000, -17000, -17000, 17000, -17000, -17000, 17000, 17000, -17000, 17000, -17000, 17000, -17000, \
//                           -17000, 17000};

////barker x7
//const float syncFrame[] = {17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, 17000, \
//                           -17000, -17000, -17000, -17000, -17000, -17000, -17000, -17000, \
//                           17000, 17000, 17000, 17000, \
//                           -17000, -17000, -17000, -17000};

MyComplex vco(float freq)
{
    MyComplex res;
    static float phase = 0;
    phase += freq *M_PI/180;

    while(phase > M_PI)
    {
        phase -= M_PI*2;
    }
    while(phase < (-M_PI))
    {
        phase += M_PI*2;
    }

    res.real = qFastCos(phase)*32768;
    res.image = qFastSin(phase)*32768;

    return res;
}

uint32_t getTx2FSK_size(uint32_t sizeData, uint16_t sps)
{
#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    return (ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + SYNC_CODE_SIZE + FIXED_FRAME_SIZE*8*sps + ZEROS_DATA_SIZE)*frams;
#else
    return ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + (SYNC_CODE_SIZE*sps) + sizeData*8*sps + ZEROS_DATA_SIZE;
#endif
}

void tx2FSK(uint8_t *data, uint32_t sizeData, uint16_t sps, MyComplex *txData)
{
    const float tx1 = TX_VCO_STEP, tx0 = -TX_VCO_STEP;

    uint32_t numBytes = 0;
    uint32_t numBits = 0;
    float tx;

    uint32_t numFrame = 0;//если фиксированный размер кадра
    uint32_t oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + (SYNC_CODE_SIZE*sps) + sizeData*8*sps + ZEROS_DATA_SIZE;

    uint32_t iteration = 0;

    float syncFrameTime[SYNC_FRAME_SIZE];
    float syncFrameCOde[SYNC_CODE_SIZE*MAX_SPS];
    int tmp = 0;

    /**************************Some init data **************************/
    for(int i = 0; i < 32/*bites in sync time data*/; i++)
    {
        if((SYN_TIME_DATA_SPS >> i) & 0x01)
            tx = tx1;
        else
            tx = tx0;

        syncFrameTime[tmp++] = tx;
    }

    tmp = 0;
    for(int i = 0; i < 32/*bites in sync time data*/; i++)
    {
        if((SYN_CODE >> i) & 0x01)
            tx = tx1;
        else
            tx = tx0;

        for(int j = 0; j < sps; j++)
            syncFrameCOde[tmp++] = tx;
    }
    /****************************************************/



#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + FIXED_FRAME_SIZE*8*sps + ZEROS_DATA_SIZE;
    for(; numFrame < frams; numFrame++)
    {
#endif

    //заголовочный порог
    for(uint32_t i = 0; i < ZEROS_DATA_SIZE; i++)
        txData[iteration++] = vco(0);

//    //add preambles
//    for(uint32_t i = 0; i < SYNC_FRAME_SIZE; i++)
//        txData[iteration++] = vco(syncFrame[i]*180.0/32768);

    //add preambles time
    for(uint32_t i = 0; i < SYNC_FRAME_SIZE; i++)
        txData[iteration++] = vco(syncFrameTime[i]);

    //add zeross
    for(uint32_t i = 0; i < (2*sps); i++)
        txData[iteration++] = vco(tx0);

    //add preambles code
    for(uint32_t i = 0; i < (SYNC_CODE_SIZE*sps); i++)
        txData[iteration++] = vco(syncFrameCOde[i]);

#if USE_FIXED_FRAME_SIZE
    for(uint32_t i = 0; i < FIXED_FRAME_SIZE*8; i++)
#else
    for(uint32_t i = 0; i < sizeData*8; i++)
#endif
    {
#if USE_FIXED_FRAME_SIZE
        if(numBytes >= sizeData)
            tx = tx0;
        else if((data[numBytes] >> numBits) & 0x01)
            tx = tx1;
        else
            tx = tx0;
#else
        if((data[numBytes] >> numBits) & 0x01)
            tx = tx1;
        else
            tx = tx0;
#endif

        for(uint16_t j = 0; j < sps; j++)
        {
            txData[iteration++] = vco(tx);
        }

        numBits++;
        if(numBits == 8)
        {
            numBits = 0;
            numBytes++;
        }
    }

    //хвостовой порог
    for(uint32_t i = 0; i < ZEROS_DATA_SIZE; i++)
        txData[iteration++] = vco(0);

#if USE_FIXED_FRAME_SIZE
    }
#endif


}

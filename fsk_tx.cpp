#include "fsk_tx.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846  /* pi */
#endif
#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#endif


#define QT_SINE_TABLE_SIZE 256

const double _qt_sine_table[QT_SINE_TABLE_SIZE] = {
    double(0.0),
    double(0.024541228522912288),
    double(0.049067674327418015),
    double(0.073564563599667426),
    double(0.098017140329560604),
    double(0.1224106751992162),
    double(0.14673047445536175),
    double(0.17096188876030122),
    double(0.19509032201612825),
    double(0.2191012401568698),
    double(0.24298017990326387),
    double(0.26671275747489837),
    double(0.29028467725446233),
    double(0.31368174039889152),
    double(0.33688985339222005),
    double(0.35989503653498811),
    double(0.38268343236508978),
    double(0.40524131400498986),
    double(0.42755509343028208),
    double(0.44961132965460654),
    double(0.47139673682599764),
    double(0.49289819222978404),
    double(0.51410274419322166),
    double(0.53499761988709715),
    double(0.55557023301960218),
    double(0.57580819141784534),
    double(0.59569930449243336),
    double(0.61523159058062682),
    double(0.63439328416364549),
    double(0.65317284295377676),
    double(0.67155895484701833),
    double(0.68954054473706683),
    double(0.70710678118654746),
    double(0.72424708295146689),
    double(0.74095112535495911),
    double(0.75720884650648446),
    double(0.77301045336273699),
    double(0.78834642762660623),
    double(0.80320753148064483),
    double(0.81758481315158371),
    double(0.83146961230254524),
    double(0.84485356524970701),
    double(0.85772861000027212),
    double(0.87008699110871135),
    double(0.88192126434835494),
    double(0.89322430119551532),
    double(0.90398929312344334),
    double(0.91420975570353069),
    double(0.92387953251128674),
    double(0.93299279883473885),
    double(0.94154406518302081),
    double(0.94952818059303667),
    double(0.95694033573220894),
    double(0.96377606579543984),
    double(0.97003125319454397),
    double(0.97570213003852857),
    double(0.98078528040323043),
    double(0.98527764238894122),
    double(0.98917650996478101),
    double(0.99247953459870997),
    double(0.99518472667219682),
    double(0.99729045667869021),
    double(0.99879545620517241),
    double(0.99969881869620425),
    double(1.0),
    double(0.99969881869620425),
    double(0.99879545620517241),
    double(0.99729045667869021),
    double(0.99518472667219693),
    double(0.99247953459870997),
    double(0.98917650996478101),
    double(0.98527764238894122),
    double(0.98078528040323043),
    double(0.97570213003852857),
    double(0.97003125319454397),
    double(0.96377606579543984),
    double(0.95694033573220894),
    double(0.94952818059303667),
    double(0.94154406518302081),
    double(0.93299279883473885),
    double(0.92387953251128674),
    double(0.91420975570353069),
    double(0.90398929312344345),
    double(0.89322430119551521),
    double(0.88192126434835505),
    double(0.87008699110871146),
    double(0.85772861000027212),
    double(0.84485356524970723),
    double(0.83146961230254546),
    double(0.81758481315158371),
    double(0.80320753148064494),
    double(0.78834642762660634),
    double(0.7730104533627371),
    double(0.75720884650648468),
    double(0.74095112535495899),
    double(0.72424708295146689),
    double(0.70710678118654757),
    double(0.68954054473706705),
    double(0.67155895484701855),
    double(0.65317284295377664),
    double(0.63439328416364549),
    double(0.61523159058062693),
    double(0.59569930449243347),
    double(0.57580819141784545),
    double(0.55557023301960218),
    double(0.53499761988709715),
    double(0.51410274419322177),
    double(0.49289819222978415),
    double(0.47139673682599786),
    double(0.44961132965460687),
    double(0.42755509343028203),
    double(0.40524131400498992),
    double(0.38268343236508989),
    double(0.35989503653498833),
    double(0.33688985339222033),
    double(0.31368174039889141),
    double(0.29028467725446239),
    double(0.26671275747489848),
    double(0.24298017990326407),
    double(0.21910124015687005),
    double(0.19509032201612861),
    double(0.17096188876030122),
    double(0.1467304744553618),
    double(0.12241067519921635),
    double(0.098017140329560826),
    double(0.073564563599667732),
    double(0.049067674327417966),
    double(0.024541228522912326),
    double(0.0),
    double(-0.02454122852291208),
    double(-0.049067674327417724),
    double(-0.073564563599667496),
    double(-0.09801714032956059),
    double(-0.1224106751992161),
    double(-0.14673047445536158),
    double(-0.17096188876030097),
    double(-0.19509032201612836),
    double(-0.2191012401568698),
    double(-0.24298017990326382),
    double(-0.26671275747489825),
    double(-0.29028467725446211),
    double(-0.31368174039889118),
    double(-0.33688985339222011),
    double(-0.35989503653498811),
    double(-0.38268343236508967),
    double(-0.40524131400498969),
    double(-0.42755509343028181),
    double(-0.44961132965460665),
    double(-0.47139673682599764),
    double(-0.49289819222978393),
    double(-0.51410274419322155),
    double(-0.53499761988709693),
    double(-0.55557023301960196),
    double(-0.57580819141784534),
    double(-0.59569930449243325),
    double(-0.61523159058062671),
    double(-0.63439328416364527),
    double(-0.65317284295377653),
    double(-0.67155895484701844),
    double(-0.68954054473706683),
    double(-0.70710678118654746),
    double(-0.72424708295146678),
    double(-0.74095112535495888),
    double(-0.75720884650648423),
    double(-0.77301045336273666),
    double(-0.78834642762660589),
    double(-0.80320753148064505),
    double(-0.81758481315158382),
    double(-0.83146961230254524),
    double(-0.84485356524970701),
    double(-0.85772861000027201),
    double(-0.87008699110871135),
    double(-0.88192126434835494),
    double(-0.89322430119551521),
    double(-0.90398929312344312),
    double(-0.91420975570353047),
    double(-0.92387953251128652),
    double(-0.93299279883473896),
    double(-0.94154406518302081),
    double(-0.94952818059303667),
    double(-0.95694033573220882),
    double(-0.96377606579543984),
    double(-0.97003125319454397),
    double(-0.97570213003852846),
    double(-0.98078528040323032),
    double(-0.98527764238894111),
    double(-0.9891765099647809),
    double(-0.99247953459871008),
    double(-0.99518472667219693),
    double(-0.99729045667869021),
    double(-0.99879545620517241),
    double(-0.99969881869620425),
    double(-1.0),
    double(-0.99969881869620425),
    double(-0.99879545620517241),
    double(-0.99729045667869021),
    double(-0.99518472667219693),
    double(-0.99247953459871008),
    double(-0.9891765099647809),
    double(-0.98527764238894122),
    double(-0.98078528040323043),
    double(-0.97570213003852857),
    double(-0.97003125319454397),
    double(-0.96377606579543995),
    double(-0.95694033573220894),
    double(-0.94952818059303679),
    double(-0.94154406518302092),
    double(-0.93299279883473907),
    double(-0.92387953251128663),
    double(-0.91420975570353058),
    double(-0.90398929312344334),
    double(-0.89322430119551532),
    double(-0.88192126434835505),
    double(-0.87008699110871146),
    double(-0.85772861000027223),
    double(-0.84485356524970723),
    double(-0.83146961230254546),
    double(-0.81758481315158404),
    double(-0.80320753148064528),
    double(-0.78834642762660612),
    double(-0.77301045336273688),
    double(-0.75720884650648457),
    double(-0.74095112535495911),
    double(-0.724247082951467),
    double(-0.70710678118654768),
    double(-0.68954054473706716),
    double(-0.67155895484701866),
    double(-0.65317284295377709),
    double(-0.63439328416364593),
    double(-0.61523159058062737),
    double(-0.59569930449243325),
    double(-0.57580819141784523),
    double(-0.55557023301960218),
    double(-0.53499761988709726),
    double(-0.51410274419322188),
    double(-0.49289819222978426),
    double(-0.47139673682599792),
    double(-0.44961132965460698),
    double(-0.42755509343028253),
    double(-0.40524131400499042),
    double(-0.38268343236509039),
    double(-0.359895036534988),
    double(-0.33688985339222),
    double(-0.31368174039889152),
    double(-0.2902846772544625),
    double(-0.26671275747489859),
    double(-0.24298017990326418),
    double(-0.21910124015687016),
    double(-0.19509032201612872),
    double(-0.17096188876030177),
    double(-0.14673047445536239),
    double(-0.12241067519921603),
    double(-0.098017140329560506),
    double(-0.073564563599667412),
    double(-0.049067674327418091),
    double(-0.024541228522912448)
};

inline double _qFastSin(double x)
{
    int si = int(x * (0.5 * QT_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    double d = x - si * (2.0 * M_PI / QT_SINE_TABLE_SIZE);
    int ci = si + QT_SINE_TABLE_SIZE / 4;
    si &= QT_SINE_TABLE_SIZE - 1;
    ci &= QT_SINE_TABLE_SIZE - 1;
    return _qt_sine_table[si] + (_qt_sine_table[ci] - 0.5 * _qt_sine_table[si] * d) * d;
}

inline double _qFastCos(double x)
{
    int ci = int(x * (0.5 * QT_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    double d = x - ci * (2.0 * M_PI / QT_SINE_TABLE_SIZE);
    int si = ci + QT_SINE_TABLE_SIZE / 4;
    si &= QT_SINE_TABLE_SIZE - 1;
    ci &= QT_SINE_TABLE_SIZE - 1;
    return _qt_sine_table[si] - (_qt_sine_table[ci] + 0.5 * _qt_sine_table[si] * d) * d;
}

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

    res.real = _qFastCos(phase)*32767;
    res.image = _qFastSin(phase)*32767;

    return res;
}

const int16_t _sin_4dot[4] = {
    0,
    32767,
    0,
    -32767
};

const int16_t _cos_4dot[4] = {
    32767,
    0,
    -32767,
    0
};

MyComplex vco_optimize(float freq)
{
    static int8_t phase = 0;
    if(freq > 0)//+90
    {
        phase++;

        if(phase >=4)
            phase -= 4;
    }
    else//-90
    {
        phase--;
        if(phase < 0)
            phase += 4;
    }


    MyComplex res = {_cos_4dot[phase], _sin_4dot[phase]};

    return res;
}

uint32_t getTx2FSK_size(uint32_t sizeData, uint16_t sps)
{
#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    return (ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + SYNC_CODE_SIZE + FIXED_FRAME_SIZE*8*sps + ZEROS_DATA_SIZE)*frams;
#else
    return ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + 2*sps + (SYNC_CODE_SIZE*sps) + sizeData*8*sps + ZEROS_DATA_SIZE;
#endif
}

uint32_t getTx4FSK_size(uint32_t sizeData, uint16_t sps)
{
#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    return (ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + SYNC_CODE_SIZE + FIXED_FRAME_SIZE*4*sps + ZEROS_DATA_SIZE)*frams;
#else
    return ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + 2*sps + (SYNC_CODE_SIZE*sps) + sizeData*4*sps + ZEROS_DATA_SIZE;
#endif
}

float syncFrameTime[SYNC_FRAME_SIZE] = {TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, \
                                       -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, \
                                        TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, \
                                       -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, \
                                        TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, \
                                       -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, \
                                        TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, TX_VCO_STEP, \
                                       -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP, -TX_VCO_STEP};

inline void txNFSK_addHeader(uint16_t sps, MyComplex *txData, uint32_t *pointTx, int repeat = 1)
{
    const float tx1 = TX_VCO_STEP, tx0 = -TX_VCO_STEP;
    float tx;

//    float syncFrameTime[SYNC_FRAME_SIZE];
    float syncFrameCOde[SYNC_CODE_SIZE*MAX_SPS];
    int tmp = 0;


    /**************************Some init data **************************/
//    for(int i = 0; i < 32/*bites in sync time data*/; i++)
//    {
//        if((SYN_TIME_DATA_SPS >> i) & 0x01)
//            tx = tx1;
//        else
//            tx = tx0;

//        syncFrameTime[tmp++] = tx;
//    }

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

    //заголовочный порог
    for(uint32_t i = 0; i < ZEROS_DATA_SIZE; i++)
        txData[(*pointTx)+=repeat] = vco_optimize(0);

//    //add preambles
//    for(uint32_t i = 0; i < SYNC_FRAME_SIZE; i++)
//        txData[iteration++] = vco_optimize(syncFrame[i]*180.0/32768);

    //add preambles time
    for(uint32_t i = 0; i < SYNC_FRAME_SIZE; i++)
        txData[(*pointTx)+=repeat] = vco_optimize(syncFrameTime[i]);

    //add zeross
    for(uint32_t i = 0; i < (2*sps); i++)
        txData[(*pointTx)+=repeat] = vco_optimize(tx0);

    //add preambles code
    for(uint32_t i = 0; i < (SYNC_CODE_SIZE*sps); i++)
        txData[(*pointTx)+=repeat] = vco_optimize(syncFrameCOde[i]);
}

void tx2FSK(uint8_t *data, uint32_t sizeData, uint16_t sps, MyComplex *txData, int repeat)
{
    const float tx1 = TX_VCO_STEP, tx0 = -TX_VCO_STEP;

    uint32_t numBytes = 0;
    uint32_t numBits = 0;
    float tx;

#if USE_FIXED_FRAME_SIZE
    uint32_t numFrame = 0;//если фиксированный размер кадра
    uint32_t oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + (SYNC_CODE_SIZE*sps) + sizeData*8*sps + ZEROS_DATA_SIZE;
#endif

    uint32_t iteration = 0;


#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + FIXED_FRAME_SIZE*8*sps + ZEROS_DATA_SIZE;
    for(; numFrame < frams; numFrame++)
    {
#endif

       txNFSK_addHeader(sps, txData, &iteration, repeat);

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
        tx = (data[numBytes] >> numBits) & 0x01 ? tx1 : tx0;
//        if((data[numBytes] >> numBits) & 0x01)
//            tx = tx1;
//        else
//            tx = tx0;
#endif

        for(uint16_t j = 0; j < sps; j++)
        {
            txData[iteration+=repeat] = vco_optimize(tx);
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
        txData[iteration+=repeat] = vco_optimize(0);

#if USE_FIXED_FRAME_SIZE
    }
#endif
}

void tx4FSK(uint8_t *data, uint32_t sizeData, uint16_t sps, MyComplex *txData, int repeat)
{
    const float _4_tx[4] = {TX_VCO_4FSK_0, TX_VCO_4FSK_1, TX_VCO_4FSK_2, TX_VCO_4FSK_3};

    uint32_t numBytes = 0;
    uint32_t numBits = 0;
    float tx;

#if USE_FIXED_FRAME_SIZE
    uint32_t numFrame = 0;//если фиксированный размер кадра
    uint32_t oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + ZEROS_DATA_SIZE + (SYNC_CODE_SIZE*sps) + sizeData*4*sps + ZEROS_DATA_SIZE;
#endif

    uint32_t iteration = 0;




#if USE_FIXED_FRAME_SIZE
    //считаю количества дарров исходяч из размера данных (добию кадр на 1 больше если данных не кратно количетвам кадров)
    uint32_t frams = sizeData%FIXED_FRAME_SIZE == 0 ? sizeData/FIXED_FRAME_SIZE : sizeData/FIXED_FRAME_SIZE + 1;
    oneFrameSize = ZEROS_DATA_SIZE + SYNC_FRAME_SIZE + FIXED_FRAME_SIZE*4*sps + ZEROS_DATA_SIZE;
    for(; numFrame < frams; numFrame++)
    {
#endif

        txNFSK_addHeader(sps, txData, &iteration, repeat);

#if USE_FIXED_FRAME_SIZE
    for(uint32_t i = 0; i < FIXED_FRAME_SIZE*8; i++)
#else
    for(uint32_t i = 0; i < sizeData*4; i++)
#endif
    {
#if USE_FIXED_FRAME_SIZE
        if(numBytes >= sizeData)
            tx = tx0;
        else
            tx = _4_tx[(data[numBytes] >> numBits) & 0x03];
#else
        tx = _4_tx[(data[numBytes] >> numBits) & 0x03];
#endif

        for(uint16_t j = 0; j < sps; j++)
        {
            txData[iteration+=repeat] = vco(tx);
        }

        numBits+=2;
        if(numBits == 8)
        {
            numBits = 0;
            numBytes++;
        }
    }

    //хвостовой порог
    for(uint32_t i = 0; i < ZEROS_DATA_SIZE; i++)
        txData[iteration+=repeat] = vco(0);

#if USE_FIXED_FRAME_SIZE
    }
#endif

}

//wake

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
//#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <sys/signal.h>
#include <errno.h>
#include <stdint.h>


#define FRAME 4096      //максимальная длина пакета
//#define USE_ADDR    /*use wake with address*/
//wake
//Коды байт-стаффинга:

#define FEND  0xC0    //Frame END
#define FESC  0xDB    //Frame ESCape
#define TFEND 0xDC    //Transposed Frame END
#define TFESC 0xDD    //Transposed Frame ESCape

#define CRC_INIT 0xDE //начальное значение контрольной суммы
#define CRC_FEND 0x82 //начальное значение контрольной суммы с учетом FEND


uint8_t addresDev = 0x44;//адрес устройства


/*************Протокол WAKE****************/
//FEND | ADDR | MD | N | Data1  | ... | DataN | CRC8;
//ADDR[7] = 1, ADDR[6:0]->аддрес устройства
//CMD[7] = 0, CMD[6:0]->команда


static void Do_Crc8(uint8_t b, uint8_t *crc)//расчет контрольной суммы
{
#ifdef TABLE_CRC
  static __flash char CrcTable[256] =
  {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E,
    0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0,
    0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D,
    0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5,
    0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58,
    0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6,
    0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B,
    0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F,
    0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92,
    0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C,
    0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1,
    0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49,
    0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4,
    0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A,
    0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7,
    0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35
  };
  *crc = CrcTable[*crc ^ b]; //табличное вычисление
#else
  for(char i = 0; i < 8; b = b >> 1, i++) //вычисление в цикле
    if((b ^ *crc) & 1) *crc = ((*crc ^ 0x18) >> 1) | 0x80;
     else *crc = (*crc >> 1) & ~0x80;
#endif
}

int addDataTx(uint8_t *mas, uint8_t data, int num)
{
    int TxStuff = 0;
    if(data == FEND) //попытка передать FEND или FESC,
    {
        mas[num] = FESC;
        mas[num + 1] = TFEND;
        TxStuff = 1;
    }
    else if(data == FESC)
    {
        mas[num] = FESC;
        mas[num + 1] = TFESC;
        TxStuff = 1;
    }
    else
        mas[num] = data;
    return TxStuff;
}

uint16_t genPaccketWake(uint8_t cmd, uint8_t *data, uint16_t size, uint8_t *dataSend)//генерирование данных
{
    if(size > FRAME)
        return 0;
    uint8_t crc = CRC_FEND;//флаговые данные
    int iterationSend = 1;//итерация присваевания данных к пакету
    dataSend[0] = FEND;//присваевание

#ifdef USE_ADDR
    iterationSend += addDataTx(dataSend, addresDev | 0x80, iterationSend) + 1;//аддрес устройства
    Do_Crc8(addresDev | 0x80, &crc);
#endif
    iterationSend += addDataTx(dataSend, cmd & 0x7F, iterationSend) + 1;//команда
    Do_Crc8(cmd & 0x7F, &crc);
    iterationSend += addDataTx(dataSend, size & 0xFF, iterationSend) + 1;//команда//размер
    Do_Crc8(size & 0xFF, &crc);
    iterationSend += addDataTx(dataSend, (size >> 8) & 0xFF, iterationSend) + 1;//команда//размер
    Do_Crc8((size >> 8) & 0xFF, &crc);

    for(int i = 0; i < size; i++)
    {
        iterationSend += addDataTx(dataSend, data[i], iterationSend) + 1;//data
    }
    //crc

    for(int i = 0; i < size; i++)
    {
        Do_Crc8(data[i], &crc);
    }
    iterationSend += addDataTx(dataSend, crc, iterationSend) + 1;

    return iterationSend;
}

//write (fd, "hello!\n", 7);           // send 7 character greeting

//usleep ((7 + 25) * 100);             // sleep enough to transmit the 7 plus
//                                     // receive 25:  approx 100 uS per char transmit
//char buf [100];
//int n = read (fd, buf, sizeof buf);  // read up to 100 characters if ready to read
//********
#include <QDebug>

typedef enum
{
    WST_IDLE, //состояние ожидания
    WST_ADD,  //прием адреса
    WST_CMD,  //прием команды
    WST_LNG,  //прием длины пакета
    WST_LNG_2,//прием длины пакета
    WST_DATA, //прием/передача данных
    WST_CRC,  //прием/передача CRC
    WST_DONE  //состояние готовности
}RxState;//этап приема данных


int readData(uint8_t *data, uint16_t size, uint8_t *addr, uint8_t *cmmd, uint8_t *dtCome, uint16_t *dataLeft)
{
    static int dataRet = 0;
    static RxState state = WST_IDLE;
    static uint8_t adress = 0;
    static uint8_t cmd = 0;
    static uint32_t lng = 0;
    static uint8_t dataCome[FRAME] = {0};
    static uint8_t RxStuff = 0;
    static uint8_t crc;

    for(int i = 0; i < size; i++)
    {
//        if(state != WST_DONE)            //если прием разрешен
//        {
            if(data[i] == FEND)                 //обнаружен FEND (из любого состояния)
            {
                state = WST_ADD;             //переход к приему адреса
                RxStuff = 0;
                crc = CRC_FEND;
                qDebug("\nFEND");
                dataRet = 0;
                continue;           //нет дестаффинга
            }
            if(data[i] == FESC)                 //принят FESC,
            { RxStuff = 1; /*Do_Crc8(data[i], &crc);*/ /*qDebug("STUFFING\n");*/continue; }         //начало дестафинга
            if(RxStuff)                      //если идет дестафинг,
            {
                if(data[i] == TFESC)              //если принят TFESC,
                    data[i] = FESC;                 //замена его на FESC
                else if(data[i] == TFEND)         //если принят TFEND,
                    data[i] = FEND;                 //замена его на FEND
                else { state = WST_IDLE; /*qDebug("STUFFING ERROR\n");*/return -1; } //иначе ошибка стаффинга
//                Do_Crc8(data[i], &crc);
                RxStuff = 0;                   //дестаффинг закончен
            }
            switch(state)
            {
            case WST_ADD:                    //прием адреса
                state = WST_CMD;           //далее - прием команды
                if(data[i] & 0x80)              //если принят адрес,
                {//I am hear
                    data[i] &= ~0x80;             //восстановление значения адреса
                    adress = data[i];
                    Do_Crc8(data[i], &crc);
                    break;                     //переход к сохранению адреса
                }
                else
                    adress = 0;           //сохранение нулевого адреса
                //                break;
            case WST_CMD:                    //прием кода команды
                cmd = data[i];
                Do_Crc8(data[i], &crc);
                state = WST_LNG;           //далее - прием длины пакета
//                qDebug("cmd %d", cmd);
                break;                       //переход к сохранению команды
            case WST_LNG:                    //идет прием длины пакета
                state = WST_LNG_2;          //далее - прием 2 байт длины пакета
                lng = data[i]; //указатель на конец данных
                Do_Crc8(data[i], &crc);
                break;
            case WST_LNG_2:                    //идет прием длины пакета
                state = WST_DATA;          //далее - прием данных
                lng |= ((uint16_t)(data[i])) << 8; //указатель на конец данных
                if(lng > FRAME) lng = FRAME;   //ограничение длины пакета
                Do_Crc8(data[i], &crc);
                if(lng == 0)
                    state = WST_CRC;
                qDebug("lng %d", lng);
                break;
            case WST_DATA:                   //идет прием данных
                dataCome[dataRet] = data[i];
                dataRet++;
                Do_Crc8(data[i], &crc);
                if(dataRet == lng)           //если все данные и CRC приняты,
                {
                    qDebug() << "CRC";
                    state = WST_CRC;        //прием окончен
                }
                break;
            case WST_CRC:
//                *dataLeft = size - i - 1;
                if(data[i] != crc)
                {
                    state = WST_IDLE;
                    qDebug("Fail CRC\n");
                    return -1;
                }
                qDebug("CRC OK\n");
                *addr = adress;
                *cmmd = cmd;
                for(int i = 0; i < dataRet; i++)
                    dtCome[i] = dataCome[i];
                state = WST_IDLE;
                return dataRet;
                break;
            default: /*return -1*/;
            }
//        }
    }
    return 0;
}

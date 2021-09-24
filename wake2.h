#ifndef CONFIN_USILOK_RS485_LIB
#define CONFIN_USILOK_RS485_LIB

#include <stdint.h>
#include <stdbool.h>

uint16_t genPaccketWake(uint8_t cmd, uint8_t *data, uint16_t size, uint8_t *dataSend);
int readData(uint8_t *data, uint16_t size, uint8_t *addr, uint8_t *cmmd, uint8_t *dtCome, uint16_t *dataLeft);


#endif // CONFIN_USILOK_RS485_LIB


#ifndef __HC05_H__
#define __HC05_H__

#include "headfile.h"
#include "string.h"

#define BLEBUFF_MAX_SIZE 20

typedef enum {
    BLE_STANDBY,
    BLE_SERVO,
}BLE_SIGN;


extern BLE_SIGN ble_sign;
extern char hc05Buff[20];
extern uint8_t hc05BuffLen;

#endif /* __HC05_H__ */

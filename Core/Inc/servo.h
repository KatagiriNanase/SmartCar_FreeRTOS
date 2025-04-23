#ifndef __SERVO_H__
#define __SERVO_H__

#include  "headfile.h"

void Servo_Init(void);

/**
 *@brief 设置sg90舵机角度
 *
 * @param angle 要设置的角度（0~180）
 */
void Servo_SetAngle(uint8_t angle);

#endif /* __SERVO_H__ */


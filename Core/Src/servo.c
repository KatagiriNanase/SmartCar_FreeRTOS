#include "servo.h"

void Servo_SetAngle(uint8_t angle)
{
    if(angle>=0 && angle<=180)
    {
        uint8_t duty = (uint8_t)(1.0f / 9.0f * angle + 5);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty);
    }
    
}

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

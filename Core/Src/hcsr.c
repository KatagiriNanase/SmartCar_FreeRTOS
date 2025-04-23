#include "hcsr.h"


/* >=====================Variable definition=====================< */




/* >=====================Function definition=====================< */

float Hcsr_DetectDis(void)
{
    uint32_t startTick;
    float hcsrDistance;
    TRIG_HIGH;
    delay_us(50);
    TRIG_LOW;
    // 使用系统tick作为超时判断
    startTick = uwTick;
    /* 进入临界区，错过超声波信息 */
    while (ECHO_READ == GPIO_PIN_RESET)
    {
        if (uwTick - startTick > 100)
        {
            hcsrDistance = 26.0f; //超时默认前方距离安全
            goto next_task;     // 跳出当前测量
        }
    }
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    HAL_TIM_Base_Start(&htim4);

    startTick = uwTick;
    while (ECHO_READ == GPIO_PIN_SET)
    {
        if ((uwTick - startTick) > 100) // 100ms超时
        {
            HAL_TIM_Base_Stop(&htim4);
            hcsrDistance = 26.0f; //超时默认前方距离安全
            goto next_task; // 跳出当前测量
        }
    }
    HAL_TIM_Base_Stop(&htim4);
    hcsrDistance = __HAL_TIM_GET_COUNTER(&htim4) / 58.0f;

next_task:
    return hcsrDistance;
}


/* >=====================Interrupt callback=====================< */


// _Bool hcsrFlag;
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     if (GPIO_Pin == HCSR_ECHO_Pin)
//     {
//         if (hcsrFlag == 0)
//         {
//             __HAL_TIM_SET_COUNTER(&htim4, 0);
//             HAL_TIM_Base_Start(&htim4);
//             hcsrFlag = 1;

//             /* 改成下降沿触发 */
//             EXTI->RTSR &= ~EXTI_RTSR_RT10;
//             EXTI->FTSR |= EXTI_FTSR_FT10;

//         }
//         else if (hcsrFlag == 1)
//         {
//             HAL_TIM_Base_Stop(&htim4);
//             uint16_t tmpDistance = (__HAL_TIM_GET_COUNTER(&htim4) / 58.0f);
//             if (tmpDistance >= 800)
//                 hcsrDistance = 0;
//             else
//                 hcsrDistance = tmpDistance;
//             /* 前方遇到障碍物，标志位置1 */
//             if (hcsrDistance <= 20)   HcsrObstacle = 1;
//             /* 前方物障碍物，标志位置0 */
//             else if (hcsrDistance > 20)  HcsrObstacle = 0;
//             /* 改成上升沿触发 */
//             EXTI->FTSR &= ~EXTI_RTSR_RT10;
//             EXTI->RTSR |= EXTI_FTSR_FT10;

//             hcsrFlag = 0;
//         }

//     }
// }

#include "headfile.h"
#include "freertosdemo.h"

#define TRIG_HIGH (HAL_GPIO_WritePin(HCSR_TRIG_GPIO_Port,HCSR_TRIG_Pin,GPIO_PIN_SET));
#define TRIG_LOW (HAL_GPIO_WritePin(HCSR_TRIG_GPIO_Port,HCSR_TRIG_Pin,GPIO_PIN_RESET));
#define ECHO_READ (HAL_GPIO_ReadPin(HCSR_ECHO_GPIO_Port,HCSR_ECHO_Pin))


float Hcsr_DetectDis(void);


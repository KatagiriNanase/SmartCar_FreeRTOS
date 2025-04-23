#include "headfile.h"
#include "motor.h"

typedef uint16_t SensorType;

/*---- 传感器宏定义 ----*/
/* 从左往右数 */
#define SENSOR1 (GPIO_PIN_13)       //1
#define SENSOR2 (GPIO_PIN_12)       //2
#define SENSOR3 (GPIO_PIN_14)       //3
#define SENSOR4 (GPIO_PIN_15)       //4
#define SENSOR_PORT (GPIOB)
/* 读取传感器状态 */
#define READ_SENSOR(GPIO_PIN)  (HAL_GPIO_ReadPin(SENSOR_PORT,GPIO_PIN) )

/*--- global variale ---*/
extern _Bool infraredEN;


/*---Funciton Declaration  */
void Infrared_Start(void);

void Infrared_Stop(void);

uint8_t Infrared_proc(void);

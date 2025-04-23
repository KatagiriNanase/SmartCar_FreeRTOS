#include "infrared.h"

/*======== Trace任务 ========*/
TaskHandle_t trace_task_handle;
void TraceTask(void* pvParameters)
{
    while (1)
    {
        if (READ_SENSOR(SENSOR1))
        {
            SmartCar_SetState(CAR_LARGE_LEFT);
#ifdef INFRARED_DEBUG
            printf("1\r\n");
#endif // DEBUG

        }
        else if (READ_SENSOR(SENSOR4))
        {
            SmartCar_SetState(CAR_LARGE_RIGHT);
#ifdef INFRARED_DEBUG
            printf("4\r\n");
#endif // DEBUG
        }
        else if(READ_SENSOR(SENSOR2))
        {
            SmartCar_SetState(CAR_LEFT);
#ifdef INFRARED_DEBUG
            printf("2\r\n");
#endif // DEBUG
        }
        else if (READ_SENSOR(SENSOR3))
        {
            SmartCar_SetState(CAR_RIGHT);
#ifdef INFRARED_DEBUG
            printf("3\r\n");
#endif // DEBUG
        }
        else
        {
            SmartCar_SetState(CAR_FORWARD);
#ifdef INFRARED_DEBUG
            printf("0\r\n");
#endif // DEBUG
        }
        vTaskDelay(250);
    }
        
    
}




void Infrared_Start(void)
{
    infraredEN = 1;
}

void Infrared_Stop(void)
{
    infraredEN = 0;
}

_Bool infraredEN;       //红外循序使能

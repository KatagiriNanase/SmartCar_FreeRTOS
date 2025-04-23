#include "SmartCar.h"

/* 模式消息队列EV */
extern QueueHandle_t mode_queue_handle;

/* 智能车模式全局变量 */
SMARTCAR_MODE uwSmartCar_Mode = MODE_ORDINARY;      //初始为正常模式

/* 智能车控制任务配置 */

TaskHandle_t smartcar_task_handle;
void SmartCarTask(void* pvParameters);

/*======== SmartCar任务 ========*/
void SmartCarTask(void* pvParameters)
{

    FUNCTION_MODE current_mode;
    float tmpHcsrDis;
    while (1)
    {
#ifdef FREERTOS_DEBUG
        printf("执行SmartCar任务\r\n");
#endif // FREERTOS_DEBUG

        if (xQueuePeek(mode_queue_handle, &current_mode, 0) == pdFALSE)
            current_mode = MODE_STANDBY;
        switch (current_mode)
        {
            /*>>>>>>>>>> 待机模式 >>>>>>>>>>*/
        case MODE_STANDBY:
            SmartCar_SetState(CAR_BRAKE);
            break;

            /*>>>>>>>>>> 避障模式 >>>>>>>>>>*/
        case MODE_AVOID:
            switch (uwSmartCar_Mode)
            {
            case MODE_ORDINARY:
                /* 小车方向设置向前 */
                SmartCar_SetDire(DIRE_FORWARD);
                /* 小车设置前进走 */
                SmartCar_SetState(CAR_FORWARD);
                tmpHcsrDis = Hcsr_DetectDis();
                if (tmpHcsrDis < 25)
                    uwSmartCar_Mode = MODE_OBSTACLE;
                break;

            case MODE_OBSTACLE:
                SmartCar_SetState(CAR_BRAKE);
                /* 舵机左转 */
                Servo_SetAngle(160);
                vTaskDelay(700);
                tmpHcsrDis = Hcsr_DetectDis();
                if (tmpHcsrDis > 25 && uwSmartCar_Mode == MODE_OBSTACLE)
                {
                    /* 左前方无障碍物，切换为左转模式 */
                    Servo_SetAngle(90);
                    uwSmartCar_Mode = MODE_AVOID_LEFT;
                }
                else if (tmpHcsrDis < 25 && uwSmartCar_Mode == MODE_OBSTACLE)  //左前方有障碍物
                {
                    /* 探测右前方 */
                    Servo_SetAngle(20);
                    vTaskDelay(700);
                    tmpHcsrDis = Hcsr_DetectDis();
                }

                if (tmpHcsrDis > 25 && uwSmartCar_Mode == MODE_OBSTACLE)
                {
                    /* 右前方无障碍物，切换为右转模式 */
                    Servo_SetAngle(90);
                    vTaskDelay(700);
                    uwSmartCar_Mode = MODE_AVOID_RIGHT;
                }
                else if (tmpHcsrDis < 25 && uwSmartCar_Mode == MODE_OBSTACLE)
                {
                    /* 右前方也有障碍物，进行后退 */
                    Servo_SetAngle(90);
                    vTaskDelay(700);
                    uwSmartCar_Mode = MODE_AVOID_BACK;
                }
                break;

            case MODE_AVOID_LEFT:

                SmartCar_SetState(CAR_LARGE_LEFT);
                vTaskDelay(1300);
                uwSmartCar_Mode = MODE_ORDINARY;
                break;

            case MODE_AVOID_RIGHT:
                SmartCar_SetState(CAR_LARGE_RIGHT);
                vTaskDelay(1300);
                uwSmartCar_Mode = MODE_ORDINARY;
                break;

            case MODE_AVOID_BACK:
                SmartCar_SetDire(DIRE_BACK);
                SmartCar_SetState(CAR_FORWARD);
                vTaskDelay(2500);
                SmartCar_SetDire(DIRE_FORWARD);
                uwSmartCar_Mode = MODE_ORDINARY;
                break;
            default:
                break;
            }
            vTaskDelay(100);
            break;

            /*>>>>>>>>>> 循迹模式 >>>>>>>>>>*/
        case MODE_TRACE:
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
            else if (READ_SENSOR(SENSOR2))
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
            break;

        case MODE_FORWARD:
            if (Hcsr_DetectDis() < 25)
                SmartCar_SetState(CAR_BRAKE);
            else
                SmartCar_SetState(CAR_FORWARD);
            vTaskDelay(200);
            break;

        case MODE_LEFT:
            if (Hcsr_DetectDis() < 25)
                SmartCar_SetState(CAR_BRAKE);
            else
                SmartCar_SetState(CAR_LARGE_LEFT);
            vTaskDelay(200);
            break;

        case MODE_RIGHT:
            if (Hcsr_DetectDis() < 25)
                SmartCar_SetState(CAR_BRAKE);
            else
                SmartCar_SetState(CAR_LARGE_RIGHT);
            vTaskDelay(200);
            break;

        case MODE_BACK:
            SmartCar_SetDire(DIRE_BACK);
            SmartCar_SetState(CAR_FORWARD);
            vTaskDelay(200);
            SmartCar_SetDire(DIRE_FORWARD);
            break;
        }

    }

}



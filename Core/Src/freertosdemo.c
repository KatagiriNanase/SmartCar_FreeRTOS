#include "freertosdemo.h"

/* 启动任务配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void StartTask(void* pvParameters);

/* SmartCar控制任务配置 */
#define SMARTCAR_TASK_STACK 256
#define SMARTCAR_TASK_PRIORITY 4
extern TaskHandle_t smartcar_task_handle;
void SmartCarTask(void* pvParameters);

/* OLED任务配置 */
#define OLED_TASK_STACK 128
#define OLED_TASK_PRIORITY 3
extern TaskHandle_t oled_task_handle;
void OLEDTask(void* pvParameters);

/* 创建模式消息队列 */
QueueHandle_t mode_queue_handle;

void FreeRTOS_Start(void)
{
    /* 创建启动任务 */
    xTaskCreate((TaskFunction_t)StartTask,
        "StartTask",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        &start_task_handle);

    /* 启动调度器 */
    vTaskStartScheduler();
}

void StartTask(void* pvParameters)
{
    /* 进入临界区 */                /*由于我们当前任务优先级低，故进入临界区，
                                   *保证不会刚创建任务的时候就被切换走*/
    taskENTER_CRITICAL();

    /* 开启蓝牙透传 */
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, hc05Buff, BLEBUFF_MAX_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, hc05Buff, BLEBUFF_MAX_SIZE);

    /* 初始化舵机 */
    Servo_Init();
    Servo_SetAngle(90);

    /* 初始化电机 */
    Motor_Init();
    /* 小车速度设置为80% */
    Motor_SetSpeed(80);

    /* 初始化OLED，排版信息 */
    OLED_ShowInfo_Init();

    /* 创建模式消息队列 */
    mode_queue_handle = xQueueCreate(1, sizeof(FUNCTION_MODE));

    /* 创建OLED任务 */
#ifdef FREERTOS_DEBUG
    printf("创建OLED任务\r\n");
#endif // FREERTOS_DEBUG
    xTaskCreate((TaskFunction_t)OLEDTask,
        "OLEDTask",
        (configSTACK_DEPTH_TYPE)OLED_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)OLED_TASK_PRIORITY,
        &oled_task_handle);

    /* 创建智能车控制任务 */
#ifdef FREERTOS_DEBUG
    printf("创建SmartCar任务\r\n");
#endif // FREERTOS_DEBUG
    xTaskCreate((TaskFunction_t)SmartCarTask,
        "SmartCarTask",
        (configSTACK_DEPTH_TYPE)SMARTCAR_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)SMARTCAR_TASK_PRIORITY,
        &smartcar_task_handle);


    /* 删除任务自身 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    taskEXIT_CRITICAL();

}





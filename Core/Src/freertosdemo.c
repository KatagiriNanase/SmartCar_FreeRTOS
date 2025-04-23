#include "freertosdemo.h"

/* ������������ */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void StartTask(void* pvParameters);

/* SmartCar������������ */
#define SMARTCAR_TASK_STACK 256
#define SMARTCAR_TASK_PRIORITY 4
extern TaskHandle_t smartcar_task_handle;
void SmartCarTask(void* pvParameters);

/* OLED�������� */
#define OLED_TASK_STACK 128
#define OLED_TASK_PRIORITY 3
extern TaskHandle_t oled_task_handle;
void OLEDTask(void* pvParameters);

/* ����ģʽ��Ϣ���� */
QueueHandle_t mode_queue_handle;

void FreeRTOS_Start(void)
{
    /* ������������ */
    xTaskCreate((TaskFunction_t)StartTask,
        "StartTask",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        &start_task_handle);

    /* ���������� */
    vTaskStartScheduler();
}

void StartTask(void* pvParameters)
{
    /* �����ٽ��� */                /*�������ǵ�ǰ�������ȼ��ͣ��ʽ����ٽ�����
                                   *��֤����մ��������ʱ��ͱ��л���*/
    taskENTER_CRITICAL();

    /* ��������͸�� */
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, hc05Buff, BLEBUFF_MAX_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, hc05Buff, BLEBUFF_MAX_SIZE);

    /* ��ʼ����� */
    Servo_Init();
    Servo_SetAngle(90);

    /* ��ʼ����� */
    Motor_Init();
    /* С���ٶ�����Ϊ80% */
    Motor_SetSpeed(80);

    /* ��ʼ��OLED���Ű���Ϣ */
    OLED_ShowInfo_Init();

    /* ����ģʽ��Ϣ���� */
    mode_queue_handle = xQueueCreate(1, sizeof(FUNCTION_MODE));

    /* ����OLED���� */
#ifdef FREERTOS_DEBUG
    printf("����OLED����\r\n");
#endif // FREERTOS_DEBUG
    xTaskCreate((TaskFunction_t)OLEDTask,
        "OLEDTask",
        (configSTACK_DEPTH_TYPE)OLED_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)OLED_TASK_PRIORITY,
        &oled_task_handle);

    /* �������ܳ��������� */
#ifdef FREERTOS_DEBUG
    printf("����SmartCar����\r\n");
#endif // FREERTOS_DEBUG
    xTaskCreate((TaskFunction_t)SmartCarTask,
        "SmartCarTask",
        (configSTACK_DEPTH_TYPE)SMARTCAR_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)SMARTCAR_TASK_PRIORITY,
        &smartcar_task_handle);


    /* ɾ���������� */
    vTaskDelete(NULL);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();

}





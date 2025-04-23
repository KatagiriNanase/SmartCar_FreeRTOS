#include "user_oled.h"

TaskHandle_t oled_task_handle;
extern QueueHandle_t mode_queue_handle;
uint8_t oledBUff[7];
/*======== OLED任务 ========*/
void OLEDTask(void* pvParameters)
{
    FUNCTION_MODE current_mode;
    /* 300ms更新一次信息 */
    while (1)
    {

#ifdef FREERTOS_DEBUG
        printf("执行OLED任务\r\n");
#endif // FREERTOS_DEBUG
        if (xQueuePeek(mode_queue_handle, &current_mode, 0) == pdFALSE)
            current_mode = MODE_STANDBY;
        if (current_mode == MODE_STANDBY)
            sprintf((char*)oledBUff, "standby");
        else if (current_mode == MODE_AVOID)
            sprintf((char*)oledBUff, "avoid  ");
        else if (current_mode == MODE_TRACE)
            sprintf((char*)oledBUff, "trace  ");

        OLED_ShowString(48, 4, (char*)oledBUff, 16, 0);
        vTaskDelay(200);
    }
}

void OLED_ShowInfo_Init(void)
{
    /* 初始化I2C */
    HAL_I2C_Init(&hi2c1);

    /* 初始化OLED */
    OLED_Init();
    OLED_Clear();

    /* OLED起始信息 */
    OLED_ShowString(0, 0, "VSCODE_HIME", 16, 1);

    /*-- ECHO信息排版 --*/
    // OLED_ShowString(0, 2, "ECHO:", 16, 0);          //ECHO信息位置
    // OLED_ShowString(56, 2, "cm", 16, 0);            //cm信息位置

    /*-- 小车状态信息排版 --*/
    OLED_ShowString(0, 4, "State:", 16, 0);
    //OLED_ShowNum(40, 2, uwHcsrDistance, 2, 16, 0);
}

#include "hc05.h"


/* >=====================Interrupt callback=====================< */
char hc05Buff[BLEBUFF_MAX_SIZE];
uint8_t hc05BuffLen;
extern QueueHandle_t mode_queue_handle;
FUNCTION_MODE current_mode = MODE_STANDBY;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{

    /* 蓝牙接收到消息 */

    hc05BuffLen = Size;
    printf("%.*s\n", Size, hc05Buff);

    if (strstr((char*)hc05Buff, "AT+AVOIDEN"))
        current_mode = MODE_AVOID;
    else if (strstr((char*)hc05Buff, "AT+TRACEEN"))
        current_mode = MODE_TRACE;
    else if (strstr((char*)hc05Buff, "AT+LEFT"))
        current_mode = MODE_LEFT;
    else if (strstr((char*)hc05Buff, "AT+RIGHT"))
        current_mode = MODE_RIGHT;
    else if (strstr((char*)hc05Buff, "AT+FORWARD"))
        current_mode = MODE_FORWARD;
    else if (strstr((char*)hc05Buff, "AT+BACK"))
        current_mode = MODE_BACK;
    else
        current_mode = MODE_STANDBY;
    xQueueOverwriteFromISR(mode_queue_handle, &current_mode, pdFALSE);
    if (huart->Instance == huart1.Instance)
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)hc05Buff, BLEBUFF_MAX_SIZE);
    else
        HAL_UARTEx_ReceiveToIdle_IT(&huart2, hc05Buff, BLEBUFF_MAX_SIZE);



}

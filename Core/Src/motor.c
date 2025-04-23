#include "motor.h"
/* >=====================Function declaration=====================< */
/* ---内部使用函数，改变状态集成在SmartCar_SetState--- */
void Motor_RunForward(void);
void Motor_Brake(void);
void Motor_LeftTurn(void);
void Motor_RightTurn(void);
void Motor_LargeRightTurn(void);
void Motor_LargeLeftTurn(void);

/**
 * @brief 计算全局速度改变后应写入CCR的值
 *
 * @param speed 改变后的速率
 * @param channel_arr  一个指针，指向存放两个通道占空比值的数组
 */
void CalSpeedPercent(int8_t speed, uint8_t* channel_arr);
/* END---内部使用函数，改变状态集成在SmartCar_SetState---END */





/* >=====================Variable definition=====================< */

/*-- 小车状态 --*/
CAR_STATE uwCar_State = CAR_STOP; //默认为停止状态
/*-- 小车速度 --*/
uint8_t uwCarSpeedPercent = 50;          //0~100%
/*-- 小车方向 --*/
uint8_t uwCarDire = 3;       //默认车辆朝前





/* >=====================Function definition=====================< */
void Motor_Init(void)
{
    /* 设置初始状态为停止 */
    SmartCar_SetState(uwCar_State);

    /* 开启pwm输出 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_Delay(10);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

}

void SmartCar_SetDire(CAR_DIRE car_dire)
{
    if (car_dire != uwCarDire)
    {
        uwCarDire = car_dire;
        switch (uwCarDire)
        {
        case DIRE_FORWARD:
            /* AIN2,BIN2高电平 */
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);

            /* AIN1,BIN1低电平 */
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
            break;

        case DIRE_BACK:
            /* AIN2,BIN2低电平 */
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);

            /* AIN1,BIN1高电平 */
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
        }

    }
}


void SmartCar_SetState(CAR_STATE car_state)
{
    /* 如果欲设置状态不等于当前小车状态 */
    if (car_state != uwCar_State)
    {
        uwCar_State = car_state;
        switch (uwCar_State)
        {
        case CAR_BRAKE:
            Motor_Brake();
            break;
        case CAR_FORWARD:
            Motor_RunForward();
            break;
        case CAR_LEFT:
            Motor_LeftTurn();
            break;
        case CAR_RIGHT:
            Motor_RightTurn();
            break;
        case CAR_LARGE_LEFT:
            Motor_LargeLeftTurn();
            break;
        case CAR_LARGE_RIGHT:
            Motor_LargeRightTurn();
            break;
        default:
            break;
        }

    }
}

void Motor_SetSpeed(int8_t speed)
{
    if (speed >= 0 && speed <= 100)
    {
        if (speed != uwCarSpeedPercent)
        {
            uwCarSpeedPercent = speed;
            uint8_t channel_arr[2];
            /* 获取当前变化后的占空比值 */
            CalSpeedPercent(speed, channel_arr);
            for (uint8_t i = 0;i < 1;++i)
            {
                if (channel_arr[i] > 100)
                    channel_arr[i] = 100;
                else if (channel_arr[i] <= 0)
                    channel_arr[i] = 0;
            }
            /* 设置占空比 */
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, channel_arr[0]);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, channel_arr[1]);
        }

    }
}


void Motor_RunForward(void)
{
    int8_t tmp_speed_diff = uwCarSpeedPercent - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint8_t)(CAR_BASE_SPEED * change_percent));
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (uint8_t)(CAR_BASE_SPEED * change_percent));

}


void Motor_LeftTurn()
{
    int8_t tmp_speed_diff = uwCarSpeedPercent - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    /* 左轮速度慢 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint8_t)(CAR_LEFTTURN_LEFTWHEEL * change_percent));
    /* 右轮速度快 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (uint8_t)(CAR_LEFTTURN_RIGHTTWHEEL * change_percent));
}

void Motor_RightTurn()
{
    int8_t tmp_speed_diff = uwCarSpeedPercent - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    /* 左轮速度快 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint8_t)(CAR_RIGHTTURN_LEFTWHEEL * change_percent));
    /* 右轮速度慢 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (uint8_t)(CAR_RIGHTTURN_RIGHTTWHEEL * change_percent));
}

void Motor_LargeLeftTurn()
{
    int8_t tmp_speed_diff = uwCarSpeedPercent - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    /* 左轮停止 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
    /* 右轮速度快 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (uint8_t)(CAR_LEFTTURN_RIGHTTWHEEL * change_percent));
}

void Motor_LargeRightTurn()
{
    int8_t tmp_speed_diff = uwCarSpeedPercent - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    /* 左轮速度快 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint8_t)(CAR_RIGHTTURN_LEFTWHEEL * change_percent));
    /* 右轮停止 */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
}



void Motor_Brake()
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
}

void CalSpeedPercent(int8_t speed, uint8_t* channel_arr)
{
    int8_t tmp_speed_diff = speed - CAR_BASE_SPEED;
    /* 获取变化百分比 */
    float change_percent = (100.0f + tmp_speed_diff) / 100.0f;

    /* 计算通道一二变化后的值 */
    channel_arr[0] = (uint8_t)(__HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_1) * change_percent);
    channel_arr[0] = (uint8_t)(__HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_2) * change_percent);

}

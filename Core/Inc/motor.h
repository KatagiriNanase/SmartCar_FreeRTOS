/**
 *@file motor.h
 * @author shicao
 * @brief   电机采用百分比控速，默认初始速度为50%的占空比
 * @date 2025-04-04
 *
 *
 */
 //#include "headfile.h"

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "headfile.h"

 /*---- 小车基准速度 ----*/
#define CAR_BASE_SPEED 50      
/* >> 左转速度 << */
#define CAR_LEFTTURN_LEFTWHEEL 25       
#define CAR_LEFTTURN_RIGHTTWHEEL 50
/* >> 右转速度 << */
#define CAR_RIGHTTURN_LEFTWHEEL 50
#define CAR_RIGHTTURN_RIGHTTWHEEL 25

 /*---- 小车状态枚举 ----*/
typedef enum {
    CAR_FORWARD,    //前进
    CAR_LEFT,       //左转
    CAR_LARGE_LEFT, //大左转
    CAR_LARGE_RIGHT,//大右转
    CAR_RIGHT,      //右转
    CAR_STOP,       //停止
    CAR_BRAKE,       //刹车
}CAR_STATE;

/*---- 小车方向枚举 ----*/
typedef enum {
    DIRE_FORWARD,
    DIRE_BACK,
}CAR_DIRE;

/*---- 小车全局变量 ----*/
extern CAR_STATE uwCar_State;              //默认为停止状态
extern uint8_t car_speed_percent;         //0~100%

void Motor_Init(void);

/**
 * @brief 设置小车速度
 *
 * @param speed 只能在0~100内
 */
void Motor_SetSpeed(int8_t speed);

/**
 * @brief 设置小车状态
 *
 */
void SmartCar_SetState(CAR_STATE car_state);

/**
 * @brief 设置方向状态
 *
 */
void SmartCar_SetDire(CAR_DIRE car_dire);

#endif /* __MOTOR_H__ */

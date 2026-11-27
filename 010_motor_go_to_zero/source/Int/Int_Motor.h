#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__
#include "tim.h"
#include "gpio.h"
#include "stdio.h"
// 电机方向
typedef enum
{
    MOTOR_DIR1, // 数字0表示电机方向
    HAND_DIR2,  // 数字1表示手轮方向

} MOTOR_DIR_T;

// 电机距离
typedef struct
{
    double target_step;  // 目标步数
    double current_step; // 当前步数
} MOTOR_DISTANCE_T;

// 定义电机状态
typedef enum
{
    // 电机初始化状态
    MOTOR_INIT,
    // 电机运行状态
    MOTOR_RUN,
    // 电机停止状态
    MOTOR_STOP,
} MOTOR_STATE_T;

// 定义一个结构体用于存储电机相关参数:方向、距离、速度
typedef struct
{
    MOTOR_DIR_T motor_dir;           // 存储电机方向的成员
    MOTOR_DISTANCE_T motor_distance; // 存储电机距离的成员
    MOTOR_STATE_T motor_state;       // 存储电机状态
} MOTOR_PARAMS_T;

// 1.初始化方法
void Int_Motor_Init(void);

// 2.控制电机启动方法
void Int_Motor_Start(MOTOR_DIR_T dir, double distance_turn, double speed);

// 3.控制电机停止方法
void Int_Motor_Stop(void);

// 4.电机归零方法
void Int_Motor_Reset(void);

//5.获取电机状态
MOTOR_STATE_T Int_Motor_GetState(void);

#endif /* __INT_MOTOR_H__ */

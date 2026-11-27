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

// 定义一个结构体用于存储电机相关参数:方向、距离、速度
typedef struct
{
    MOTOR_DIR_T motor_dir;           // 存储电机方向的成员
    MOTOR_DISTANCE_T motor_distance; // 存储电机距离的成员
} MOTOR_PARAMS_T;

// 1.初始化方法
void Int_Motor_Init(void);

// 2.控制电机启动方法
void Int_Motor_Start(MOTOR_DIR_T dir, double distance_mm);

// 3.控制电机停止方法
void Int_Motor_Stop(void);

#endif /* __INT_MOTOR_H__ */

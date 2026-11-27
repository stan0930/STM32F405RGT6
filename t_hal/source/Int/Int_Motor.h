#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__
#ifndef INT_MOTOR_H
#define INT_MOTOR_H
#include "tim.h"
#include "stdio.h"
#include "gpio.h"
#include "usart.h"
#endif

//电机方向
typedef enum
{
    MOTOR_DIR1, //数字0表示向电机方向
    HAND_DIR2,  //数字1表示向手轮方向
}MOTOR_DIR_T;

//电机距离
typedef struct
{
    double target_step; //目标步数
    double current_step;//当前步数
    double acc_step;   //加速阶段距离
    double dec_step;   //减速阶段距离
    double constant_step; //恒速阶段距离
}MOTOR_DISTANCE_T;

//储存电机速度
typedef struct
{
    double target_speed; //目标速度，最大速度
    double current_speed;//电机当前速度
    double min_speed;    //最小速度
    double acc;          //加速度
    double dec;          //减速度
}MOTOR_SPEED_T;

//电机状态
typedef enum
{
    //电机初始化状态
    MOTOR_INIT,
    //电机运行状态
    MOTOR_RUN,
    //电机停止状态
    MOTOR_STOP,
}MOTOR_STATE_T;

//定义一个结构体用于储存电机相关参数：方向，距离，速度
typedef struct
{
    MOTOR_DIR_T motor_dir; //电机方向
    MOTOR_DISTANCE_T motor_distance; //电机距离
    MOTOR_SPEED_T motor_speed; //电机速度
    MOTOR_STATE_T motor_state; //电机状态
}MOTOR_PARAMS_T;

//1.电机初始化
void Int_Motor_Init(void);

//2.控制电机启动
void Int_Motor_Start(MOTOR_DIR_T dir);

//3.控制电机停止
void Int_Motor_Stop(void);

//4.电机归零
void Int_Motor_Reset(void);

//5.获取电机状态
MOTOR_STATE_T Int_Motor_GetState(void);

//6.获取电机当前速度
double Int_Motor_GetSpeed(void);

#endif /* __INT_MOTOR_H__ */

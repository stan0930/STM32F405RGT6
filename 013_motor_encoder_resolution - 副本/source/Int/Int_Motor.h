#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__
#include "tim.h"
#include "gpio.h"
#include "stdio.h"
//1.初始化方法
void Int_Motor_Init(void);

//2.控制电机启动方法
void Int_Motor_Start(void);

//3.控制电机停止方法
void Int_Motor_Stop(void);

#endif /* __INT_MOTOR_H__ */

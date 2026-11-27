#ifndef __INT_ENCODER_H__
#define __INT_ENCODER_H__
#include "gpio.h"
#include "tim.h"
#include "Dri_I2C.h"
#include "stdio.h"

// 1.编码器初始化
void Int_Encoder_Init(void);

// 2.编码器开启方法
void Int_Encoder_Start(void);

// 3.编码器关闭方法
void Int_Encoder_Stop(void);

// 4.编码器提供一个方法,用于让用户知道当前电机行走多远! 4096 - >一圈->8mm->1600步
uint32_t Int_Encoder_GetNumber(void);

#endif /* __INT_ENCODER_H__ */

#ifndef __INT_ENCODER_H__
#define __INT_ENCODER_H__
#include "gpio.h"
#include "tim.h"

// 1.编码器初始化
void Int_Encoder_Init(void);

// 2.编码器开启方法
void Int_Encoder_Start(void);

// 3.编码器关闭方法
void Int_Encoder_Stop(void);

#endif /* __INT_ENCODER_H__ */
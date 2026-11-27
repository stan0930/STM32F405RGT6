#include "Int_Encoder.h"

// 1.编码器初始化
void Int_Encoder_Init(void)
{

    // 1.TXS104EPWR电压转换芯片,使能开关打开!
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_SET);

    // 2.设置编码器芯片MT6701,工作模式:拉低,ABZ模式,这三根线,给主控信息！ 拉高,主控与编码器I2C通信【AB】
    HAL_GPIO_WritePin(ENCODER_MODE_GPIO_Port, ENCODER_MODE_Pin, GPIO_PIN_RESET);
}

// 2.编码器开启方法
void Int_Encoder_Start(void)
{
    // 开启主控定时器1,CNT计数,就能知道电机行走实际距离
    HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
}

// 3.编码器关闭方法
void Int_Encoder_Stop(void)
{
    // 关闭主控定时器1,CNT计数,就能知道电机行走实际距离
    HAL_TIM_Encoder_Stop_IT(&htim1, TIM_CHANNEL_ALL);
}

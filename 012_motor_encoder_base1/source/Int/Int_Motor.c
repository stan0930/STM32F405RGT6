#include "Int_Motor.h"

// 全局变量:统计PWM个数,电机前进步数
static uint32_t PWM_Count;

void Int_Motor_Init(void)
{
}
// 2.控制电机启动方法
void Int_Motor_Start(void)
{
    printf("start\r\n");
    // 1.电机驱动芯片使能开关开启
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_SET);
    // 2.确定电机旋转的方向:正转->手轮  反转->电机方向
    HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
    // 3.开启定时器5通道1,输出1KHZ方波
    HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_1);
}

// 3.控制电机停止方法
void Int_Motor_Stop(void)
{
    // 关闭电机驱动芯片使能开关
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_RESET);
    // 关闭外设定时器
    HAL_TIM_PWM_Stop_IT(&htim5, TIM_CHANNEL_1);
    printf("end\r\n");
}

// 定时器中断:CNT与CCRX数值相等的时候会执行一次
// 中断服务程序本质,通知PWM个数,向前走了多少步!
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        PWM_Count++;
        if (PWM_Count == 1600)
        {
            Int_Motor_Stop();
        }
    }
}
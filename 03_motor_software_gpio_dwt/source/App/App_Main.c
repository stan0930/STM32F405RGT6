#include "App_Main.h"
void App_Main(void)
{

    // 初始化DWT
    // STM32芯片:M3、M4、M7系列都有这个定时器
    // STM32芯片:M0没有这玩意!
    Com_DWT_Init();

    // 1.电机驱动芯片使能开启
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_SET);
    // 2.确定电机的放线:1正传,0反转
    HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);

    // 3.主控需要提供PWM[1600],转一圈!
    // 电机工作频率是有要求的:0.5KHZ - 8KHZ
    // 1.CPU浪费
    // 2.HAL_Delay误差,不精准,而且在for循环误差了1ms
    printf("start\r\n");
    for (uint16_t i = 0; i < 1600; i++)
    {
        HAL_GPIO_WritePin(MOTOR_STEP_GPIO_Port, MOTOR_STEP_Pin, GPIO_PIN_SET);
        Com_DWT_delay_us(1000);
        HAL_GPIO_WritePin(MOTOR_STEP_GPIO_Port, MOTOR_STEP_Pin, GPIO_PIN_RESET);
        Com_DWT_delay_us(1000);
    }
    printf("end\r\n");

    // 4.关闭使能开关,电机停止了
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_RESET);
}

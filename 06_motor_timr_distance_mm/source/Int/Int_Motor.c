#include "Int_Motor.h"
// 结构体变量用于存储电机全部参数:方向、距离、速度.
MOTOR_PARAMS_T motor;
// 用于修改电机的方向的
static void Int_Motor_SetDir(MOTOR_DIR_T dir);
// 用于设置距离:将MM单位转换为电机行走的步数
static void Int_Motor_SetDistance_mm(double distance_mm);

void Int_Motor_Init(void)
{
}
// 2.控制电机启动方法
void Int_Motor_Start(MOTOR_DIR_T dir, double distance_mm)
{
    printf("start\r\n");
    // 1.电机驱动芯片使能开关开启
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_SET);
    // 2.确定电机旋转的方向:正转(高电平)->手轮  反转(低电平)->电机方向
    // 设置电机的方向
    Int_Motor_SetDir(dir);

    // 3.设置电机距离---MM单位
    Int_Motor_SetDistance_mm(distance_mm);
    // 4.开启定时器5通道1,输出1KHZ方波
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
        // 此中断服务程序:电机向前走一步,执行一次
        motor.motor_distance.current_step++;
        if (motor.motor_distance.current_step == motor.motor_distance.target_step)
        {
            // 电机停止
            Int_Motor_Stop();
        }
    }
}

// 用于修改电机的方向的方法
static void Int_Motor_SetDir(MOTOR_DIR_T dir)
{
    // 存储电机运动方向
    motor.motor_dir = dir;
    // 手轮方向
    if (motor.motor_dir == HAND_DIR2)
    {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
    }
    else
    {
        // 电机方向
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET);
    }
}

// 设置滑台运动距离-mm
// 滑台运动最大距离:240-260mm
static void Int_Motor_SetDistance_mm(double distance_mm)
{

    // 将距离单位mm转为电机行走的步数
    if (distance_mm >= 240)
    {
        // 约束滑台能运动最大距离
        distance_mm = 240;
    }
    motor.motor_distance.target_step = distance_mm / 8.0 * 1600;
}

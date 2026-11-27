#include "App_Main.h"
void App_Main(void)
{
    // 1.初始化
    Int_Motor_Init();
    // 2.启动电机
    // 设置电机方向,设置电机运动距离----MM
    // Int_Motor_Start(MOTOR_DIR1,24);
    // 设置电机方向,设置电机运动距离-角度
    Int_Motor_Start(MOTOR_DIR1, 360 * 3);
}

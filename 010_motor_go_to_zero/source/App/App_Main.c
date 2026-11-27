#include "App_Main.h"

void App_Main(void)
{
    // 1.初始化
    Int_Motor_Init();
    // 2.归零操作
    Int_Motor_Reset();
    printf("%d\r\n", (int)Int_Motor_GetState());
    // 等待归零结束以后在执行后面语句
    while (Int_Motor_GetState() != MOTOR_STOP);

    // 3.向电机方向行走10圈
    Int_Motor_Start(MOTOR_DIR1, 10, 1000);
}

#include "App_Main.h"


void App_Main_Init(void)
{   
    //1.初始化电机
    Int_Motor_Init();

    //启动电机
    Int_Motor_Start(MOTOR_DIR1,20,6000);
}





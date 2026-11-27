#include "Int_Motor.h"
#define BASE_SPEED 1000
//结构体变量用于存储电机全部参数：方向，距离，速度
static MOTOR_PARAMS_T motor;
//修改电机方向
static void Int_Motor_SetDir(MOTOR_DIR_T dir);
// 用于设置距离:将圈数单位转换为电机行走的步数
static void Int_Motor_SetDistance_turn(double distance_turn);
void Int_Motor_Init(void)
{
    //电机处于初始化状态
    motor.motor_state = MOTOR_INIT;

    //1.设置电机的初始速度
    motor.motor_speed.min_speed = BASE_SPEED;

    //当前速度为最小速度
    motor.motor_speed.current_speed = motor.motor_speed.min_speed;

    //加速度
    motor.motor_speed.acc = 2*BASE_SPEED;

    //减速度
    motor.motor_speed.dec = 2*BASE_SPEED;
}

//2.控制电机启动方法
void Int_Motor_Start(MOTOR_DIR_T dir,double distance_turn,double speed)
{
    //设置电机状态：运行状态
    motor.motor_state = MOTOR_RUN;

    printf("Int_Motor_Start\n");
    //1.电机驱动的使能开关
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port,MOTOR_EN_Pin,GPIO_PIN_SET);
    //2.确定电机旋转的方向：正转(高电平)->手轮 反转(低电平)->电机
    //设置电机方向
    Int_Motor_SetDir(dir);

    //3.设置电机运动距离
    Int_Motor_SetDistance_turn(distance_turn);

    //4.设置电机速度
    motor.motor_speed.target_speed = speed;
    //设置电机速度
    Int_Motor_SetSpeed();

}

static void Int_Motor_SetDir(MOTOR_DIR_T dir)
{
    //储存电机运动方向
    motor.motor_dir = dir;
    if(motor.motor_dir == HAND_DIR2)
    {   
        //手轮方向
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port,MOTOR_DIR_Pin,GPIO_PIN_SET);
    }
    else
    {   
        //电机方向
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port,MOTOR_DIR_Pin,GPIO_PIN_RESET);
    }
}

// 用于设置距离:将圈数单位转换为电机行走的步数
void Int_Motor_SetDistance_turn(double distance_turn){
    if(distance_turn > 30){
        distance_turn = 30;
    }

    motor.motor_distance.target_step = distance_turn*1600.0;
}
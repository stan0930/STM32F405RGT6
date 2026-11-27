#include "Int_Encoder.h"

// 更新中断标志位
uint8_t over_flow_count = 0;

// 编码器:I2C通信从机地址
#define ENCODER_I2C_ADDR 0x06
#define ENCODER_I2C_ADDR_WRITE (ENCODER_I2C_ADDR << 1)
#define ENCODER_I2C_ADDR_READ (ENCODER_I2C_ADDR << 1 | 0x01)

#define REG_L 0x31
#define REG_H 0x30
// 用于修改编码器分辨率的方法
static void Int_Encoder_SetResulution(uint16_t resolution);

// 1.编码器初始化
void Int_Encoder_Init(void)
{

    // 1.TXS104EPWR电压转换芯片,使能开关打开!
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_SET);

    // 2.通过I2C修改编码器分辨率 + 编码器处于I2C工作模式
    // 编码器AB引脚已经处于I2C工作模式
    HAL_GPIO_WritePin(ENCODER_MODE_GPIO_Port, ENCODER_MODE_Pin, GPIO_PIN_SET);

    // I2C通信,初始化
    Dri_I2C_Init();

    // 在设置分辨率之前:解决引脚冲突问题
    GPIO_InitTypeDef gpio = {0};
    /* PB1 controls TXS0104E OE, NOT the sensor's 5 V supply. */
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_RESET);
    HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
    HAL_GPIO_WritePin(ENCODER_MODE_GPIO_Port, ENCODER_MODE_Pin, GPIO_PIN_SET);
    /* Z is CSN in I2C mode: release it high instead of using an ABZ pulldown. */
    gpio.Pin = ENCODER_Z_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_Z_GPIO_Port, &gpio);
    __HAL_GPIO_EXTI_CLEAR_IT(ENCODER_Z_Pin);
    HAL_Delay(100);
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(600);

    // MCU与编码器进行I2C通信,修改编码器的分辨率
    Int_Encoder_SetResulution(0x3FF);

    // 3.设置编码器芯片MT6701,工作模式:拉低,ABZ模式,这三根线,给主控信息！ 拉高,主控与编码器I2C通信【AB】
    HAL_GPIO_WritePin(ENCODER_MODE_GPIO_Port, ENCODER_MODE_Pin, GPIO_PIN_RESET);
}

// 2.编码器开启方法
void Int_Encoder_Start(void)
{

    GPIO_InitTypeDef gpio = {0};
    HAL_TIM_Encoder_MspInit(&htim1);
    HAL_GPIO_WritePin(ENCODER_MODE_GPIO_Port, ENCODER_MODE_Pin, GPIO_PIN_RESET);
    gpio.Pin = ENCODER_Z_Pin;
    gpio.Mode = GPIO_MODE_IT_RISING;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ENCODER_Z_GPIO_Port, &gpio);
    __HAL_GPIO_EXTI_CLEAR_IT(ENCODER_Z_Pin);
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    // 开启主控定时器1,CNT计数,就能知道电机行走实际距离
    // 不能这样开启定时器编码器模式,因为开启中断不是咱么想要更新中断。而是捕获比较【源码】
    // HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
}

// 3.编码器关闭方法
void Int_Encoder_Stop(void)
{
    // 关闭主控定时器1,CNT计数,就能知道电机行走实际距离
    // HAL_TIM_Encoder_Stop_IT(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
}

// 用于修改编码器分辨率的方法
// resolution:11,11111111
static void Int_Encoder_SetResulution(uint16_t resolution)
{

    // 获取想要写入低八位数据
    uint8_t data_l = resolution & 0xFF;
    uint8_t data_h = (resolution >> 8) & 0xFF;
    // 1.第一步:写入寄存器
    //  1.对于高位寄存器(八位),咱们只是使用低两位,先读取,再修改(用的那两位),在写入!
    uint8_t reg_val = 0;
    Dri_I2C_ReadReg(ENCODER_I2C_ADDR, REG_H, &reg_val);

    // 循环判断
    for (uint8_t i = 0; i < 2; i++)
    {
        // 对应位置0
        if ((data_h & (1 << i)) == 0)
        {
            reg_val &= ~(1 << i);
        }
        else
        {
            // 对应的位置1
            reg_val |= (1 << i);
        }
    }
    // 写入寄存器
    Dri_I2C_WriteReg(ENCODER_I2C_ADDR, REG_L, data_l);
    Dri_I2C_WriteReg(ENCODER_I2C_ADDR, REG_H, reg_val);

    // 第二步:编程秘钥
    Dri_I2C_WriteReg(ENCODER_I2C_ADDR, 0x09, 0xB3);
    // 第三步:编程命令
    Dri_I2C_WriteReg(ENCODER_I2C_ADDR, 0x0A, 0x05);

    // 第四步:写入周期,等待600ms以上
    HAL_Delay(1000);

    // 第五步:验证分辨率是否写入成功
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(ENCODER_EN_GPIO_Port, ENCODER_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    // 查看写入分辨率是否正确
    uint8_t reg_val_l = 0;
    uint8_t reg_val_h = 0;
    Dri_I2C_ReadReg(ENCODER_I2C_ADDR, REG_L, &reg_val_l);
    Dri_I2C_ReadReg(ENCODER_I2C_ADDR, REG_H, &reg_val_h);

    printf("resolution:%d\r\n", ((reg_val_h & 0x03) << 8) | reg_val_l);
}


//4.获取编码器数值
uint32_t Int_Encoder_GetNumber(void)
{

    return over_flow_count * 65536 + __HAL_TIM_GET_COUNTER(&htim1);
}

// 定时器更新中断服务程序,当定时器XCNT计数溢出时候,会执行一次！
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    printf("over_flow_count");
    // 编码器使用定时器统计计数
    if (htim->Instance == TIM1)
    {
        over_flow_count++;
    }
}
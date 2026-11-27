#include "App_Main.h"
#include "Int_Encoder.h"

uint8_t turn = 0;

void App_Main(void)
{
    // 1.编码器初始化方法
    Int_Encoder_Init();
    // 2.开启编码器
    Int_Encoder_Start();

    printf("gaga\r\n");

    turn = 0;
    // 读取器CNT数值
    while (1)
    {
        printf("%d tuen = %d\r\n", __HAL_TIM_GetCounter(&htim1), turn);
        HAL_Delay(100);
    }
}

// ABZ项,对应Z这个引脚外部中断:电机转了一圈,编码器芯片会给脉冲,采集上升沿执行中断服务程序,表示电机转一圈!

//上电的IO会有波动,误触外部中断,但是电机根本没动!
//中断内部:尽可能不要书写太多逻辑,打标记！【实际工作:外包公司随便玩!】
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ENCODER_Z_Pin)
    {
        printf("hehe\r\n");
        turn++;
    }
}
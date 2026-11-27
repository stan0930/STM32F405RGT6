#include "App_Main.h"
#include "Com_DWT.h"
#include "Int_Encoder.h"
void App_Main(void)
{

    // 0.软件模拟的I2C底层通信依赖DWT定时器,进行延时
    Com_DWT_Init();

    // 1.编码器初始化方法
    Int_Encoder_Init();
    // 2.开启编码器
    Int_Encoder_Start();
    // 读取器CNT数值
    while (1)
    {
        printf("%d\r\n",Int_Encoder_GetNumber());
        HAL_Delay(100);
    }
}

// ABZ项,对应Z这个引脚外部中断:电机转了一圈,编码器芯片会给脉冲,采集上升沿执行中断服务程序,表示电机转一圈!

// 上电的IO会有波动,误触外部中断,但是电机根本没动!
// 中断内部:尽可能不要书写太多逻辑,打标记！【实际工作:外包公司随便玩!】
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    
}

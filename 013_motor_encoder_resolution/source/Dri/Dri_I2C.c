#include "Dri_I2C.h"
#include <stdio.h>
#define SDA_HIGH (I2C_GPIO_PORT->BSRR = I2C_SDA_PIN)
#define SDA_LOW  (I2C_GPIO_PORT->BSRR = (uint32_t)I2C_SDA_PIN << 16U)
#define SCL_HIGH (I2C_GPIO_PORT->BSRR = I2C_SCL_PIN)
#define SCL_LOW  (I2C_GPIO_PORT->BSRR = (uint32_t)I2C_SCL_PIN << 16U)
#define SDA_READ ((I2C_GPIO_PORT->IDR & I2C_SDA_PIN) != 0U)
#define SCL_READ ((I2C_GPIO_PORT->IDR & I2C_SCL_PIN) != 0U)
#define I2C_DELAY() Com_DWT_delay_us(5)

/* Wait for the actual pin, not just its output latch. Bounded to 100 us. */
static Dri_I2C_Status ClockHigh(void)
{
    uint32_t retry;
    SCL_HIGH;
    for (retry = 0; retry < 20U; ++retry) {
        if (SCL_READ) { I2C_DELAY(); return DRI_I2C_OK; }
        I2C_DELAY();
    }
    return DRI_I2C_CLOCK_TIMEOUT;
}
static Dri_I2C_Status Stop(void)
{
    Dri_I2C_Status status;
    SCL_LOW;
    SDA_LOW;
    I2C_DELAY();
    status = ClockHigh();
    SDA_HIGH;
    I2C_DELAY();
    return status;
}
void Dri_I2C_RecoverBus(void)
{
    uint8_t i;
    SDA_HIGH;
    I2C_DELAY();
    for(i=0;i<9;i++)
    {
        SCL_LOW;
        I2C_DELAY();
        SCL_HIGH;
        I2C_DELAY();
    }
    Stop();
}

void Dri_I2C_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Preload released levels before switching from TIM1 to open drain. */
    SDA_HIGH;
    SCL_HIGH;
    gpio.Pin = I2C_SDA_PIN | I2C_SCL_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_GPIO_PORT, &gpio);
    I2C_DELAY();

    printf("SDA_READ=%d SCL_READ=%d\r\n", SDA_READ, SCL_READ);
    Dri_I2C_RecoverBus(); //新增总线恢复
}
static Dri_I2C_Status Start(void)
{
    Dri_I2C_Status status;
    SDA_HIGH;
    I2C_DELAY();
    status = ClockHigh();
    if (status != DRI_I2C_OK) return status;
    if (!SDA_READ) return DRI_I2C_BUS_BUSY;
    SDA_LOW;
    I2C_DELAY();
    SCL_LOW;
    I2C_DELAY();
    return DRI_I2C_OK;
}

/* Each byte has exactly eight data clocks and ONE ACK clock. */
static Dri_I2C_Status SendByte(uint8_t data, Dri_I2C_Status nack)
{
    uint8_t i, acknowledged;
    Dri_I2C_Status status;
    for (i = 0; i < 8U; ++i) {
        if (data & 0x80U) { SDA_HIGH; } else { SDA_LOW; }
        I2C_DELAY(); /* SDA setup before SCL rises. */
        status = ClockHigh();
        if (status != DRI_I2C_OK) return status;
        SCL_LOW;
        I2C_DELAY();
        data <<= 1;
    }
    SDA_HIGH;
    I2C_DELAY();
    status = ClockHigh();
    if (status != DRI_I2C_OK) return status;
    acknowledged = !SDA_READ; /* Sample after the high-level delay. */
    SCL_LOW;
    I2C_DELAY();
    return acknowledged ? DRI_I2C_OK : nack;
}
static Dri_I2C_Status ReceiveByte(uint8_t *data)
{
    uint8_t i, value = 0;
    Dri_I2C_Status status;
    SDA_HIGH;
    I2C_DELAY();
    for (i = 0; i < 8U; ++i) {
        status = ClockHigh();
        if (status != DRI_I2C_OK) return status;
        value = (uint8_t)((value << 1) | (SDA_READ ? 1U : 0U));
        SCL_LOW;
        I2C_DELAY();
    }
    /* Single-byte read: master ends the transfer with NACK. */
    SDA_HIGH;
    I2C_DELAY();
    status = ClockHigh();
    SCL_LOW;
    I2C_DELAY();
    if (status == DRI_I2C_OK) *data = value;
    return status;
}
static Dri_I2C_Status Finish(Dri_I2C_Status status)
{
    Dri_I2C_Status stop_status = Stop();
    return status == DRI_I2C_OK ? stop_status : status;
}
Dri_I2C_Status Dri_I2C_Probe(uint8_t address)
{
    Dri_I2C_Status status;
    if (address > 0x7FU) return DRI_I2C_BAD_ARGUMENT;
    status = Start();
    if (status == DRI_I2C_OK)
        status = SendByte((uint8_t)(address << 1), DRI_I2C_NACK_WRITE_ADDRESS);
    return Finish(status);
}
Dri_I2C_Status Dri_I2C_ReadReg(uint8_t address, uint8_t reg, uint8_t *data)
{
    uint8_t value;
    Dri_I2C_Status status;
    if (address > 0x7FU || data == 0) return DRI_I2C_BAD_ARGUMENT;
    status = Start();
    if (status == DRI_I2C_OK)
        status = SendByte((uint8_t)(address << 1), DRI_I2C_NACK_WRITE_ADDRESS);
    if (status == DRI_I2C_OK)
        status = SendByte(reg, DRI_I2C_NACK_REGISTER);
    if (status == DRI_I2C_OK)
        status = Start(); /* Repeated START; SCL was left low. */
    if (status == DRI_I2C_OK)
        status = SendByte((uint8_t)((address << 1) | 1U), DRI_I2C_NACK_READ_ADDRESS);
    if (status == DRI_I2C_OK)
        status = ReceiveByte(&value);
    status = Finish(status);
    if (status == DRI_I2C_OK) *data = value; /* Error never masquerades as zero. */
    return status;
}
Dri_I2C_Status Dri_I2C_WriteReg(uint8_t address, uint8_t reg, uint8_t data)
{
    Dri_I2C_Status status;
    if (address > 0x7FU) return DRI_I2C_BAD_ARGUMENT;
    status = Start();
    if (status == DRI_I2C_OK)
        status = SendByte((uint8_t)(address << 1), DRI_I2C_NACK_WRITE_ADDRESS);
    if (status == DRI_I2C_OK)
        status = SendByte(reg, DRI_I2C_NACK_REGISTER);
    if (status == DRI_I2C_OK)
        status = SendByte(data, DRI_I2C_NACK_DATA);
    return Finish(status);
}
const char *Dri_I2C_StatusName(Dri_I2C_Status status)
{
    static const char *const names[] = {
        "OK", "BUS_BUSY", "CLOCK_TIMEOUT", "NACK_WRITE_ADDRESS",
        "NACK_REGISTER", "NACK_READ_ADDRESS", "NACK_DATA", "BAD_ARGUMENT"
    };
    return (unsigned)status < sizeof(names) / sizeof(names[0]) ? names[status] : "UNKNOWN";
}

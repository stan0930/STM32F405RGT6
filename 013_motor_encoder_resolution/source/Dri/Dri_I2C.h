#ifndef __DRI_I2C_H__
#define __DRI_I2C_H__
#include "main.h"
#include "Com_DWT.h"
/* Encoder 1: PA8 = SDA, PA9 = SCL. APIs take 7-bit addresses. */
#define I2C_SCL_PIN GPIO_PIN_9
#define I2C_SDA_PIN GPIO_PIN_8
#define I2C_GPIO_PORT GPIOA
typedef enum {
    DRI_I2C_OK = 0, DRI_I2C_BUS_BUSY, DRI_I2C_CLOCK_TIMEOUT,
    DRI_I2C_NACK_WRITE_ADDRESS, DRI_I2C_NACK_REGISTER,
    DRI_I2C_NACK_READ_ADDRESS, DRI_I2C_NACK_DATA, DRI_I2C_BAD_ARGUMENT
} Dri_I2C_Status;
void Dri_I2C_Init(void);
Dri_I2C_Status Dri_I2C_Probe(uint8_t address);
Dri_I2C_Status Dri_I2C_ReadReg(uint8_t address, uint8_t reg, uint8_t *data);
Dri_I2C_Status Dri_I2C_WriteReg(uint8_t address, uint8_t reg, uint8_t data);
const char *Dri_I2C_StatusName(Dri_I2C_Status status);
#endif

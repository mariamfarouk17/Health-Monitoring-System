/*
 * I2C_int.h
 *
 *  Created on: Sep 7, 2026
 *      Author: HP
 */

#ifndef MCAL_I2C_I2C_INT_H_
#define MCAL_I2C_I2C_INT_H_

#include "../../LIB/STD_TYPES.h"

#define I2C_OK                0U
#define I2C_ERR_TIMEOUT       1U
#define I2C_ERR_NACK          2U
#define I2C_ERR_NULL_PTR      3U

/* Enables GPIOB + I2C1 clocks, configures PB6(SCL)/PB7(SDA) as AF4 open-drain,
 * sets bus speed from I2C_cfg.h, enables the peripheral. Call once at startup. */
void MI2C1_vInit(void);

/* Low-level primitives */
u8 MI2C1_u8Start(void);
void MI2C1_vStop(void);
u8 MI2C1_u8SendAddress(u8 A_u8DeviceAddress, u8 A_u8Read);
u8 MI2C1_u8SendByte(u8 A_u8Data);

/* High-level register access for HAL drivers (MPU6050 / MAX30102) */
u8 MI2C1_u8WriteReg(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8 A_u8Data);
u8 MI2C1_u8ReadReg(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8* A_pu8Data);
u8 MI2C1_u8ReadBurst(u8 A_u8DeviceAddress, u8 A_u8RegAddress, u8* A_pu8Buffer, u16 A_u16Len);

#endif /* MCAL_I2C_I2C_INT_H_ */

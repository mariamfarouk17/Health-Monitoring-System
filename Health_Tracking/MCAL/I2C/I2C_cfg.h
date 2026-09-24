#ifndef MCAL_I2C_I2C_CFG_H_
#define MCAL_I2C_I2C_CFG_H_

/* Must match your APB1 clock frequency in MHz (Standard STM32F4 Black Pill is usually 16, 25, or 42 MHz) */
#define I2C_PCLK1_MHZ         16U

/* Bus Speed Modes */
#define I2C_MODE_STANDARD     100000U   /* 100 kHz */
#define I2C_MODE_FAST         400000U   /* 400 kHz (Recommended for MPU6050 Burst Reads) */

#define I2C_SPEED_HZ          I2C_MODE_STANDARD //I2C_MODE_FAST

#define I2C_OWN_ADDRESS       0x00U

#endif /* MCAL_I2C_I2C_CFG_H_ */

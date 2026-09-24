/*
 * I2C_MAX_cfg.h
 *
 * Created on: Sep 16, 2026
 * Author: HP
 */

#ifndef MCAL_I2C_MAX_I2C_MAX_CFG_H_
#define MCAL_I2C_MAX_I2C_MAX_CFG_H_


/* ============================================================
 * I2C CLOCK CONFIGURATION
 * ============================================================ */

/*
 * PCLK1 = 16 MHz
 *
 * This must match the actual APB1 clock.
 */
#define I2C_MAX_PCLK1_MHZ         16U


/* ============================================================
 * I2C SPEED
 * ============================================================ */

#define I2C_MAX_MODE_STANDARD     100000U
#define I2C_MAX_MODE_FAST         400000U

#define I2C_MAX_SPEED_HZ          I2C_MAX_MODE_STANDARD


/* ============================================================
 * OWN ADDRESS
 * ============================================================ */

#define I2C_MAX_OWN_ADDRESS       0x00U


#endif /* MCAL_I2C_MAX_I2C_MAX_CFG_H_ */

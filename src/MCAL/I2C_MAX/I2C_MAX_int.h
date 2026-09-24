/*
 * I2C_MAX_int.h
 *
 * Created on: Sep 16, 2026
 * Author: HP
 */

#ifndef MCAL_I2C_MAX_I2C_MAX_INT_H_
#define MCAL_I2C_MAX_I2C_MAX_INT_H_


/* ============================================================
 * I2C MAX STATUS
 * ============================================================ */

#define I2C_MAX_OK                 0U
#define I2C_MAX_ERR_NACK           1U
#define I2C_MAX_ERR_TIMEOUT        2U
#define I2C_MAX_ERR_NULL_PTR       3U


/* ============================================================
 * I2C2 Initialization
 * ============================================================ */

void MI2C2_MAX_vInit(void);


/* ============================================================
 * START / STOP
 * ============================================================ */

u8 MI2C2_MAX_u8Start(void);

void MI2C2_MAX_vStop(void);


/* ============================================================
 * Address
 *
 * A_u8DeviceAddress:
 *      7-bit slave address
 *
 * A_u8Read:
 *      0 = WRITE
 *      1 = READ
 * ============================================================ */

u8 MI2C2_MAX_u8SendAddress(
    u8 A_u8DeviceAddress,
    u8 A_u8Read
);


/* ============================================================
 * Send one byte
 * ============================================================ */

u8 MI2C2_MAX_u8SendByte(
    u8 A_u8Data
);


/* ============================================================
 * Write register
 *
 * Sequence:
 *
 * START
 * Slave + WRITE
 * Register
 * Data
 * STOP
 * ============================================================ */

u8 MI2C2_MAX_u8WriteReg(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 A_u8Data
);


/* ============================================================
 * Read one register
 *
 * Sequence:
 *
 * START
 * Slave + WRITE
 * Register
 * REPEATED START
 * Slave + READ
 * Data
 * STOP
 * ============================================================ */

u8 MI2C2_MAX_u8ReadReg(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 *A_pu8Data
);


/* ============================================================
 * Read burst
 *
 * Supports:
 *
 *      1 byte
 *      2 bytes
 *      3+ bytes
 *
 * Used by MAX30102 FIFO.
 * ============================================================ */

u8 MI2C2_MAX_u8ReadBurst(
    u8 A_u8DeviceAddress,
    u8 A_u8RegAddress,
    u8 *A_pu8Buffer,
    u16 A_u16Len
);


/* ============================================================
 * Debug
 * ============================================================ */

void MI2C2_MAX_vGetDebugRegisters(
    u32 *A_pu32SR1,
    u32 *A_pu32SR2,
    u32 *A_pu32CR1,
    u32 *A_pu32CR2
);


void MI2C2_MAX_vGetDebugRegistersAfterInit(
    u32 *A_pu32SR1,
    u32 *A_pu32SR2,
    u32 *A_pu32CR1,
    u32 *A_pu32CR2
);


#endif /* MCAL_I2C_MAX_I2C_MAX_INT_H_ */

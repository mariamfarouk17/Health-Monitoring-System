/*
 * I2C_MAX_prv.h
 *
 * Created on: Sep 7, 2026
 * Author: HP
 */

#ifndef MCAL_I2C_MAX_I2C_MAX_PRV_H_
#define MCAL_I2C_MAX_I2C_MAX_PRV_H_


/* ============================================================
 * I2C2 BASE ADDRESS
 * STM32F401
 * ============================================================ */

#define I2C2_MAX_BASE_ADDR        0x40005800UL


/* ============================================================
 * I2C REGISTER MAP
 * ============================================================ */

typedef struct
{
    volatile u32 CR1;       /* 0x00 */
    volatile u32 CR2;       /* 0x04 */
    volatile u32 OAR1;      /* 0x08 */
    volatile u32 OAR2;      /* 0x0C */
    volatile u32 DR;        /* 0x10 */
    volatile u32 SR1;       /* 0x14 */
    volatile u32 SR2;       /* 0x18 */
    volatile u32 CCR;       /* 0x1C */
    volatile u32 TRISE;     /* 0x20 */
    volatile u32 FLTR;      /* 0x24 */

} I2C_MAX_MemMap_t;


/* ============================================================
 * I2C2 INSTANCE
 * ============================================================ */

#define I2C2_MAX \
    ((volatile I2C_MAX_MemMap_t *)(I2C2_MAX_BASE_ADDR))


/* ============================================================
 * CR1 REGISTER BITS
 * ============================================================ */

#define I2C_MAX_CR1_PE            0U
#define I2C_MAX_CR1_START         8U
#define I2C_MAX_CR1_STOP          9U
#define I2C_MAX_CR1_ACK           10U
#define I2C_MAX_CR1_SWRST         15U


/* ============================================================
 * CR2 REGISTER
 * ============================================================ */

#define I2C_MAX_CR2_FREQ_MASK     0x3FU


/* ============================================================
 * CCR REGISTER BITS
 * ============================================================ */

#define I2C_MAX_CCR_FS            15U
#define I2C_MAX_CCR_DUTY          14U


/* ============================================================
 * SR1 REGISTER BITS
 * ============================================================ */

#define I2C_MAX_SR1_SB            0U
#define I2C_MAX_SR1_ADDR          1U
#define I2C_MAX_SR1_BTF           2U
#define I2C_MAX_SR1_RXNE          6U
#define I2C_MAX_SR1_TXE           7U
#define I2C_MAX_SR1_AF            10U


/* ============================================================
 * SR2 REGISTER BITS
 * ============================================================ */

#define I2C_MAX_SR2_BUSY          1U


/* ============================================================
 * RCC
 * ============================================================ */

/*
 * STM32F401:
 *
 * APB1ENR
 * I2C2EN = bit 22
 */
#define RCC_I2C2_MAX_BIT          22U


/* ============================================================
 * TIMEOUT
 * ============================================================ */

#define I2C_MAX_TIMEOUT_LOOPS     100000UL


#endif /* MCAL_I2C_MAX_I2C_MAX_PRV_H_ */

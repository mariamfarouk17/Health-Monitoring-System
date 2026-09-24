/*
 * I2C_prv.h
 *
 *  Created on: Sep 7, 2026
 *      Author: HP
 */

#ifndef MCAL_I2C_I2C_PRV_H_
#define MCAL_I2C_I2C_PRV_H_

#define I2C1_BASE_ADDR        0x40005400UL

typedef struct {
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 OAR1;
    volatile u32 OAR2;
    volatile u32 DR;
    volatile u32 SR1;
    volatile u32 SR2;
    volatile u32 CCR;
    volatile u32 TRISE;
    volatile u32 FLTR;
} I2C_MemMap_t;

#define I2C1                  ((volatile I2C_MemMap_t *)(I2C1_BASE_ADDR))

/* CR1 bits */
#define I2C_CR1_PE            0
#define I2C_CR1_START         8
#define I2C_CR1_STOP          9
#define I2C_CR1_ACK           10
#define I2C_CR1_SWRST         15

/* CR2 bits */
#define I2C_CR2_FREQ_MASK     0x3F

/* CCR bits */
#define I2C_CCR_FS            15
#define I2C_CCR_DUTY          14

/* SR1 bits */
#define I2C_SR1_SB            0      // Start Bit (Master Mode)
#define I2C_SR1_ADDR          1      // Address sent
#define I2C_SR1_BTF           2      // Byte Transfer Finished
#define I2C_SR1_RXNE          6      // Data Register Not Empty
#define I2C_SR1_TXE           7      // Data Register Empty
#define I2C_SR1_AF            10     // Acknowledge Failure

#define RCC_I2C1_BIT          21
#define I2C_TIMEOUT_LOOPS     100000UL

#endif /* MCAL_I2C_I2C_PRV_H_ */

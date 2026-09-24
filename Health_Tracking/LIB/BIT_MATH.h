/*
 * BIT_MATH.h
 *
 *  Created on: Jul 23, 2026
 *      Author: HP
 */

#ifndef LIB_BIT_MATH_H_
#define LIB_BIT_MATH_H_

#define No_of_bits 32

//set bit
#define SET_BIT(reg , bitn)         ((reg) |= (1U << (bitn)))

//Clear bit
#define CLR_BIT(reg , bitn)       ((reg) &= (~(1U << (bitn))))

//Toggle bit
#define TOGGLE_BIT(reg , bitn)      ((reg) ^= (1U << (bitn)))

//Get bit
#define GET_BIT(reg , bitn)         (((reg) >> (bitn)) & 1U)

//Set byte
#define SET_BYTE(reg , value)       ((reg) = (value))

// Rotate left
// #define ROT_LEFT(reg , bitn)        ((unsigned char)((reg) << (bitn)) | ((reg) >> (No_of_bits - bitn)))

// Rotate right
// #define ROT_RIGHT(reg , bitn)       ((unsigned char)((reg) >> (bitn)) | ((reg) << (No_of_bits - bitn)))


#endif /* LIB_BIT_MATH_H_ */

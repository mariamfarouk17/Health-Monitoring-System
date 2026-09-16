/*
 * BIT_MATH.h
 *
 *  Created on: Aug 18, 2026
 *      Author: PC
 */

#ifndef LIB_BIT_MATH_H_
#define LIB_BIT_MATH_H_



#define SET_BIT(reg,bitN)       	((reg) |=  (1U << (bitN) ))

#define CLR_BIT(reg,bitN)       	((reg) &= (~(1U << (bitN) )))

#define TOG_BIT(reg,bitN)           ((reg) ^=  (1U << (bitN) ))

#define GET_BIT(reg,bitN)           (((reg) >> (bitN) ) & (1U))

#define SET_BYTE(reg,value)         ( (reg) = (value) )

// #define ROT_LEFT(reg,bitN)          ((unsigned char)((reg<<bitN)|(reg>>(No_of_bits - bitN))))

// #define ROT_RIGHT(reg,bitN)         ((unsigned char)((reg>>bitN)|(reg<<(No_of_bits - bitN))))




#endif /* LIB_BIT_MATH_H_ */

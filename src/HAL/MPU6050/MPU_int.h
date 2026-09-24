/*
 * MPU_int.h
 *
 *  Created on: Sep 8, 2026
 *      Author: Tona
 */

#ifndef HAL_MPU6050_MPU_INT_H_
#define HAL_MPU6050_MPU_INT_H_

/*void HMPU_vInit(void);
void HMPU_vGetRawValues(s16 *X, s16 *Y, s16 *Z);
void HMPU_vGetAcceleration(float *X, float *Y, float *Z);*/

void HMPU_vInit(void);

u8 HMPU_u8ReadReg(u8 A_u8Reg);

void HMPU_vWriteReg(u8 A_u8Reg, u8 A_u8Data);

void HMPU_vGetRawValues(s16 *A_ps16X, s16 *A_ps16Y, s16 *A_ps16Z);

void HMPU_vGetAcceleration(float *A_pfX, float *A_pfY, float *A_pfZ);

u8 HMPU_u8GetID(void);

#endif /* HAL_MPU6050_MPU_INT_H_ */

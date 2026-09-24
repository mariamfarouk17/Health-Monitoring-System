/*
 * MPU_prg.c
 *
 *  Created on: Sep 8, 2026
 *      Author: Tona
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/I2C/I2C_int.h"

#include "MPU_int.h"
#include "MPU_prv.h"


/*========================================================
 *              LOW LEVEL REGISTER ACCESS
 *========================================================*/

u8 HMPU_u8ReadReg(u8 A_u8Reg)
{
    u8 L_u8Data = 0U;

    MI2C1_u8ReadReg(MPU6050_ADDRESS, A_u8Reg, &L_u8Data);

    return L_u8Data;
}


void HMPU_vWriteReg(u8 A_u8Reg, u8 A_u8Data)
{
    MI2C1_u8WriteReg(MPU6050_ADDRESS, A_u8Reg, A_u8Data);
}


/*========================================================
 *                    INITIALIZATION
 *========================================================*/

void HMPU_vInit(void)
{
    u8 L_u8Data;

    /*----------------------------------------------------
     * 1) Wake up MPU6050
     *    Clear SLEEP bit (bit 6)
     *----------------------------------------------------*/

    L_u8Data = HMPU_u8ReadReg(PWR_MGMT_1);

    CLR_BIT(L_u8Data, 6);

    HMPU_vWriteReg(PWR_MGMT_1, L_u8Data);


    /*----------------------------------------------------
     * 2) Select Accelerometer Full Scale = ±2g
     *    AFS_SEL bits [4:3] = 00
     *----------------------------------------------------*/

    L_u8Data = HMPU_u8ReadReg(ACCEL_CONFIG);

    CLR_BIT(L_u8Data, 3);
    CLR_BIT(L_u8Data, 4);

    HMPU_vWriteReg(ACCEL_CONFIG, L_u8Data);
}


/*========================================================
 *                 READ RAW ACCELERATION
 *========================================================*/

void HMPU_vGetRawValues(s16 *A_ps16X, s16 *A_ps16Y, s16 *A_ps16Z)
{
    u8 L_u8Buffer[6];

    /* Read:
     * X_H, X_L, Y_H, Y_L, Z_H, Z_L
     */

    MI2C1_u8ReadBurst(MPU6050_ADDRESS, ACCEL_XOUT_H,  L_u8Buffer, 6U);


    /* Combine High Byte + Low Byte */

    *A_ps16X = (s16)((L_u8Buffer[0] << 8) | L_u8Buffer[1]);

    *A_ps16Y = (s16)((L_u8Buffer[2] << 8) | L_u8Buffer[3]);

    *A_ps16Z = (s16)((L_u8Buffer[4] << 8) | L_u8Buffer[5]);
}


/*========================================================
 *              CONVERT RAW VALUES TO "g"
 *========================================================*/

void HMPU_vGetAcceleration(float *A_pfX, float *A_pfY, float *A_pfZ)
{
    s16 L_s16RawX;
    s16 L_s16RawY;
    s16 L_s16RawZ;

    HMPU_vGetRawValues(&L_s16RawX,&L_s16RawY, &L_s16RawZ);


    *A_pfX = L_s16RawX / MPU6050_ACCEL_SENSITIVITY;
    *A_pfY = L_s16RawY / MPU6050_ACCEL_SENSITIVITY;
    *A_pfZ = L_s16RawZ / MPU6050_ACCEL_SENSITIVITY;
}

u8 HMPU_u8GetID(void)
{
    return HMPU_u8ReadReg(WHO_AM_I);
}
/*void HMPU_vInit(void)
{
	u8 data;

	//WAKE UP SENSOR
	data = MI2C_u8ReadReg(PWR_MGMT_1);
	CLR_BIT(data, 6);	//6 -> SLEEP
	MI2C_vWriteReg(PWR_MGMT_1, data);

	//AFS SELECTION (2g)
	data = MI2C_u8ReadReg(ACCEL_CONFIG);
	CLR_BIT(data, 3);
	CLR_BIT(data, 4);
	MI2C_vWriteReg(ACCEL_CONFIG, data);


	//CHECK
	data = MI2C_u8ReadReg(WHO_AM_I);

	if (data == 0x68)
	{
	    // MPU6050 detected
	}
	else
	{
	    // MPU6050 not detected
	}

}

void HMPU_vGetRawValues(s16 *X, s16 *Y, s16 *Z)
{
	u8 X_H, X_L;
	u8 Y_H, Y_L;
	u8 Z_H, Z_L;

	X_H = MI2C_u8ReadReg(ACCEL_XOUT_H);
	X_L = MI2C_u8ReadReg(ACCEL_XOUT_L);

	Y_H = MI2C_u8ReadReg(ACCEL_YOUT_H);
	Y_L = MI2C_u8ReadReg(ACCEL_YOUT_L);

	Z_H = MI2C_u8ReadReg(ACCEL_ZOUT_H);
	Z_L = MI2C_u8ReadReg(ACCEL_ZOUT_L);

	*X = (s16)((X_H << 8) | X_L);
	*Y = (s16)((Y_H << 8) | Y_L);
	*Z = (s16)((Z_H << 8) | Z_L);

}

void HMPU_vGetAcceleration(float *X, float *Y, float *Z)
{
    s16 RawX, RawY, RawZ;

    HMPU_vGetRawValues(&RawX, &RawY, &RawZ);

    *X = RawX / 16384.0;
    *Y = RawY / 16384.0;
    *Z = RawZ / 16384.0;
}*/






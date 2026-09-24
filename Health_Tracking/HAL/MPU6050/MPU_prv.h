/*
 * MPU_prv.h
 *
 *  Created on: Sep 8, 2026
 *      Author: Tona
 */

#ifndef HAL_MPU6050_MPU_PRV_H_
#define HAL_MPU6050_MPU_PRV_H_

/*#define ACCEL_CONFIG 	*((volatile u8 *)(0x1C))

#define ACCEL_XOUT_H	*((volatile u8 *)(0x3B))
#define ACCEL_XOUT_L	*((volatile u8 *)(0x3C))

#define ACCEL_YOUT_H	*((volatile u8 *)(0x3D))
#define ACCEL_YOUT_L	*((volatile u8 *)(0x3E))

#define ACCEL_ZOUT_H	*((volatile u8 *)(0x3F))
#define ACCEL_ZOUT_L	*((volatile u8 *)(0x40))

#define PWR_MGMT_1		*((volatile u8 *)(0x6B))
#define WHO_AM_I		*((volatile u8 *)(0x75))*/

#define MPU6050_ADDRESS       0x68U

#define WHO_AM_I              0x75U
#define PWR_MGMT_1            0x6BU
#define ACCEL_CONFIG          0x1CU

#define ACCEL_XOUT_H          0x3BU
#define ACCEL_XOUT_L          0x3CU
#define ACCEL_YOUT_H          0x3DU
#define ACCEL_YOUT_L          0x3EU
#define ACCEL_ZOUT_H          0x3FU
#define ACCEL_ZOUT_L          0x40U

#define MPU6050_ACCEL_SENSITIVITY    16384.0f


#endif /* HAL_MPU6050_MPU_PRV_H_ */

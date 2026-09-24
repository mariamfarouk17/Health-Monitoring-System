/*
 * NCIV_prv.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Tona
 */

#ifndef MCAL_NVIC_NVIC_PRV_H_
#define MCAL_NVIC_NVIC_PRV_H_

#define NVIC_BASE_ADDRESS		0xE000E100U

typedef struct
{
	u32 ISERx[8];
	u32 Reserved0[24];

	u32 ICERx[8];
	u32 Reserved1[24];

	u32 ISPRx[8];
	u32 Reserved2[24];

	u32 ICPRx[8];
	u32 Reserved3[24];

	u32 IABRx[8];
	u32 Reserved4[56];

	u8 IPRx[240];
	u32 Reserved6[644];

	u32 STIR;

}NVIC_MemMap_t;

#define NVIC	((volatile NVIC_MemMap_t*)(NVIC_BASE_ADDRESS))

#define SCB_AIRCR 		*((volatile u32*)(0xE000ED0C))

#define VECTKEY 		0x05FA0000
#endif /* MCAL_NVIC_NVIC_PRV_H_ */

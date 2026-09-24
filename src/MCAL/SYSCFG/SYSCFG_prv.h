/*
 * SYSCFG_prv.h
 *
 *  Created on: Aug 24, 2026
 *      Author: Tona
 */

#ifndef MCAL_SYSCFG_SYSCFG_PRV_H_
#define MCAL_SYSCFG_SYSCFG_PRV_H_


#define SYSCFG_BASE_ADDRESS		0x40013800U

typedef struct
{
	u32 MEMRMP;
	u32 PMC;
	u32 EXTICRx[4];
	u32 RESERVED[2];
	u32 CMPCR;
}SYSCFG_MemMap_t;

#define SYSCFG		((volatile SYSCFG_MemMap_t*)(SYSCFG_BASE_ADDRESS))
#endif /* MCAL_SYSCFG_SYSCFG_PRV_H_ */

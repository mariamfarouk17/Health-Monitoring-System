/*
 * NVIC_int.h
 *
 *  Created on: Aug 22, 2026
 *      Author: PC
 */

#ifndef MCAL_NVIC_NVIC_INT_H_
#define MCAL_NVIC_NVIC_INT_H_



typedef enum
{
	Group16Sub0 = 3,
	Group8Sub2,
	Group4Sub4,
	Group2Sub8,
	Group0Sub16,
}NVIC_Group_t;

void MNVIC_vEnable_Peripheral_INIT(u8 A_u8Position);



void MNVIC_vDisable_Peripheral_INIT(u8 A_u8Position);

void MNVIC_vSetPendingFlag(u8 A_u8Position);

void MNVIC_vCLRPendingFlag(u8 A_u8Position);

u8 MNVIC_vGetFlagStatus(u8 A_u8Position);


void MNVIC_vSetGroupPriority(NVIC_Group_t A_xGroupPriority);

void MNVIC_vSetPeripheralPriority(u8 A_u8Position, u8 A_u8Group , u8 A_u8SubGroup);



#endif /* MCAL_NVIC_NVIC_INT_H_ */

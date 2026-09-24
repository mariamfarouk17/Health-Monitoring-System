/*
 * RCC_prg.c
 *
 *  Created on: Aug 17, 2026
 *      Author: HP
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "RCC_int.h"
#include "RCC_prv.h"
#include "RCC_cfg.h"

void MRCC_vInit(void)
{
	// HSE
#if RCC_SYS_CLK == HSE_CLK

#if HSE_BYPASS == RC_CLK

	SET_BIT(RCC -> CR , HSE_BYP);

#elif HSE_BYPASS == MECHANICAL_CLK

	CLR_BIT(RCC -> CR , HSE_BYP);

#endif

	// HSE ON
	SET_BIT(RCC -> CR , HSE_ON);

	while(!GET_BIT(RCC -> CR , HSE_RDY))
		{

		}
		CLR_BIT(RCC -> CFGR , 1);
		SET_BIT(RCC -> CFGR , 0);


	// HSI
#elif RCC_SYS_CLK == HSI_CLK

	SET_BIT(RCC -> CR , HSI_ON);

	while(!GET_BIT(RCC -> CR , HSI_RDY));
		CLR_BIT(RCC -> CFGR , 1);
		CLR_BIT(RCC -> CFGR , 0);


	// PLL
#elif RCC_SYS_CLK == PLL_CLK

#else
#error "Invalid Option"

#endif

}

void MRCC_vEnableCLK(u8 A_u8BusID, u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{
//		switch(A_u8PeripheralID)
//		{
//		case RCC_GPIOA:
//			SET_BIT(RCC->AHB1ENR, 0);
//		case RCC_GPIOB:
//			SET_BIT(RCC->AHB1ENR, 1);
//		}
	case RCC_AHB1:
		SET_BIT(RCC->AHB1ENR, A_u8PeripheralID);
		break;

	case RCC_AHB2:
		SET_BIT(RCC->AHB2ENR, A_u8PeripheralID);

		break;

	case RCC_APB1:
		SET_BIT(RCC->APB1ENR, A_u8PeripheralID);
		break;

	case RCC_APB2:
		SET_BIT(RCC->APB2ENR, A_u8PeripheralID);
		break;
	}
}

void MRCC_vDisableCLK(u8 A_u8BusID, u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{

	case RCC_AHB1:
		CLR_BIT(RCC->AHB1ENR, A_u8PeripheralID);
		break;

	case RCC_AHB2:
		CLR_BIT(RCC->AHB2ENR, A_u8PeripheralID);
		break;

	case RCC_APB1:
		CLR_BIT(RCC->APB1ENR, A_u8PeripheralID);
		break;

	case RCC_APB2:
		CLR_BIT(RCC->APB2ENR, A_u8PeripheralID);
		break;
	}
}

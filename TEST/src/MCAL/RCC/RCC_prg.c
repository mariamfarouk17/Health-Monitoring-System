/*
 * RCC_int.c
 *
 *  Created on: Aug 17, 2026
 *      Author: PC
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"


#include "RCC_int.h"
#include "RCC_prv.h"
#include "RCC_cfg.h"






void MRCC_vInit(void)
{
#if RCC_SYS_CLK == HSE_CLK
#if HSE_BYPATH == RC_CLK


	SET_BIT(RCC->CR , HSEBYP);


#elif HSE_BYPATH == MECANICAL_CLK


	CLR_BIT(RCC->CR , HSEBYP);


#endif
	SET_BIT(RCC->CR , 16);

	while(!GET_BIT(RCC->CR , 17));


	CLR_BIT(RCC->CFGR ,1);
	SET_BIT(RCC->CFGR ,0);


#elif RCC_SYS_CLK == HSI_CLK

	SET_BIT(RCC->CR , 0);

	while(!GET_BIT(RCC->CR , 1));

	CLR_BIT(RCC->CFGR ,1);
	CLR_BIT(RCC->CFGR ,0);

#elif RCC_SYS_CLK == PLL_CLK

	SET_BIT(RCC->CR , 24);

	while(!GET_BIT(RCC->CR , 1));

	SET_BIT(RCC->CFGR ,1);
	CLR_BIT(RCC->CFGR ,0);



#else
#error  "invalid option"
#endif
}


void MRCC_vEnableclk(u8 A_u8BusID , u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{
	case RCC_AHB1:

		SET_BIT(RCC->AHB1ENR , A_u8PeripheralID);

		break;

	case RCC_AHB2:

		SET_BIT(RCC->AHB2ENR , A_u8PeripheralID);

		break;

	case RCC_APB1:

		SET_BIT(RCC->APB2ENR , A_u8PeripheralID);

		break;

	case RCC_APB2:

		SET_BIT(RCC->APB2ENR , A_u8PeripheralID);

		break;
	}
}


void MRCC_vDisableclk(u8 A_u8BusID , u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{
	case RCC_AHB1:

		CLR_BIT(RCC->AHB1ENR , A_u8PeripheralID);

		break;

	case RCC_AHB2:

		CLR_BIT(RCC->AHB2ENR , A_u8PeripheralID);

		break;

	case RCC_APB1:

		CLR_BIT(RCC->APB2ENR , A_u8PeripheralID);

		break;

	case RCC_APB2:

		CLR_BIT(RCC->APB2ENR , A_u8PeripheralID);

		break;
	}
}





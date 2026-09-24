/*
 * SPI_prg.c
 *
 *  Created on: Sep 2, 2026
 *      Author: Tona
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SPI_int.h"
#include "SPI_prv.h"

void MSPI_vInit(void)
{
	//SW SLAVE
	SET_BIT(SPI1 -> CR1, SSM);

	//MASTER
	SET_BIT(SPI1 -> CR1, SSI);

	//DATA
	CLR_BIT(SPI1 -> CR1, DFF);

	//MSB FIRST
	CLR_BIT(SPI1 -> CR1, LSBFIRST);

	//SELECT MASTER
	SET_BIT(SPI1 -> CR1, MSTR);

	//CLK POLARITY & PHASE
	CLR_BIT(SPI1 -> CR1, CPOL);
	CLR_BIT(SPI1 -> CR1, CPHA);

	//ENABLE SPI
	SET_BIT(SPI1 -> CR1, SPE);

}

u8 MSPI_u8Transceive(u8 A_u8Data)
{
	while(!GET_BIT(SPI1 ->SR, TXE));
	SPI1 -> DR = A_u8Data;

	while(!GET_BIT(SPI1 ->SR, RXNE));
	return SPI1 -> DR;

}


/*
 * SPI_prv.h
 *
 *  Created on: Sep 2, 2026
 *      Author: Tona
 */

#ifndef MCAL_SPI_SPI_PRV_H_
#define MCAL_SPI_SPI_PRV_H_

#define SPI1_BASE_ADDRESS		0x40013000U
#define SPI2_BASE_ADDRESS		0x40003800U
#define SPI3_BASE_ADDRESS		0x40003C00U
#define SPI4_BASE_ADDRESS		0x40013400U

//BITS
#define CPHA		0
#define CPOL		1
#define MSTR		2
#define SPE			6
#define LSBFIRST	7
#define SSI			8
#define	SSM			9
#define DFF			11

#define RXNE		0
#define TXE			1


typedef struct
{
	u32 CR1;
	u32 CR2;
	u32 SR;
	u32 DR;
	u32 CRCPR;
	u32 RXCRCR;
	u32 TXCRCR;
	u32 I2SCFGR;
	u32 I2SPR;

}SPI_MemMap_t;


#define SPI1		((volatile SPI_MemMap_t *)(SPI1_BASE_ADDRESS))
#define SPI2		((volatile SPI_MemMap_t *)(SPI2_BASE_ADDRESS))
#define SPI3		((volatile SPI_MemMap_t *)(SPI3_BASE_ADDRESS))
#define SPI4		((volatile SPI_MemMap_t *)(SPI4_BASE_ADDRESS))

#endif /* MCAL_SPI_SPI_PRV_H_ */

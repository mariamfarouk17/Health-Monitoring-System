/*
 * leds.c
 *
 *  Created on: Aug 22, 2026
 *      Author: PC
 */



/*
 *
 * #include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#define DELAY_MS(d)   do{unsigned int i = d * 4000; while(i--){asm ("NOP");}}while(0)

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableclk(RCC_AHB1, RCC_GPIOA);

    GPIOx_PinConfig_t LED = {
        GPIO_PORTA, GPIO_PIN0,
        .Mode = GPIO_MODE_OUTPUT,
        .Speed = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&LED);

    while(1)
    {
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
        DELAY_MS(500);
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
        DELAY_MS(500);
    }

    return 0;
}



 *
 *
 *
 *
 */






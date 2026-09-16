/*
 * 7segment.c
 *
 *  Created on: Aug 22, 2026
 *      Author: PC
 */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

/*
 *
 *
 * #define DELAY_MS(d)   do{unsigned int i = d * 4000; while(i--){asm ("NOP");}}while(0)

const u8 SevenSegDecoder[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};

void Display_7Segment(u8 number)
{
    if (number > 9) number = 9;
    u8 pattern = SevenSegDecoder[number];

    for (u8 pin = 0; pin < 7; pin++)
    {
        u8 pinVal = GET_BIT(pattern, pin);
        MGPIO_vSetPinValue(GPIO_PORTA, pin, pinVal);
    }
}

int main(void)
{
    MRCC_vInit();

    MRCC_vEnableclk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableclk(RCC_AHB1, RCC_GPIOB);

    for (u8 pin = 0; pin < 7; pin++)
    {
        GPIOx_PinConfig_t segPin = {
            GPIO_PORTA, pin,
            .Mode = GPIO_MODE_OUTPUT,
            .Speed = GPIO_SPEED_LOW,
            .OutputType = GPIO_OT_PUSHPULL,
            .PullType = GPIO_NO_PULL
        };
        MGPIO_vPinInit(&segPin);
    }

    GPIOx_PinConfig_t btnPin = {
        GPIO_PORTB, GPIO_PIN0,
        .Mode = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };
    MGPIO_vPinInit(&btnPin);

    u8 count = 0;

    while(1)
    {
        Display_7Segment(count);

        if (MGPIO_u8GetPinValue(GPIO_PORTB, GPIO_PIN0) == GPIO_LOW)
        {
            DELAY_MS(30);

            if (MGPIO_u8GetPinValue(GPIO_PORTB, GPIO_PIN0) == GPIO_LOW)
            {
                count++;
                if (count > 9)
                {
                    count = 0;
                }

                while (MGPIO_u8GetPinValue(GPIO_PORTB, GPIO_PIN0) == GPIO_LOW);
                DELAY_MS(30);
            }
        }
    }

    return 0;
}
 *
 *
 */






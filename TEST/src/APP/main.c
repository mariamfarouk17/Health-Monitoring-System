#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"

/*
 * void Toggle_LED(void)
{

	MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
}

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableclk(RCC_AHB1, RCC_GPIOA);


    GPIOx_PinConfig_t LED = {
        GPIO_PORTA, GPIO_PIN1,
        .Mode = GPIO_MODE_OUTPUT,
        .Speed = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&LED);

    GPIOx_PinConfig_t BTN = {
        GPIO_PORTA, GPIO_PIN0,
        .Mode = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };
    MGPIO_vPinInit(&BTN);



    MEXTI_vSetCallBack(Toggle_LED, EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vEnable(EXTI_LINE0);

    MNVIC_vEnable_Peripheral_INIT(6);

    while(1)
    {
    }

    return 0;
}
 *
 *
 *
 */



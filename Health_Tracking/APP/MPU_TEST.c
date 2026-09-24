//#include "../LIB/STD_TYPES.h"
//#include "../LIB/BIT_MATH.h"
//
//#include "../MCAL/RCC/RCC_int.h"
//#include "../MCAL/GPIO/GPIO_int.h"
//#include "../MCAL/I2C/I2C_int.h"
//#include "../MCAL/SPI/SPI_int.h"
//
//#include "../HAL/MPU6050/MPU_int.h"
//#include "../HAL/TFT/TFT_int.h"
//
//#include <math.h> // Needed for sqrtf
//
//#define TFT_BLACK   0x0000
//#define TFT_WHITE   0xFFFF
//#define TFT_GREEN   0x07E0
//
//// Algorithm Tuning Constants (in g's)
//#define STEP_THRESHOLD_HI   1.25f   // High peak trigger
//#define STEP_THRESHOLD_LO   1.05f   // Low reset trigger
//#define SMOOTHING_ALPHA     0.15f   // Filter strength
//
//u16 Step_Count = 0;
//u8  Step_State = 0; // 0 = Waiting for Peak, 1 = Peak Detected
//float Filtered_Mag = 1.0f;
//
//int main()
//{
//    float Xg, Yg, Zg;
//    float magnitude;
//
//    MRCC_vInit();
//    MI2C1_vInit();
//    HMPU_vInit();
//
//    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
//    MRCC_vEnableCLK(RCC_APB2, RCC_SPI1);
//
//    GPIOx_PinConfig_t MOSI = {
//        .Port = GPIO_PORTA, .Pin = GPIO_PIN7,
//        .Mode = GPIO_MODE_AF, .AltFunc = GPIO_AF5
//    };
//    GPIOx_PinConfig_t SCK = {
//        .Port = GPIO_PORTA, .Pin = GPIO_PIN5,
//        .Mode = GPIO_MODE_AF, .AltFunc = GPIO_AF5
//    };
//
//    MGPIO_vPinInit(&MOSI);
//    MGPIO_vPinInit(&SCK);
//
//    HTFT_vInit();
//    HTFT_vFillBackgroundColor(TFT_BLACK);
//
//    // Draw static label once to avoid wasting TFT transmission time
//    HTFT_vPrintString("STEPS:", 10, 20, TFT_WHITE, TFT_BLACK);
//    HTFT_vPrintNumber(Step_Count, 70, 20, TFT_GREEN, TFT_BLACK);
//
//    while(1)
//    {
//        // 1. Get acceleration directly in g's using your driver function
//        HMPU_vGetAcceleration(&Xg, &Yg, &Zg);
//
//        // 2. Compute 3D Magnitude
//        magnitude = sqrtf((Xg * Xg) + (Yg * Yg) + (Zg * Zg));
//
//        // 3. Low Pass Filter (Smooth out noise)
//        Filtered_Mag = (SMOOTHING_ALPHA * magnitude) + ((1.0f - SMOOTHING_ALPHA) * Filtered_Mag);
//
//        // 4. Dual-Threshold Peak Detection State Machine
//        if (Step_State == 0)
//        {
//            // Waiting for acceleration peak above upper threshold
//            if (Filtered_Mag > STEP_THRESHOLD_HI)
//            {
//                Step_State = 1;
//            }
//        }
//        else if (Step_State == 1)
//        {
//            // Waiting for acceleration to drop back down to register step
//            if (Filtered_Mag < STEP_THRESHOLD_LO)
//            {
//                Step_Count++;
//                Step_State = 0;
//
//                // Only update screen on step count increment
//                HTFT_vPrintNumber(Step_Count, 70, 20, TFT_GREEN, TFT_BLACK);
//            }
//        }
//    }
//    return 0;
//}

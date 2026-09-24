/*
 * VITALS_prg.c
 *
 * Heart Rate + SpO2 calculation
 */

#include "../../LIB/STD_TYPES.h"

#include "VITALS_int.h"


/* =========================================================
 * Configuration
 * ========================================================= */

#define VITALS_SAMPLE_RATE_HZ        100U
#define VITALS_BUFFER_SIZE           400U
#define VITALS_MIN_PEAK_DISTANCE      40U


#define VITALS_SPO2_MIN               70U
#define VITALS_SPO2_MAX              100U


/* =========================================================
 * Buffers
 * ========================================================= */

static u32 G_u32IRBuffer[VITALS_BUFFER_SIZE];

static u32 G_u32RedBuffer[VITALS_BUFFER_SIZE];

static u16 G_u16SampleCount = 0U;


/* =========================================================
 * Integer Square Root
 * ========================================================= */

static u32 VITALS_u32Sqrt(
    u32 A_u32Number
)
{
    u32 L_u32Result = 0U;
    u32 L_u32Bit = 1UL << 30U;


    while(
        L_u32Bit > A_u32Number
    )
    {
        L_u32Bit >>= 2U;
    }


    while(
        L_u32Bit != 0U
    )
    {
        if(
            A_u32Number >=
            (L_u32Result + L_u32Bit)
        )
        {
            A_u32Number -=
                L_u32Result +
                L_u32Bit;

            L_u32Result =
                (L_u32Result >> 1U) +
                L_u32Bit;
        }

        else
        {
            L_u32Result >>=
                1U;
        }


        L_u32Bit >>=
            2U;
    }


    return L_u32Result;
}


/* =========================================================
 * VITALS INIT
 * ========================================================= */

void MVITALS_vInit(void)
{
    u16 L_u16Index;


    G_u16SampleCount = 0U;


    for(
        L_u16Index = 0U;
        L_u16Index < VITALS_BUFFER_SIZE;
        L_u16Index++
    )
    {
        G_u32IRBuffer[L_u16Index] = 0U;

        G_u32RedBuffer[L_u16Index] = 0U;
    }
}


/* =========================================================
 * Add Sample
 * ========================================================= */

void MVITALS_vAddSample(
    u32 A_u32Red,
    u32 A_u32Ir
)
{
    if(
        G_u16SampleCount <
        VITALS_BUFFER_SIZE
    )
    {
        G_u32RedBuffer[
            G_u16SampleCount
        ] =
            A_u32Red;


        G_u32IRBuffer[
            G_u16SampleCount
        ] =
            A_u32Ir;


        G_u16SampleCount++;
    }
}


/* =========================================================
 * Check Buffer
 * ========================================================= */

u8 MVITALS_u8IsBufferFull(void)
{
    if(
        G_u16SampleCount >=
        VITALS_BUFFER_SIZE
    )
    {
        return 1U;
    }


    return 0U;
}


/* =========================================================
 * Heart Rate Calculation
 *
 * ORIGINAL CODE
 * ========================================================= */

u8 MVITALS_u8CalculateHR(
    u16 *A_pu16HR
)
{
    u16 L_u16Index;
    u16 L_u16PeakCount = 0U;
    u16 L_u16LastPeak = 0U;

    u32 L_u32Sum = 0U;
    u32 L_u32Average;
    u32 L_u32DistanceSum = 0U;
    u32 L_u32AverageDistance;


    if(A_pu16HR == NULL)
    {
        return 1U;
    }


    if(
        G_u16SampleCount <
        VITALS_BUFFER_SIZE
    )
    {
        return 2U;
    }


    /* Calculate average IR value */

    for(
        L_u16Index = 0U;
        L_u16Index < VITALS_BUFFER_SIZE;
        L_u16Index++
    )
    {
        L_u32Sum +=
            G_u32IRBuffer[L_u16Index];
    }


    L_u32Average =
        L_u32Sum /
        VITALS_BUFFER_SIZE;


    /* Detect peaks above average */

    for(
        L_u16Index = 1U;
        L_u16Index <
            (VITALS_BUFFER_SIZE - 1U);
        L_u16Index++
    )
    {
        if(
            (G_u32IRBuffer[L_u16Index] >
             L_u32Average)

            &&

            (G_u32IRBuffer[L_u16Index] >
             G_u32IRBuffer[
                 L_u16Index - 1U
             ])

            &&

            (G_u32IRBuffer[L_u16Index] >=
             G_u32IRBuffer[
                 L_u16Index + 1U
             ])
        )
        {
            if(
                L_u16PeakCount == 0U
            )
            {
                L_u16LastPeak =
                    L_u16Index;

                L_u16PeakCount++;
            }

            else
            {
                u16 L_u16Distance =
                    L_u16Index -
                    L_u16LastPeak;


                if(
                    L_u16Distance >=
                    VITALS_MIN_PEAK_DISTANCE
                )
                {
                    L_u32DistanceSum +=
                        L_u16Distance;


                    L_u16LastPeak =
                        L_u16Index;


                    L_u16PeakCount++;
                }
            }
        }
    }


    /* Reset buffer after calculation attempt */

    G_u16SampleCount = 0U;


    if(
        L_u16PeakCount < 2U
    )
    {
        return 3U;
    }


    L_u32AverageDistance =
        L_u32DistanceSum /
        (L_u16PeakCount - 1U);


    if(
        L_u32AverageDistance == 0U
    )
    {
        return 4U;
    }


    /*
     * HR = (60 * Fs) / samples per beat
     */

    *A_pu16HR =
        (u16)(
            (60U * VITALS_SAMPLE_RATE_HZ) /
            L_u32AverageDistance
        );


    return 0U;
}


/* =========================================================
 * SpO2 Calculation
 *
 * DC:
 *
 * DC = average signal
 *
 *
 * AC:
 *
 * AC = RMS(signal - DC)
 *
 *
 * Ratio:
 *
 * R =
 * (AC_RED / DC_RED) /
 * (AC_IR / DC_IR)
 *
 *
 * Approximation:
 *
 * SpO2 = 110 - 25R
 *
 * R is internally scaled by 1000.
 * ========================================================= */

u8 MVITALS_u8CalculateSpO2(
    u8 *A_pu8SpO2
)
{
    u16 L_u16Index;


    u32 L_u32RedSum = 0U;
    u32 L_u32IRSum = 0U;


    u32 L_u32RedDC;
    u32 L_u32IRDC;


    u32 L_u32RedSquareSum = 0U;
    u32 L_u32IRSquareSum = 0U;


    u32 L_u32RedAC;
    u32 L_u32IRAC;


    u32 L_u32Ratio;
    u32 L_u32SpO2;


    if(
        A_pu8SpO2 == NULL
    )
    {
        return 1U;
    }


    if(
        G_u16SampleCount <
        VITALS_BUFFER_SIZE
    )
    {
        return 2U;
    }


    /* =====================================================
     * Calculate DC
     * ===================================================== */

    for(
        L_u16Index = 0U;
        L_u16Index < VITALS_BUFFER_SIZE;
        L_u16Index++
    )
    {
        L_u32RedSum +=
            G_u32RedBuffer[L_u16Index];


        L_u32IRSum +=
            G_u32IRBuffer[L_u16Index];
    }


    L_u32RedDC =
        L_u32RedSum /
        VITALS_BUFFER_SIZE;


    L_u32IRDC =
        L_u32IRSum /
        VITALS_BUFFER_SIZE;


    if(
        (L_u32RedDC == 0U) ||
        (L_u32IRDC == 0U)
    )
    {
        return 3U;
    }


    /* =====================================================
     * Calculate AC squared sum
     * ===================================================== */

    for(
        L_u16Index = 0U;
        L_u16Index < VITALS_BUFFER_SIZE;
        L_u16Index++
    )
    {
        u32 L_u32RedDifference;
        u32 L_u32IRDifference;


        if(
            G_u32RedBuffer[L_u16Index] >=
            L_u32RedDC
        )
        {
            L_u32RedDifference =
                G_u32RedBuffer[L_u16Index] -
                L_u32RedDC;
        }
        else
        {
            L_u32RedDifference =
                L_u32RedDC -
                G_u32RedBuffer[L_u16Index];
        }


        if(
            G_u32IRBuffer[L_u16Index] >=
            L_u32IRDC
        )
        {
            L_u32IRDifference =
                G_u32IRBuffer[L_u16Index] -
                L_u32IRDC;
        }
        else
        {
            L_u32IRDifference =
                L_u32IRDC -
                G_u32IRBuffer[L_u16Index];
        }


        /*
         * Values are 18-bit MAX30102 samples.
         * Difference is therefore small enough
         * for u32 square calculation.
         */

        L_u32RedSquareSum +=
            L_u32RedDifference *
            L_u32RedDifference;


        L_u32IRSquareSum +=
            L_u32IRDifference *
            L_u32IRDifference;
    }


    /* =====================================================
     * Calculate RMS AC
     * ===================================================== */

    L_u32RedAC =
        VITALS_u32Sqrt(
            L_u32RedSquareSum /
            VITALS_BUFFER_SIZE
        );


    L_u32IRAC =
        VITALS_u32Sqrt(
            L_u32IRSquareSum /
            VITALS_BUFFER_SIZE
        );


    if(
        (L_u32RedAC == 0U) ||
        (L_u32IRAC == 0U)
    )
    {
        return 4U;
    }


    /* =====================================================
     * Ratio of Ratios
     *
     * R =
     *
     * (AC_RED / DC_RED)
     * -----------------
     * (AC_IR / DC_IR)
     *
     * R =
     *
     * AC_RED * DC_IR
     * ----------------
     * DC_RED * AC_IR
     *
     * Result multiplied by 1000.
     * ===================================================== */

    L_u32Ratio =
        (
            L_u32RedAC *
            L_u32IRDC *
            1000U
        )
        /
        (
            L_u32RedDC *
            L_u32IRAC
        );


    /* =====================================================
     * SpO2 approximation
     *
     * SpO2 = 110 - 25R
     * ===================================================== */

    if(
        L_u32Ratio >= 1600U
    )
    {
        return 5U;
    }


    L_u32SpO2 =
        110U -
        (
            25U *
            L_u32Ratio
        ) /
        1000U;


    /* =====================================================
     * Limit maximum
     * ===================================================== */

    if(
        L_u32SpO2 >
        VITALS_SPO2_MAX
    )
    {
        L_u32SpO2 =
            VITALS_SPO2_MAX;
    }


    /* =====================================================
     * Check minimum
     * ===================================================== */

    if(
        L_u32SpO2 <
        VITALS_SPO2_MIN
    )
    {
        return 6U;
    }


    *A_pu8SpO2 =
        (u8)L_u32SpO2;


    return 0U;
}

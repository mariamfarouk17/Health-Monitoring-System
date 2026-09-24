/*
 * RTOS_prg.c
 *
 *  Created on: Aug 26, 2026
 *      Author: HP
 */

#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/STK/STK_int.h"

#include "RTOS_int.h"
#include "RTOS_prv.h"
#include "RTOS_cfg.h"


Task_t SystemTasks[MAX_SYSTASK_SIZE] = {(NULL)};

u32 TimingArray[MAX_SYSTASK_SIZE] = {0};

void RTOS_vStart(void)
{
	MSTK_Config_t STK_cfg =
	{
			.InterruptEnable = INT_ENABLE,
			.CLK_SRC         = CLK_SRC_AHB_8
	};
	MSTK_vInit(&STK_cfg);

	MSTK_vSetIntervalMulti(TICKTIME , RTOS_vScheduler);
}

u8 RTOS_u8CreateTask(void (*TaskFunction)(void), u32 Periodicity, u8 A_u8Priority)
{
    u8 L_u8ErrorState = 0;

    if (A_u8Priority < MAX_SYSTASK_SIZE)
    {
        if (SystemTasks[A_u8Priority].TaskFunction == NULL)
        {
            SystemTasks[A_u8Priority].TaskFunction = TaskFunction;
            SystemTasks[A_u8Priority].Periodicity  = Periodicity;
            SystemTasks[A_u8Priority].State        = READY;

            /* FIX 1: Set initial counter to 0 so the task runs on its first period */
            TimingArray[A_u8Priority] = 0;
        }
        else
        {
            L_u8ErrorState = RESERVED_PRIORITY_ERROR;
        }
    }
    else
    {
        L_u8ErrorState = OUT_OF_SYSTASK_RANGE_ERROR;
    }

    return L_u8ErrorState;
}


void RTOS_vScheduler(void)
{
	for(u8 counter = 0 ; counter < MAX_SYSTASK_SIZE ; counter ++)
	{
		if(SystemTasks[counter].State == READY)
		{
			if(TimingArray[counter] == 0)
			{
				SystemTasks[counter].State = RUNNING;
				SystemTasks[counter].TaskFunction();
				SystemTasks[counter].State = READY;

				TimingArray[counter] = SystemTasks[counter].Periodicity;
			}
			else
			{
				TimingArray[counter] --;
			}
		}
	}
}


u8 RTOS_u8SuspendTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(A_u8Priority < MAX_SYSTASK_SIZE)
	{
		if(SystemTasks[A_u8Priority].State == RUNNING)
		{
		    SystemTasks[A_u8Priority].State = SUSPENDED;
		}
	}
	else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;

}


u8 RTOS_u8ResumeTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(A_u8Priority < MAX_SYSTASK_SIZE)
	{
		if(SystemTasks[A_u8Priority].State == SUSPENDED)
		{
		    SystemTasks[A_u8Priority].State = READY;
		    SystemTasks[A_u8Priority].State = RUNNING;
		}
	}
	else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;
}


u8 RTOS_u8DeleteTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(A_u8Priority < MAX_SYSTASK_SIZE)
	{
		if(SystemTasks[A_u8Priority].TaskFunction != NULL)
		{
		    SystemTasks[A_u8Priority].TaskFunction = NULL;
		}
	}
	else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;
}

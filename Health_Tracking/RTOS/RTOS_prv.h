/*
 * RTOS_prv.h
 *
 *  Created on: Aug 26, 2026
 *      Author: HP
 */

#ifndef RTOS_RTOS_PRV_H_
#define RTOS_RTOS_PRV_H_

typedef enum{
	READY = 1,
	RUNNING,
	SUSPENDED
}TASK_STATES_t;

typedef struct{
    void (*TaskFunction)(void);
    u32 Periodicity;
    TASK_STATES_t State;
}Task_t;


#define RESERVED_PRIORITY_ERROR          1
#define OUT_OF_SYSTASK_RANGE_ERROR       2
#define NO_TASK_ERROR                    3

#endif /* RTOS_RTOS_PRV_H_ */

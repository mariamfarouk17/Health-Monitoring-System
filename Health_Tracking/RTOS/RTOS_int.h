/*
 * RTOS_int.h
 *
 *  Created on: Aug 26, 2026
 *      Author: HP
 */

#ifndef RTOS_RTOS_INT_H_
#define RTOS_RTOS_INT_H_

void RTOS_vStart(void);

void RTOS_vScheduler(void);

u8 RTOS_u8CreateTask(void (*TaskFunction)(void) , u32 A_u32Periodicity , u8 A_u8Priority);
u8 RTOS_u8SuspendTask(u8 A_u8Priority);
u8 RTOS_u8ResumeTask(u8 A_u8Priority);
u8 RTOS_u8DeleteTask(u8 A_u8Priority);


/* ERROR States */
// 1 -> RESERVED_PRIORITY_ERROR
// 2 -> OUT_OT_SYSTASK_RANGE_ERROR
// 3 -> NO_TASK_ERROR

#endif /* RTOS_RTOS_INT_H_ */

/**
  ******************************************************************************
  * @file           : closed_loop.h
  * @brief          : Header for closed_loop.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#ifndef _CLOSED_LOOP_H
#define  _CLOSED_LOOP_H
/* 用户头文件引用 */
#include "pid.h"
/* 核心头文件引用 */
#include "main.h"


#define GM6020_NUM_MAX 1
#define RM2006_NUM_MAX 1

typedef struct 
{
	/* 目标值 */
	float target_value;
	/* 电机数据指针 */
	const motor_data_typedef* motor_data;
	/* PID参数及计算数据 */
	Pid_typedef *pid;

}Closed_Loop_typedef;

extern Closed_Loop_typedef motor_control[1];
extern void Speed_control_Init(void);
extern float Speed_control(Closed_Loop_typedef motor_control);
#endif



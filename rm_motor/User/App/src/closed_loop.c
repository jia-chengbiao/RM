/**
  ******************************************************************************
  * @file           : closed_loop.c
  * @brief          : 
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
  *******************************************************************************
  */
#include "closed_loop.h"

/**
  * @brief  速度闭环控制初始化
  * @param  
  * @retval 
  */
Closed_Loop_typedef motor_control[GM6020_NUM_MAX];
void Speed_control_Init(void)
{
		/* PID参数初始化 */
		const float PID[3] = {0,0,0};
		/* 电机数据初始化 */
		motor_control[0].motor_data = get_gm6020_data(1);
		/* PID数据初始化 */
		PID_Init(motor_control[0].pid,PID,0,0);
		
}

/**
  * @brief  速度闭环计算
  * @param  
  * @retval 
  */
float Speed_control(Closed_Loop_typedef motor_control)
{
    float current;

		current = PID_calc(motor_control.pid,1000,motor_control.motor_data->speed_rpm);

    return current;
}










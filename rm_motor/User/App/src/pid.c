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
#include "pid.h"


//极限判断
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }
		
void PID_Init(Pid_typedef *pid,const float PID[3], float max_out, float max_iout)
{
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
}
//PID计算
float PID_calc(Pid_typedef *pid, float target, float fdb)
{

    pid->err[1] = pid->err[0];
    pid->target = target;
    pid->fdb = fdb;
    pid->err[0] = target - fdb;
	
    //位置式PID
    pid->Pout = pid->Kp * pid->err[0];
    pid->Iout += pid->Ki * pid->err[0];
    pid->Dbuf = (pid->err[0] - pid->err[1]);
    pid->Dout = pid->Kd * pid->Dbuf;
    LimitMax(pid->Iout, pid->max_iout);
    pid->out = pid->Pout + pid->Iout + pid->Dout;
    LimitMax(pid->out, pid->max_out);
	
    return pid->out;
}












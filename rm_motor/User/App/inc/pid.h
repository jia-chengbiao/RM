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

#ifndef _PID_H
#define _PID_H

/* 用户头文件引用 */

/* 核心头文件引用 */
#include "main.h"

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float target;	//目标值
    float fdb;		//反馈值
    float err[2];	//err值
    float Dbuf;   //微分项

    float max_out;	//最大限幅
    float max_iout; //积分限幅

    float Pout;
    float Iout;
    float Dout;

    float out;		//总输出
}Pid_typedef;


extern void PID_Init(Pid_typedef *pid,const float PID[3], float max_out, float max_iout);
extern float PID_calc(Pid_typedef *pid, float target, float fdb);
#endif








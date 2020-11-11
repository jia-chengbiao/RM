/**
  ******************************************************************************
  * @file           : bsp_can.h
  * @brief          : Header for bsp_can.c file.
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

#ifndef _BSP_CAN_H
#define _BSP_CAN_H

/* 用户头文件引用 */

/* 核心头文件引用 */
#include "main.h"
#include "can.h"

#define GM6020_NUM_MAX 1
#define RM2006_NUM_MAX 1

typedef enum
{
  /* rm2006电机id号 */
  CAN_RM2006_ALL_ID = 0X200,
  CAN_RM2006_M1_ID = 0X201,

  /* gm6020电机id号 */
  CAN_GM6020_ALL_ID = 0x1FF,
  CAN_GM6020_M1_ID = 0x205,
}rm_motor_ID;
//rm电机统一数据结构体
typedef struct
{
    uint16_t ecd;           //编码器
		int16_t angle;          //换算后的角度值
    int16_t speed_rpm;      //转速
    int16_t given_current;  //输出电流
    uint8_t temperate;      //温度
    int16_t last_ecd;       //上一次编码器值
} motor_data_typedef;

/* 电机数据指针函数 */
extern const motor_data_typedef* get_gm6020_data(uint8_t motor_num);
extern const motor_data_typedef* get_rm2006_data(uint8_t motor_num);
/* 电机电流输出 */
void Out_motor_voltage(uint8_t id,int16_t motor1);

#endif


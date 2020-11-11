/**
  ******************************************************************************
  * @file           : bsp_can.c
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

#include "bsp_can.h"


/* 电机数据获取获取 */
static void Get_GM6020_Data(uint8_t motor_num,uint8_t RxData[8]);
static void Get_RM2006_Data(uint8_t motor_num,uint8_t RxData[8]);
static void Can_Hook(CAN_RxHeaderTypeDef* rx_header,uint8_t rx_data[8]);

/**
  * @brief   初始化过滤器, 开启CAN, 使能CAN接收中断
  * @param   hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval  无
	* @attention  
  */
void Can_User_Init(CAN_HandleTypeDef* hcan)
{
  CAN_FilterTypeDef  can_filter;

  can_filter.FilterBank = 0;                       // filter 0
  can_filter.FilterMode =  CAN_FILTERMODE_IDMASK;  // mask mode
  can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter.FilterIdHigh = 0;
  can_filter.FilterIdLow  = 0;
  can_filter.FilterMaskIdHigh = 0;
  can_filter.FilterMaskIdLow  = 0;                // set mask 0 to receive all can id
  can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // assign to fifo0
  can_filter.FilterActivation = ENABLE;           // enable can filter
  can_filter.SlaveStartFilterBank  = 14;          // only meaningful in dual can mode
   
  HAL_CAN_ConfigFilter(hcan, &can_filter);        // init can filter
  HAL_CAN_Start(&hcan1);                          // start can1
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // enable can1 rx interrupt
}

/********************************************中断回调*****************************************************************/
/**
  * @brief  CAN中断回调函数
  * @param  
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t             rx_data[8];
  if(hcan == (&hcan1))
  {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //receive can data
    Can_Hook(&rx_header,rx_data);
  }
}

/**************************************************电机数据获取***************************************************************/
/**
  * @brief   获取GM6020电机数据
  * @param   motor_num：电机编号
  * @retval  
	* @attention  
  */
static motor_data_typedef GM6020_Data[GM6020_NUM_MAX];
static void Get_GM6020_Data(uint8_t motor_num,uint8_t RxData[8])
{
	GM6020_Data[motor_num].last_ecd = GM6020_Data[motor_num].ecd;
  GM6020_Data[motor_num].ecd = (uint16_t)(RxData[0] << 8 | RxData[1]);
  GM6020_Data[motor_num].speed_rpm = (uint16_t)(RxData[2] << 8 | RxData[3]);
  GM6020_Data[motor_num].given_current = (uint16_t)(RxData[4] << 8 | RxData[5]);
  GM6020_Data[motor_num].temperate = (uint16_t)(RxData[6] << 8 | RxData[7]);
}

/**
  * @brief   获取RM2006电机数据
  * @param   motor_num：电机编号
  * @retval  无
	* @attention  
  */
static motor_data_typedef RM2006_Data[RM2006_NUM_MAX];
static void Get_RM2006_Data(uint8_t motor_num,uint8_t RxData[8])
{
	RM2006_Data[motor_num].last_ecd = RM2006_Data[motor_num].ecd;
  RM2006_Data[motor_num].ecd = (uint16_t)(RxData[0] << 8 | RxData[1]);
  RM2006_Data[motor_num].speed_rpm = (uint16_t)(RxData[2] << 8 | RxData[3]);
  RM2006_Data[motor_num].given_current = (uint16_t)(RxData[4] << 8 | RxData[5]);
  RM2006_Data[motor_num].temperate = (uint16_t)(RxData[6] << 8 | RxData[7]);
}
/**
  * @brief  全部电机数据获取
  * @param  rx_header：接收头
  *         rx_data[]:接收到的数据
  * @retval 在CAN中断回调函数中执行进行全部电机数据获取
  */
static void Can_Hook(CAN_RxHeaderTypeDef* rx_header,uint8_t rx_data[8])
{
    switch(rx_header->StdId)
    {
        case CAN_GM6020_M1_ID:
				{
						static uint8_t i = 0;
						//处理电机ID号
						i = rx_header->StdId - CAN_GM6020_M1_ID;
						//处理电机数据宏函数
						Get_GM6020_Data(i,rx_data);
						break;
				}
        case CAN_RM2006_M1_ID:
				{
						static uint8_t i = 0;
						//处理电机ID号
						i = rx_header->StdId - CAN_RM2006_M1_ID;
						//处理电机数据宏函数
						Get_RM2006_Data(i,rx_data);
						break;
				}
        default:
				{
						break;
				}
    }

}

/**********************************************电机数据指针***********************************************************/
/**
  * @brief  gm6020电机数据输出指针函数
  * @param  motor_num：电机编号
  * @retval 本函数用于初始化，只执行一次
  */
const motor_data_typedef* get_gm6020_data(uint8_t motor_num)
{
    return &GM6020_Data[motor_num];
}

/**
  * @brief  rm2006电机数据输出指针函数
  * @param  motor_num：电机编号
  * @retval 本函数用于初始化，只执行一次
  */
const motor_data_typedef* get_rm2006_data(uint8_t motor_num)
{
    return &RM2006_Data[motor_num];
}

/**
  * @brief  send motor control message through can bus
  * @param  id:电机编号
  *         motor1：对应电机的输出电流
  * @param  
  * @retval 这里的IdCard需要根据需要更改，这里只给出了6020的IdCard
  */
void Out_motor_voltage(uint8_t id,int16_t motor1)
{
  uint16_t            IdCard;
  CAN_TxHeaderTypeDef tx_header;
  uint8_t             tx_data[8] ={0};
    
  if(id < 4)
    IdCard = 0x1ff;
  else IdCard = 0x2ff;
  
  tx_header.StdId = IdCard;
  tx_header.IDE   = CAN_ID_STD;
  tx_header.RTR   = CAN_RTR_DATA;
  tx_header.DLC   = 8;

  tx_data[0] = (motor1>>8)&0xff;
  tx_data[1] =    (motor1)&0xff;
  HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0); 
}








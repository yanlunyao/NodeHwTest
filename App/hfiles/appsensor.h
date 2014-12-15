/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @copy
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPSENSOR_H
#define __APPSENSOR_H

#include "stm32f10x.h"

/*****************
define 
*****************/
//sensor num
#define		SENSOR_NUM													5

#define	 	SENSOR_ALARM_STATE							0x01		
#define		SENSOR_NORMAL_STATE							0x02
#define		SENSOR_SHORT_STATE							0x02
#define		SENSOR_PARALLEL_STATE						0x22


extern void AppSensorInit(void);
extern void AppSensorProcess(void);
extern u8 *GetCurrentSensorStatus(u8 *status);
extern void UploadRspondConfirmed(u8 sensorNum);
#endif 

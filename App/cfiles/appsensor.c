#include <stdio.h>
#include <string.h>
#include "hw_config.h"
#include "osadc.h"
#include "ostmr.h"
#include "appsensor.h"
#include "appusart.h"
#include "appCommProtocolDef.h"


/*****************
define 
*****************/
//sensor value
#define	 	SENSOR_NORMAL_MIN_VALUE							1380
#define		SENSOR_NORMAL_MAX_VALUE							1480
#define		SENSOR_ALARM_OPEN_THRESHOLD_VALUE		4000
#define		SENSOR_SHORT_THRESHOLD_VALUE				20
#define		SENSOR_PARALLEL_THRESHOLD_VALUE

//sensor upload event parameter
#define   REUPLOAD_SENSOR_STATUS_CONTER				50  //5s   temporary
#define 	REUPLOAD_CNT_MAX										3
//sensor state change effective time
#define 	SENSOR_STATE_CHANGE_VALID_TIME			5  //500ms

//
/*
********************************************************************************
  variable declaration
********************************************************************************
*/
// sensor channel
static u8 s_channel = 0;

// the buf of sensor state of old and new
static u8 s_old_status_buf[5] = {SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE};//测试默认为不接传感器状态，为alarm
static u8 s_new_status_buf[5] = {SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE, SENSOR_ALARM_STATE};

u8  g_sensor_timing_flg = 0;		// sensor timing flag
u16 g_sensor_cnt = 0;				// sensor count

//upload sensor event
typedef struct
{
	bool_t 		needUpRespondConfirm;  //需要确认的标记
	u8        uploadCnt;						 //重发次数：重发超过3次不再重发，此时置needUpRespondConfirm=false
	bool_t 		uploadFlag;            //1s计时到的标记
	u8     		Counter;               //计时值
	u8     		SensorMsg;	           //消息
}uploadSensorEvent_t;	
static uploadSensorEvent_t  uploadSensorStsEvent[SENSOR_NUM];
//bool_t UpRespondConfirmed[SENSOR_NUM];
/////////////////////////////////////////////////////////////////////


static void sensor_alarm_check(u8 ch, u8 *status);
static void App100msSensorTask(void);
static void sensor_scan(void);
static void ResetUploadEventValue(void);


static void ResetUploadEventValue(void)
{
	memset(uploadSensorStsEvent, 0, sizeof(uploadSensorEvent_t)*SENSOR_NUM);
//	memset(UpRespondConfirmed, 1, sizeof(UpRespondConfirmed));
}
static void App100msSensorTask(void)
{
	u8 i;
	//传感器状态变化事件计数
	if(g_sensor_timing_flg)
	{
		g_sensor_cnt++;
		if(g_sensor_cnt >= 65534)
		{
			g_sensor_cnt = 65534;
		}
	}
	else
	{
		g_sensor_cnt = 0;	
	}
	//上传事件计数
	for(i=0; i<SENSOR_NUM; i++)  
	{
		if(uploadSensorStsEvent[i].Counter)
		{
			uploadSensorStsEvent[i].Counter --;
			if(uploadSensorStsEvent[i].Counter==0)
			{
				if(uploadSensorStsEvent[i].needUpRespondConfirm)
					uploadSensorStsEvent[i].uploadFlag = TRUE;
			}	
		}	
	}
}

void AppSensorInit(void)
{
	OsAdcInit();
	t_ostmr_insertTask(App100msSensorTask, 100, OSTMR_PERIODIC);  // 100 ms task
	ResetUploadEventValue();
}

static void sensor_alarm_check(u8 ch, u8 *status)
{
	u16 adcx;
	u8 status_tmp;

	adcx = 0;
	status_tmp = 0;
	adcx = get_adc_average(ch, 100); //采样次数平均值
	
	if(adcx != 65535)
	{
		if(((adcx > SENSOR_NORMAL_MIN_VALUE) && (adcx < SENSOR_NORMAL_MAX_VALUE)) || (adcx < SENSOR_SHORT_THRESHOLD_VALUE))	// normal
		{
			s_channel++;		
			*status = SENSOR_NORMAL_STATE;
			g_sensor_timing_flg = 0;
			g_sensor_cnt = 0;
		}
		else
		{
			if(adcx > SENSOR_ALARM_OPEN_THRESHOLD_VALUE)					// alarm/open
			{
				status_tmp = SENSOR_ALARM_STATE;	
			}
//			else if((adcx < SENSOR_SHORT_THRESHOLD_VALUE))			// short  门磁合上adc值=短路值，所以等同于normal
//			{
//				status_tmp = SENSOR_SHORT_STATE;	
//			}
			else							// parallel
			{
				status_tmp = SENSOR_PARALLEL_STATE;
			}
			
			if(status_tmp == *status)
			{
				s_channel++;		
				g_sensor_timing_flg = 0;
				g_sensor_cnt = 0;	
			}
			else
			{
				g_sensor_timing_flg = 1;		
				if(g_sensor_cnt > SENSOR_STATE_CHANGE_VALID_TIME)			
				{
					g_sensor_timing_flg = 0;	
					g_sensor_cnt = 0;
					*status = status_tmp;
					s_channel++;	
				}
			}
		}
	}
}
static void sensor_scan(void)
{
	const u8 __numToChannel[] = {ADC_SENSOR1_CHANNEL, ADC_SENSOR2_CHANNEL, ADC_SENSOR3_CHANNEL, ADC_SENSOR4_CHANNEL,ADC_SENSOR5_CHANNEL};																						
	if(s_channel >= SENSOR_NUM)
	{
		s_channel = 0;
	}
	sensor_alarm_check(__numToChannel[s_channel], &s_new_status_buf[s_channel]); 
}

void AppSensorProcess(void)
{
	u8 i;
	u8 tempBuff[50];
	u8 shortString[] = " sensor be shorted \n";
	u8 numAscii[5] = {'0','1','2','3','4'};
	u16 tempBuffLen;
	sensor_scan();
	if(s_channel <= SENSOR_NUM)
	{
		i = s_channel -1;
		if(s_new_status_buf[i] != s_old_status_buf[i])
		{
			if(s_new_status_buf[i] ==  SENSOR_ALARM_STATE)
			{	
				uploadSensorStsEvent[i].SensorMsg = SENSOR_ALARM_STATE;//记录此消息
			}
			else
			{
				uploadSensorStsEvent[i].SensorMsg = SENSOR_SHORT_STATE;//
				memcpy(&tempBuff[1], shortString, sizeof(shortString));     
				tempBuff[0] = numAscii[i];
				tempBuffLen = sizeof(shortString) +1;
				//t_osscomm_sendMessage(tempBuff, tempBuffLen, USART2_COM);   //短路发送串口消息提示
				t_osscomm_sendMessage(tempBuff, tempBuffLen, USART1_COM); 
			}
			s_old_status_buf[i] = s_new_status_buf[i];	
		}
	}
}


u8 *GetCurrentSensorStatus(u8 *status)
{
  memcpy(status, s_old_status_buf, sizeof(s_old_status_buf));
	return status;
}
void UploadRspondConfirmed(u8 sensorNum)
{
	uploadSensorStsEvent[sensorNum].needUpRespondConfirm = FALSE;
}	

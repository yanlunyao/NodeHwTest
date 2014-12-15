/**
  ******************************************************************************
  * @file    
  * @author  yanly
  * @version 
  * @date    
  * @brief   ����ѧϰ�����ļ�
  ******************************************************************************/
	
#include "oskey.h"
#include "osqtmr.h"
#include "osled.h"
#include "osrf315tx.h"
#include "appkey.h"


/*define********************************************************************************************************/	
#define  KEY_STATUS_IDLE																0
#define  KEY_STATUS_PRESSING														1	
#define	 KEY_STATUS_REAL_PRESS													2
/*variable********************************************************************************************************/	
static u8 learnKeyStatus;
u8 nodeAddrValue;

/*********************************************************************************************************
*	brief: 100ms���ѧϰ���Ƿ񻹰��£�����Ϊ��ʵ����
*********************************************************************************************************/
void AppSanKeyTask_10ms()
{
	static u8 shakeTimeCnt =0;
	if(learnKeyStatus == KEY_STATUS_PRESSING)
	{
		if(LEARN_KEY_PRESSED)
		{	
			shakeTimeCnt++;
			if(shakeTimeCnt == ANTI_SHAKE_TIME)
			{
				learnKeyStatus = KEY_STATUS_REAL_PRESS;
				shakeTimeCnt = 0;
			}	
		}
		else
		{
			learnKeyStatus = KEY_STATUS_IDLE;
			shakeTimeCnt = 0;
		}
	}
}
void AppKeyDialSwitchInit()
{
	OsKeyInit();
	OsDialSwitchInit();
	t_systmr_insertQuickTask(AppSanKeyTask_10ms, 10, OSTMR_PERIODIC);
	learnKeyStatus = KEY_STATUS_IDLE;
	nodeAddrValue = OsGetNodeAddr(); 
}
void AppKeyProcess()
{
	u8 __temp[3] = {0xaa,0xbb,0xcc};
	if(learnKeyStatus == KEY_STATUS_REAL_PRESS)
	{
		u8 learningCode[3]={0,0,0};
		learningCode[0] = OsGetSmartSocketAddr();
		learningCode[1] = OsGetSmartControlSwitchValue();
		learningCode[2] = OsGetNodeAddr(); 
		if((learningCode[0]==0)&&(learningCode[1]==0)&&(learningCode[2]==0))  //����ʱ������ȫ����·�ŷ���rf�룬ȫ����·ֵΪ0��
		{
			Rf315SendMsg((u8 *)&__temp[0]);
			learnKeyStatus = KEY_STATUS_IDLE;
		}
		else
		{
			//�������ó�������״̬��λ
			learnKeyStatus = KEY_STATUS_IDLE;
		}	
	}
  else if(learnKeyStatus == KEY_STATUS_IDLE)
	{
		if(LEARN_KEY_PRESSED)
		{
			learnKeyStatus = KEY_STATUS_PRESSING;
		}
	}
	else
	{//�ж������
	}
}

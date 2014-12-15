#include "stdio.h"
#include "string.h"
#include "ostmr.h"
#include "osqtmr.h"
#include "osled.h"
#include "osiwdg.h"
#include "appsensor.h"
#include "appkey.h"
#include "apprf315tx.h"
#include "appusart.h"
#include "appusarttest.h"

//#define USE_WATCHDOG

static void AppInit(void);

int main()
{
	AppInit();
	#ifdef DEBUG_PRINTF
  printf("start program \r\n");
	#endif
	while(1)
	{
		AppSensorProcess();
		AppKeyProcess();	
		AppUsartProcess();
		//AppUsart1Process();		//debug usart1's process
		#ifdef 	USE_WATCHDOG
		IwdgFeed();
		#endif
	}
}

static void AppInit()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组
	b_ostmr_init();   //多定时器任务10ms
	b_systmr_init();  //多定时器任务1ms
	OsRf315Init();      
	//delay_init(72);	 
	OsLedInit(); 
	AppUsartInit();
	AppUsart1Init();  
 	AppSensorInit();
	AppKeyDialSwitchInit();
	#ifdef USE_WATCHDOG
	IwdgInit();
	#endif
}	


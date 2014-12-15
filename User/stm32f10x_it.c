/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
	
	/*
*********************************************************************************************************                            
*	ģ������ : �ж�ģ��
*	�ļ����� : stm32f10x_it.c
*	˵    �� : ���ļ�������е��жϷ�������Ϊ�˱��������˽�����õ����жϣ����ǲ����齫�жϺ����Ƶ�����
*			���ļ���
*			
*			����ֻ��Ҫ�����Ҫ���жϺ������ɡ�һ���жϺ������ǹ̶��ģ��������޸��������ļ���
*				Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\startup_stm32f10x_hd.s
*			
*			�����ļ��ǻ�������ļ�������ÿ���жϵķ���������Щ����ʹ����WEAK �ؼ��֣���ʾ�����壬�����
*			��������c�ļ����ض����˸÷��������������ͬ��������ô�����ļ����жϺ������Զ���Ч����Ҳ��
*			�����ض���ĸ�����C++�еĺ������ص��������ơ�
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define USE_USART2
//#define	USE_USART1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void _TIM2_IRQHandler(void);
extern void _TIM3_IRQHandler(void);
extern void _TIM4_IRQHandler(void);
//#ifdef USE_USART2
extern void _USART2_IRQHandler(void);
//#else
extern void _USART1_IRQHandler(void);
//#endif

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//#ifdef USE_USART2
void USART2_IRQHandler(void)
{
	_USART2_IRQHandler();
}
//#else
void USART1_IRQHandler(void)
{
  _USART1_IRQHandler();
}
//#endif
void TIM2_IRQHandler(void)
{
	_TIM2_IRQHandler();
}
void TIM3_IRQHandler(void)
{
	_TIM3_IRQHandler();
}
void TIM4_IRQHandler(void)
{
	_TIM4_IRQHandler();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/* 
	��Ϊ�жϷ�����������;����Ӧ���йأ����õ��û�����ģ��ı���������������ڱ��ļ�չ���������Ӵ�����
	�ⲿ������������include��䡣
	
	��ˣ������Ƽ�����ط�ֻдһ��������䣬�жϷ������ı���ŵ���Ӧ���û�����ģ���С�
	����һ����ûή�ʹ����ִ��Ч�ʣ�����������Ը��ʧ���Ч�ʣ��Ӷ���ǿ�����ģ�黯���ԡ�
	
	����extern�ؼ��֣�ֱ�������õ����ⲿ�������������ļ�ͷinclude����ģ���ͷ�ļ�
extern void ppp_ISR(void);	
void PPP_IRQHandler(void)
{
	ppp_ISR();
}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

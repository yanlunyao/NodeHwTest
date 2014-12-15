#include "string.h"
#include "stdio.h"
#include "osrf315tx.h"
#include "appusart.h"
#include "appCommProtocolDef.h"
#include "appsensor.h"

static  rxBuffer_t       					_appRsRxBuf;
static  txBuffer_t       					_appRsTxBuf;		
static  rxProtocol_t       				*_pRcvProMsg = NULL;
//static  txProtocol_t       				*_pSndProMsg = NULL;
static  u8                        _txTempBuffer[ZIGBEE_INFO_LENGTH+10];
static  CommStatus_t				_commStatus;
//------------------------------------------------------------------------------------//
void AppUsartInit()
{
	OsUsartInit();

  memset((u8 *)&_appRsRxBuf, 0x00, sizeof(_appRsRxBuf));
  memset((u8 *)&_appRsTxBuf, 0x00, sizeof(_appRsTxBuf));
  memset(_txTempBuffer, 0x00, sizeof(_txTempBuffer));
  _pRcvProMsg = &(_appRsRxBuf.buffer);
	_commStatus = COM_STATUS_IDLE;
}	
//------------------------------------------------------------------------------------//
void AppUsartProcess()
{
	u8 __temp[3] = {0xaa,0xbb,0xcc};
	u16 __bufferLen;
	
  switch(_commStatus)
  {
    case COM_STATUS_IDLE:
			if(_appRsTxBuf.status == TX_BUFFER_READY)
			{
				//有发送数据要发送
				_commStatus = COM_STATUS_SEND;
			}
			else
			{
				_commStatus = COM_STATUS_RECEIVE;
			}	
    break;
    case COM_STATUS_SEND:  //主动发才会进入
			__bufferLen = _appRsTxBuf.dataLen;
		  memcpy(_txTempBuffer, &(_appRsTxBuf.buffer), __bufferLen);
		  t_osscomm_sendMessage(_txTempBuffer, __bufferLen, USART2_COM);
		  memset(&(_appRsTxBuf.buffer), 0, sizeof(_appRsTxBuf.buffer));
		  _appRsTxBuf.status = TX_BUFFER_EMPTY; //发送完成后置
			_commStatus = COM_STATUS_RECEIVE;
    break;
    case COM_STATUS_RECEIVE:
			if(t_osscomm_ReceiveMessage((u8 *)_pRcvProMsg, (u16 *)&_appRsRxBuf.dataLen, USART2_COM) == SCOMM_RET_OK)
			{
				if((_appRsRxBuf.buffer.cmdNum == 0xaa)&&(_appRsRxBuf.buffer.cmdType == 0xbb)&&(_appRsRxBuf.buffer.cmdString[0] == 0xcc)&&(_appRsRxBuf.buffer.cmdString[1] == 0xdd))
				{	
					Rf315SendMsg((u8 *)&__temp[0]);
					_commStatus = COM_STATUS_IDLE;
				}
				else
				{	
					_commStatus = COM_STATUS_IDLE;
				}
				memset((u8 *)&_appRsRxBuf, 0x00, sizeof(_appRsRxBuf));
			}
			else
			{
				_commStatus = COM_STATUS_IDLE;
			}	
    break;
    default:
      _commStatus = COM_STATUS_IDLE;
    break;
  }
}	

#include "SerialCmdTask.h"


extern volatile int next_msg_length;
static bool process_msg(unsigned char *aRxBuffer, int msg_length)
{
	ProcessedMessage response;
	if (ProtoIngestor.process(&response, aRxBuffer, msg_length))
	{
		next_msg_length = HEADER_LENGTH;														
		if (response.type == T_RetrivalStatus)
		{
			SystemState.set_status(&response.retrival_status);
			if (response.retrival_status.status == RetrivalStatus_StatusType_FLUSH)
			{
				xTaskNotify(drawTaskHandle, 0x00, eSetBits);
			}
			else if (response.retrival_status.status == RetrivalStatus_StatusType_UPDATE)
			{						
				xTaskNotify(drawTaskHandle, 0x01, eSetBits);
			}
			else if (response.retrival_status.status == RetrivalStatus_StatusType_FAIL)
			{						
				xTaskNotify(drawTaskHandle, 0x02, eSetBits);
			}
		}
		else if (response.type == T_Header)
		{
			next_msg_length = response.header.length;
		}				
		else if (response.type == T_Weather)
		{									
			SystemState.add_weather(&response.weather);							
		}
		else if (response.type == T_Meeting)
		{					
			SystemState.add_meeting(&response.meeting);					
		}
		else if (response.type == T_Todo)
		{
					
			SystemState.add_todo(&response.todo);				
		}		
		else if (response.type == T_Time)
		{			
					
			SystemState.set_time(&response.time);				
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
#ifdef INCLUDE_uxTaskGetStackHighWaterMark
				SerialCmdTask_Watermark = uxTaskGetStackHighWaterMark(NULL);
#endif
	return true;
}
void StartSerialCmdTask(void const * argument) {	
	unsigned char aRxBuffer[MAX_I2C_BUFFER];
	next_msg_length = HEADER_LENGTH;
	//todo maybe make this a switch so multiple actiosn including failre handling can happen here
	while(1) {		
		if (osMutexWait(I2CSerialCommandMuxHandle, 1000) == osOK)
		{
			if (I2CSerialCommandAdapter.next(aRxBuffer, next_msg_length, serialCmdTaskHandle) == false)
			{
				Error_Handler();   
			}		
			xTaskNotifyWait(pdFALSE, ULONG_MAX, NULL, osWaitForever);
			if (osMutexRelease(I2CSerialCommandMuxHandle) != osOK) {
				Error_Handler();   					
			}
			if (process_msg(aRxBuffer, next_msg_length) == false)
			{
				Error_Handler();   
			}
		}
	}
}

static void start() {
//	xTaskNotify(serialCmdTaskHandle, 0x01, eSetBits);
}
const struct serialcmdtask SerialCmdTask = { 
	.start = start,		
};
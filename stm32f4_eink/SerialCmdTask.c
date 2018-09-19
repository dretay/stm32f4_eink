#include "SerialCmdTask.h"


void StartSerialCmdTask(void const * argument) {	
	static int expected_msg_length = HEADER_LENGTH;
	xTaskNotifyWait(pdFALSE, ULONG_MAX, NULL, osWaitForever);
	
	while(1) {	
		ProcessedMessage response;
		unsigned char aRxBuffer[MAX_I2C_BUFFER];
		if (I2CSerialCommandAdapter.next(aRxBuffer, expected_msg_length))
		{
			if (ProtoIngestor.process(&response, aRxBuffer, expected_msg_length))
			{
				if (response.type == T_RetrivalStatus)
				{
					SystemState.set_status(&response.retrival_status);
					if (response.retrival_status.status == RetrivalStatus_StatusType_FLUSH)
					{
						expected_msg_length = HEADER_LENGTH;											
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
					expected_msg_length = response.header.length;
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
					Error_Handler();   				
				}
			}
			else
			{
				Error_Handler();   				
			}
		}
		else
		{
			Error_Handler();   
		}
			
#ifdef INCLUDE_uxTaskGetStackHighWaterMark
				SerialCmdTask_Watermark = uxTaskGetStackHighWaterMark(NULL);
#endif

		osThreadYield();			
	}
}

static void start() {
	xTaskNotify(serialCmdTaskHandle, 0x01, eSetBits);
}
const struct serialcmdtask SerialCmdTask = { 
	.start = start,		
};
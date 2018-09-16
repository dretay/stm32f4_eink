#include "application.h"

void run(void)
{
	gfxInit();

//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, RESET);
//	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, SET);
	HAL_Delay(200);

	View *calendar_view = CalendarView.init();	
	View *status_view = StatusView.init();	
	View *battery_view = BatteryView.init();	

	while (true)
	{
		const pb_field_t* msg_type = ProtoIngestor.next();
		if (msg_type == RetrivalStatus_fields)
		{
			if (SystemState.status->status == RetrivalStatus_StatusType_FLUSH)
			{
				calendar_view->render();			
			}
			else if (SystemState.status->status != RetrivalStatus_StatusType_START)
			{
				status_view->render();			
			}
		}
	}
}
int wake_counter = 0;
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (wake_counter % 2 == 0)
	{		
		//todo: someday this will be where the rpi gets woken up
	}
	wake_counter++;
}
//todo: why is this firing for i2c dma abort ???
void FLASH_IRQHandler()
{
	
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
//	Error_Handler();   
}
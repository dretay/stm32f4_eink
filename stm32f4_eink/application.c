#include "application.h"

static void run(void)
{	
	SerialCmdTask.start();
	DrawTask.start();
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


const struct application Application = { 
	.run = run,		
};
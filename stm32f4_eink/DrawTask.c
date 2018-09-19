#include "DrawTask.h"

static View *calendar_view;
static View *status_view; 
static View *battery_view;

void StartDrawTask(void const * argument) {	
	uint32_t ulNotifiedValue;
	calendar_view = CalendarView.init();	
	status_view = StatusView.init();	
	battery_view = BatteryView.init();	

	GDisplay *g = gdispGetDisplay(0);
	while (1) {	
#ifdef INCLUDE_uxTaskGetStackHighWaterMark
		DrawTask_Watermark = uxTaskGetStackHighWaterMark(NULL);
#endif

		xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotifiedValue, osWaitForever);
		if ((ulNotifiedValue & 0x00) != 0)
		{
			calendar_view->render();			
		}
		else if ((ulNotifiedValue & 0x01) != 0)
		{
			status_view->render();			
		}
		else if ((ulNotifiedValue & 0x02) != 0)
		{
			battery_view->render();			
		}

#ifdef INCLUDE_uxTaskGetStackHighWaterMark
		DrawTask_Watermark = uxTaskGetStackHighWaterMark(NULL);
#endif

		osThreadYield();			
	}
}

static void start() {

}
const struct drawtask DrawTask = { 
	.start = start,		
};
#pragma once
#include "cmsis_os.h"
#include <limits.h>

#include "CalendarView.h"
#include "StatusView.h"
#include "BatteryView.h"

#ifdef INCLUDE_uxTaskGetStackHighWaterMark
UBaseType_t DrawTask_Watermark;
#endif

extern osThreadId drawTaskHandle;

struct drawtask {	
	void(*start)(void);		
};

extern const struct drawtask DrawTask;
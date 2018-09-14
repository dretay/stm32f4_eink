#pragma once

#include "time.h"
#include "stm32f4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

struct datehelper
{
	time_t(*get_epoch)(void);
	char*(*get_day_of_week)(void);
	char*(*get_month)();	
	int(*get_year)();
};
extern const struct datehelper DateHelper;
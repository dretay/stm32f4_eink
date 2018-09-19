#pragma once

#include "stm32f4xx_hal.h"
#include "union.pb.h"

#define MAX_MEETINGS 15
#define MAX_TODOS 6
#define MAX_WEATHERS 20

extern RTC_HandleTypeDef hrtc;

struct systemstate
{
	Meeting *meetings;
	Todo *todos;
	Weather *weathers;
	RetrivalStatus *status;
	int *meetings_cnt;
	int *todos_cnt;
	int *weathers_cnt;
	void(*reset)(void);
	bool(*add_weather)(Weather *weather);
	bool(*add_meeting)(Meeting *meeting);
	bool(*add_todo)(Todo *todo);
	bool(*set_time)(Time *time);
	bool(*set_status)(RetrivalStatus *status_in);
};

extern volatile const struct systemstate SystemState;
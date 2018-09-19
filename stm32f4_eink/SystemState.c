#include "SystemState.h"

static int meetings_cnt = 0;
static int todos_cnt = 0;
static int weathers_cnt = 0;

static Meeting meetings[MAX_MEETINGS];
static Todo todos[MAX_TODOS];
static Weather weathers[MAX_WEATHERS];
static RetrivalStatus status;

static void reset(void)
{
	memset(meetings, '\0', sizeof(Meeting) * MAX_MEETINGS);		
	memset(todos, '\0', sizeof(Todo) * MAX_TODOS);		
	memset(weathers, '\0', sizeof(Weather) * MAX_WEATHERS);		
	meetings_cnt = 0;
	todos_cnt = 0;
	weathers_cnt = 0;
}
static bool add_weather(Weather *weather)
{
	if (weather->idx < MAX_WEATHERS)
	{
		memcpy(&weathers[weather->idx], weather, sizeof(Weather));		
		weathers_cnt++;
		return true;
	}				
	return false;
}
static bool add_meeting(Meeting *meeting)
{
	if (meeting->idx < MAX_MEETINGS)
	{
		memcpy(&meetings[meeting->idx], meeting, sizeof(Meeting));				
		meetings_cnt++;					
		return true;
	}			
	return false;
}
static bool add_todo(Todo *todo)
{
	if (todo->idx < MAX_TODOS)
	{
		memcpy(&todos[todo->idx], todo, sizeof(Todo));				
		todos_cnt++;				
		return true;
	}
	return false;
}
static bool set_status(RetrivalStatus *status_in)
{	
	memcpy(&status, status_in, sizeof(RetrivalStatus));
	if (status.status == RetrivalStatus_StatusType_START)
	{
		reset();					
	}
	else if (status.status == RetrivalStatus_StatusType_FLUSH)
	{
				
	}
}
static bool set_time(Time *time)
{
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;

	sdatestructure.Year = time->year;
	sdatestructure.Month = time->month;
	sdatestructure.Date = time->date;
	sdatestructure.WeekDay = time->weekday;
  
	if (HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK)
	{
		return false;
	}

	stimestructure.Hours = time->hours;
	stimestructure.Minutes = time->minutes;
	stimestructure.Seconds = time->seconds;
	stimestructure.TimeFormat = time->am ? RTC_HOURFORMAT12_AM : RTC_HOURFORMAT12_PM;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
	{
		return false;
	}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);

	return true;
}

volatile const struct systemstate SystemState = { 
	.meetings = meetings,
	.todos = todos,
	.weathers = weathers,
	.reset = reset,
	.status = &status,
	.meetings_cnt = &meetings_cnt,
	.todos_cnt = &todos_cnt,
	.weathers_cnt = &weathers_cnt,
	.add_weather = add_weather,
	.add_meeting = add_meeting,
	.add_todo = add_todo,
	.set_time = set_time,
	.set_status = set_status
};
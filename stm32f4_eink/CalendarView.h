#pragma once

#include "View.h"
#include <string.h>
#include "union.pb.h"
#include "math.h"
#include "time.h"
#include "datehelper.h"

extern RTC_HandleTypeDef hrtc;

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct calendarview
{
	View*(*init)(void);	
};
extern const struct calendarview CalendarView;
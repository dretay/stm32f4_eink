#pragma once

#include "View.h"

struct batteryview
{
	View*(*init)(void);	
};
extern const struct batteryview BatteryView;

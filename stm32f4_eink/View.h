#pragma once

#include "stm32f4xx_hal.h"
#include "gfx.h"
#include "SystemState.h"


typedef struct {
	void(*render)(void);		
} View;
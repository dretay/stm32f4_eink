#pragma once

#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "DrawTask.h"
#include "SerialCmdTask.h"


struct application {	
	void(*run)(void);		
};

extern const struct application Application;
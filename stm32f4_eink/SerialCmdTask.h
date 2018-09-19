#pragma once
#include "cmsis_os.h"
#include <limits.h>

#include "ProtoIngestor.h"
#include "I2CSerialCommandAdapter.h"

#define HEADER_LENGTH 4
#define MAX_I2C_MSG 31
#define MAX_I2C_BUFFER 128

extern osThreadId drawTaskHandle;	         

#ifdef INCLUDE_uxTaskGetStackHighWaterMark
UBaseType_t SerialCmdTask_Watermark;
#endif

extern osThreadId serialCmdTaskHandle;

struct serialcmdtask {	
	void(*start)(void);		
};

extern const struct serialcmdtask SerialCmdTask;
#pragma once

#include "stm32f4xx_hal.h"
#include "stdbool.h"

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;

struct i2cserialcommandadapter
{
	const bool(*next)(unsigned char *aRxBuffer, int expected_msg_length);
};

extern const struct i2cserialcommandadapter I2CSerialCommandAdapter;
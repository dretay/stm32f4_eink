#pragma once
#include <pb.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include <stdlib.h>

//TODO: can the multipart stuff be broken out into its own file? can you span or inherit from other files?
#include "union.pb.h"

#include "stm32f4xx_hal.h"
#include "eink.h"

#define MAX_I2C_MSG 31
#define MAX_I2C_BUFFER 128
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern RTC_HandleTypeDef hrtc;


void run(void);
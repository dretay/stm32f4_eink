#pragma once

#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include "union.pb.h"
#include "SystemState.h"

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;


struct protoingestor
{
	const pb_field_t*(*next)();
};

extern const struct protoingestor ProtoIngestor;
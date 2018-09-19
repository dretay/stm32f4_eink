#pragma once

#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include "union.pb.h"
#include "SystemState.h"

typedef struct
{
	enum { T_Header, T_RetrivalStatus, T_Weather, T_Meeting, T_Todo, T_Time } type;
	union
	{
		Header header;
		RetrivalStatus retrival_status;
		Weather weather;
		Meeting meeting;
		Todo todo;
		Time time;
	};
}ProcessedMessage;

struct protoingestor
{
	bool(*process)(ProcessedMessage *response, unsigned  char *aRxBuffer, int expected_msg_length);
};

extern const struct protoingestor ProtoIngestor;
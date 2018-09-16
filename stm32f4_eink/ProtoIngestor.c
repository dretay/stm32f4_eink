#include "ProtoIngestor.h"

static const pb_field_t* decode_unionmessage_type(pb_istream_t *stream)
{
	pb_wire_type_t wire_type;
	uint32_t tag;
	bool eof;
	while (pb_decode_tag(stream, &wire_type, &tag, &eof))
	{
		if (wire_type == PB_WT_STRING)
		{
			const pb_field_t *field;
			for (field = UnionMessage_fields; field->tag != 0; field++)
			{
				if (field->tag == tag && (field->type & PB_LTYPE_SUBMESSAGE))
				{
					/* Found our field. */
					return field->ptr;
				}
			}
		}
        
		/* Wasn't our field.. */
		pb_skip_field(stream, wire_type);
	}
    
	return NULL;
}

static bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
{
	pb_istream_t substream;
	bool status;
	if (!pb_make_string_substream(stream, &substream))
		return false;
    
	status = pb_decode(&substream, fields, dest_struct);
	pb_close_string_substream(stream, &substream);
	return status;
}


#define HEADER_LENGTH 4
static int expected_msg_length = HEADER_LENGTH;
#define MAX_I2C_MSG 31
#define MAX_I2C_BUFFER 128
pb_byte_t aRxBuffer[MAX_I2C_BUFFER];

static const pb_field_t* process(void)
{
	pb_istream_t stream = pb_istream_from_buffer(aRxBuffer, expected_msg_length);
	const pb_field_t *type = decode_unionmessage_type(&stream);
	
	if (type == Header_fields)
	{
		Header header;
		if (decode_unionmessage_contents(&stream, Header_fields, &header))
		{				
			expected_msg_length = header.length;
		}
		else
		{
			Error_Handler();   
		}
	}
	else if (type == RetrivalStatus_fields)
	{
		RetrivalStatus status;
		if (decode_unionmessage_contents(&stream, RetrivalStatus_fields, &status))
		{				
			SystemState.set_status(&status);
			if(status.status == RetrivalStatus_StatusType_FLUSH)
			{
				expected_msg_length = HEADER_LENGTH;
			}
		}
		else
		{
			Error_Handler();   
		}

	}
	else if (type == Weather_fields)
	{
		Weather weather;
		if (decode_unionmessage_contents(&stream, Weather_fields, &weather))
		{				
			SystemState.add_weather(&weather);			
		}
		else
		{
			Error_Handler();   
		}
	}
	else if (type == Meeting_fields)
	{
		Meeting meeting;
		if (decode_unionmessage_contents(&stream, Meeting_fields, &meeting))
		{			
			SystemState.add_meeting(&meeting);
		}
		else
		{
			Error_Handler();   
		}
	}
	else if (type == Todo_fields)
	{
		Todo todo;
		if (decode_unionmessage_contents(&stream, Todo_fields, &todo))
		{
			SystemState.add_todo(&todo);
		}
		else
		{
			Error_Handler();   
		}
	}		
	else if (type == Time_fields)
	{			
		Time time;
		if (decode_unionmessage_contents(&stream, Time_fields, &time))
		{	
			SystemState.set_time(&time);
		}
		else
		{
			Error_Handler();   
		}
	}
	else
	{
		//this can happen if we've decided to chunk xmit.. should probably have some 
		//better handling around this... maybe do something special if we overflow
	}	
	return type;
}


static void reset_rx_buffer()
{
	HAL_DMA_Abort(&hdma_i2c1_rx);		
}
static void wait_for_rx()
{
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{		
	} 	
}
static const pb_field_t* next(void)
{	
	if (HAL_I2C_Slave_Receive_DMA(&hi2c1, aRxBuffer, expected_msg_length) != HAL_OK)
	{
		/* Transfer error in reception process */
		Error_Handler();        
	}
	wait_for_rx();
	reset_rx_buffer();
	return process();
	
}

const struct protoingestor ProtoIngestor = {
	.next = next,
};
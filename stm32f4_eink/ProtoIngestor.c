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

static bool process(ProcessedMessage *response, unsigned  char *aRxBuffer, int expected_msg_length)
{
	pb_istream_t stream = pb_istream_from_buffer(aRxBuffer, expected_msg_length);
	const pb_field_t *type = decode_unionmessage_type(&stream);
	
	if (type == Header_fields)
	{
		Header header;
		if (decode_unionmessage_contents(&stream, Header_fields, &header))
		{				
			memcpy(&response->header, &header, sizeof(Header));
			response->type = T_Header;
			return true;
		}
		else
		{
			Error_Handler();   
		}
	}
	else if (type == RetrivalStatus_fields)
	{
		RetrivalStatus retrival_status;
		if (decode_unionmessage_contents(&stream, RetrivalStatus_fields, &retrival_status))
		{				
			memcpy(&response->retrival_status, &retrival_status, sizeof(RetrivalStatus));
			response->type = T_RetrivalStatus;
			return true;
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
			memcpy(&response->weather, &weather, sizeof(Weather));
			response->type = T_Weather;
			return true;
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
			memcpy(&response->meeting, &meeting, sizeof(Meeting));
			response->type = T_Meeting;
			return true;
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
			memcpy(&response->todo, &todo, sizeof(Todo));
			response->type = T_Todo;
			return true;
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
			memcpy(&response->time, &time, sizeof(Time));
			response->type = T_Time;
			return true;
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
		return false;
	}	
	return false;
}


const struct protoingestor ProtoIngestor = {
	.process = process,
};
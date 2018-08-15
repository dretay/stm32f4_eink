#include "application.h"

const pb_field_t* decode_unionmessage_type(pb_istream_t *stream)
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
bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
{
	pb_istream_t substream;
	bool status;
	if (!pb_make_string_substream(stream, &substream))
		return false;
    
	status = pb_decode(&substream, fields, dest_struct);
	pb_close_string_substream(stream, &substream);
	return status;
}


uint8_t aRxBuffer[MAX_I2C_BUFFER];


Meeting meetings[6];
//not sure this is legit...
void reset_rx_buffer()
{
	memset(aRxBuffer, '\0', sizeof(char) * MAX_I2C_MSG);
	HAL_DMA_Abort(&hdma_i2c1_rx);		
}
void run(void)
{
	memset(aRxBuffer, '\0', sizeof(char) * MAX_I2C_MSG);
	while (true)
	{
		//this doesn't really need dma sice we're not doing anything while we wait
		if (HAL_I2C_Slave_Receive_DMA(&hi2c1, (uint8_t *)aRxBuffer, MAX_I2C_BUFFER) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();        
		}
  

		while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
		{
		} 
	
		int in_byte_cnt = strcspn(&aRxBuffer, "\0")+1;
		pb_istream_t stream = pb_istream_from_buffer(aRxBuffer, in_byte_cnt);
		const pb_field_t *type = decode_unionmessage_type(&stream);
		
		if (type == RetrivalStatus_fields)
		{
			reset_rx_buffer();

		}
		else if (type == Meeting_fields)
		{
			Meeting meeting;
			if (decode_unionmessage_contents(&stream, Meeting_fields, &meeting))
			{
				memcpy(&meetings[meeting.idx], &meeting, sizeof(Meeting));				
				reset_rx_buffer();
			}
			else
			{
				Error_Handler();   
			}
		}
		else if (type == Weather_fields)
		{
			reset_rx_buffer();
		
		}
		else
		{
			//this can happen if we've decided to chunk xmit.. should probably have some 
			//better handling around this... maybe do something special if we overflow
		}		
	}
}
//todo: why is this firing for i2c dma abort ???
void FLASH_IRQHandler()
{
	
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
	Error_Handler();   
}
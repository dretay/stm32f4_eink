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

#define MAX_MEETINGS 15
Meeting meetings[MAX_MEETINGS];
int meetings_cnt = 0;

#define MAX_TODOS 6
Todo todos[MAX_TODOS];
int todos_cnt = 0;

#define MAX_WEATHERS 20
Weather weathers[MAX_WEATHERS];
int weathers_cnt = 0;

#define HEADER_LENGTH 4
//not sure this is legit...
void reset_rx_buffer()
{
	HAL_DMA_Abort(&hdma_i2c1_rx);		
}
void wait_for_rx()
{
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{		
	} 	
}
void rx_message(int expected_msg_length)
{
	if (HAL_I2C_Slave_Receive_DMA(&hi2c1, (uint8_t *)aRxBuffer, expected_msg_length) != HAL_OK)
	{
		/* Transfer error in reception process */
		Error_Handler();        
	}
  
	wait_for_rx();

	pb_istream_t stream = pb_istream_from_buffer(aRxBuffer, expected_msg_length);
	const pb_field_t *type = decode_unionmessage_type(&stream);
	if (type == RetrivalStatus_fields)
	{
		RetrivalStatus status;
		if (decode_unionmessage_contents(&stream, RetrivalStatus_fields, &status))
		{				
			if (status.status == RetrivalStatus_StatusType_START)
			{
				memset(meetings, '\0', sizeof(Meeting) * MAX_MEETINGS);		
				memset(todos, '\0', sizeof(Todo) * MAX_TODOS);		
				memset(weathers, '\0', sizeof(Weather) * MAX_WEATHERS);		
				meetings_cnt = 0;
				todos_cnt = 0;
				weathers_cnt = 0;
			}
			else if (status.status == RetrivalStatus_StatusType_FLUSH)
			{
				CalendarView.draw(meetings, meetings_cnt, todos, todos_cnt, weathers, weathers_cnt);		
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
			if (weather.idx < MAX_WEATHERS)
			{
				memcpy(&weathers[weather.idx], &weather, sizeof(Weather));		
				weathers_cnt++;
			}				
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
			if (meeting.idx < MAX_MEETINGS)
			{
				memcpy(&meetings[meeting.idx], &meeting, sizeof(Meeting));				
				meetings_cnt++;					
			}
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
			if (todo.idx < MAX_TODOS)
			{
				memcpy(&todos[todo.idx], &todo, sizeof(Todo));				
				todos_cnt++;					
			}
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
			RTC_DateTypeDef sdatestructure;
			RTC_TimeTypeDef stimestructure;

			sdatestructure.Year = time.year;
			sdatestructure.Month = time.month;
			sdatestructure.Date = time.date;
			sdatestructure.WeekDay = time.weekday;
  
			if (HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK)
			{
				/* Initialization Error */
				Error_Handler();
			}

			stimestructure.Hours = time.hours;
			stimestructure.Minutes = time.minutes;
			stimestructure.Seconds = time.seconds;
			stimestructure.TimeFormat = time.am ? RTC_HOURFORMAT12_AM : RTC_HOURFORMAT12_PM;
			stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
			stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

			if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
			{
				/* Initialization Error */
				Error_Handler();
			}

			HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
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
}
void run(void)
{

	CalendarView.init();	
	while (true)
	{
		//this doesn't really need dma sice we're not doing anything while we wait
		if(HAL_I2C_Slave_Receive_DMA(&hi2c1, (uint8_t *)aRxBuffer, HEADER_LENGTH) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();        
		}
  

		wait_for_rx();
		
		pb_istream_t stream = pb_istream_from_buffer(aRxBuffer, HEADER_LENGTH);
		const pb_field_t *type = decode_unionmessage_type(&stream);
		
		if (type == Header_fields)
		{
			Header header;
			if (decode_unionmessage_contents(&stream, Header_fields, &header))
			{				
				rx_message(header.length);
			}
			else
			{
				Error_Handler();   
			}
		}
		reset_rx_buffer();
	}
}
int wake_counter = 0;
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (wake_counter % 2 == 0)
	{		
		//todo: someday this will be where the rpi gets woken up
	}
	wake_counter++;
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
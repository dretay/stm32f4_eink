#include "I2CSerialCommandAdapter.h"

static osThreadId *thread_handle;
static const bool next(unsigned char *aRxBuffer, int expected_msg_length, osThreadId *thread_handle_in)
{		
	thread_handle = thread_handle_in;
	HAL_DMA_Abort_IT(&hdma_i2c1_rx);		
	if (HAL_I2C_Slave_Receive_DMA(&hi2c1, aRxBuffer, expected_msg_length) != HAL_OK)
	{
		return false;
	}		
	return true;			
	
}

const struct i2cserialcommandadapter I2CSerialCommandAdapter = {
	.next = next,
};

void FLASH_IRQHandler()
{
	asm("nop");
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//todo: can notify differently on success and failure
	xTaskNotifyFromISR(thread_handle, 0x00, eSetBits, &xHigherPriorityTaskWoken);	
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
	Error_Handler();   
}
#include "I2CSerialCommandAdapter.h"

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

static const bool next(unsigned char *aRxBuffer, int expected_msg_length)
{	
	if (HAL_I2C_Slave_Receive_DMA(&hi2c1, aRxBuffer, expected_msg_length) != HAL_OK)
	{
		return false;
	}
	wait_for_rx();
	reset_rx_buffer();
	return true;	
}

const struct i2cserialcommandadapter I2CSerialCommandAdapter = {
	.next = next,
};

//todo: do i still need these?
void FLASH_IRQHandler()
{
	
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
	//	Error_Handler();   
}
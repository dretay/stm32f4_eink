#pragma once


#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOA
#define BUSY_Pin GPIO_PIN_3
#define BUSY_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA

#define COLORED      1
#define UNCOLORED    0

#include "stm32f4xx_hal.h"
#include "gfx.h"


void run(void);
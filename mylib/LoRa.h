#ifndef __LORA_H
#define __LORA_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"

/*This library design for module E32*/

typedef enum
{
	UART1 = 1,
	UART2 = 2,
	UART3 = 3,
} UART_num;

typedef enum 
{
	mode0 = 0,
	mode1 = 1,
	mode2 = 2,
	mode3 = 3,
} E32_mode;

int LORA_Init(UART_num UARTx,UART_HandleTypeDef *s_UARTHandle);
void Lora_SetMode (GPIO_TypeDef *GPIOx, E32_mode mode);
void Lora_transmit ( UART_HandleTypeDef *s_UARTHandle, uint8_t *pData);
void clear_data(uint8_t *pData);
int Lora_Receive ( UART_HandleTypeDef *s_UARTHandle, uint8_t *pData);


#endif
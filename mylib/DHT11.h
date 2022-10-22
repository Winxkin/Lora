#ifndef __DHT11_H
#define __DHT11_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_rcc.h"
#define DHT11_SUCCESS         1
#define DHT11_ERROR_CHECKSUM  2
#define DHT11_ERROR_TIMEOUT   3

#define DHT11_PIN  GPIO_PIN_0
#define DHT11_PORT GPIOA

uint8_t DHT11_init(TIM_HandleTypeDef *ustim);
uint8_t DHT11_Read(TIM_HandleTypeDef *ustim, uint8_t *temparature, uint8_t *humidity);




#endif

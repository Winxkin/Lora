#ifndef __SOIL_MOISTURE_H
#define __SOIL_MOISTURE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"


/*This library design for module soil_moisture*/
/*A0 -> PA0 */
/*analog value 0->1023*/
uint8_t Soil_moisture_init_ADC1(ADC_HandleTypeDef *hadc1);
uint8_t Soil_moisture_Read(ADC_HandleTypeDef *hadc1);
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif

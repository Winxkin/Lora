#include "Soil_moisture.h"


uint8_t Soil_moisture_init_ADC1(ADC_HandleTypeDef *hadc1)
{	
		__HAL_RCC_ADC1_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  ADC_ChannelConfTypeDef sConfig = {0};
		hadc1->Instance = ADC1;
		hadc1->Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc1->Init.ContinuousConvMode = DISABLE;
		hadc1->Init.DiscontinuousConvMode = DISABLE;
		hadc1->Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc1->Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc1->Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(hadc1) != HAL_OK)
  {
    return 0;
  }
	
	sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK)
  {
    return 0;
  }
	
	return 1;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t Soil_moisture_Read(ADC_HandleTypeDef *hadc1)
{
		uint32_t var;
		HAL_ADC_Start(hadc1);
		HAL_Delay(100);
	  var = HAL_ADC_GetValue(hadc1);
	  HAL_ADC_Stop(hadc1);
	  uint8_t humi = map(var,0,4095,1,100);
		return (100 - humi); // return humi % value
}

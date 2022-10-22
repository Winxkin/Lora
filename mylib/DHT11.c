#include "DHT11.h"

uint8_t DHT11_init(TIM_HandleTypeDef *ustim)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_TIM1_CLK_ENABLE(); //using TIM1
	
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

	
  ustim->Instance = TIM1;
  ustim->Init.Prescaler = 84;
  ustim->Init.CounterMode = TIM_COUNTERMODE_UP;
  ustim->Init.Period = 84000000-1;
  ustim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	ustim->Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	 if (HAL_TIM_Base_Init(ustim) != HAL_OK)
  {
    return 0;
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(ustim, &sClockSourceConfig) != HAL_OK)
  {
    return 0;
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(ustim, &sMasterConfig) != HAL_OK)
  {
    return 0;
  }
	HAL_TIM_Base_Start(ustim);
	
	//init GPIO 
	__GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Pin = DHT11_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
 
	return 1;
}

uint8_t DHT11_Read(TIM_HandleTypeDef *ustim, uint8_t *temparature, uint8_t *humidity)
{

	uint8_t i, j , _temp;
	uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = DHT11_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	
	//Put LOW for at least 18ms
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
	
	
	//wait 18ms
	
	
	ustim->Instance->CNT=0;
	while((ustim->Instance->CNT) <= 18000);
	
	//Put HIGH for 20-40us
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
	
	//wait 40us
	ustim->Instance->CNT=0;
	while((ustim->Instance->CNT) <= 40);
	//End start condition
	
	//io();
	//Input mode to receive data
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	
	//DHT11 ACK
	//should be LOW for at least 80us
	//while(!GPIO_ReadInputDataBit(dev->port, dev->pin));
	ustim->Instance->CNT=0;
	while(!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
		if(ustim->Instance->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//should be HIGH for at least 80us
	//while(GPIO_ReadInputDataBit(dev->port, dev->pin));		
	ustim->Instance->CNT=0;
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
		if(ustim->Instance->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//Read 40 bits (8*5)
	for(j = 0; j < 5; ++j) {
		for(i = 0; i < 8; ++i) {
			
			//LOW for 50us
			while(!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
			
			
			//Start counter
			ustim->Instance->CNT=0;
			
			//HIGH for 26-28us = 0 / 70us = 1
			while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
			
			
			//Calc amount of time passed
			_temp = ustim->Instance->CNT;
			
			//shift 0
			data[j] = data[j] << 1;
			
			//if > 30us it's 1
			if(_temp > 40)
				data[j] = data[j]+1;
		}
	}
	
	//verify the Checksum
	if(data[4] != (data[0] + data[2]))
		return DHT11_ERROR_CHECKSUM;
	
	//set data
	*temparature = data[2];
	*humidity = data[0];
	
	return DHT11_SUCCESS;

}

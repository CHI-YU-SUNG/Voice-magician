 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET);
	HAL_Delay(125);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_Delay(125);
	
	
	
	void HAL_GPIO_EXIT_Callback(unint16_t GPIO_PIN){
		IF(GPIO_PIN==B1_PIN){
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
		}
	}

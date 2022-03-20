/**
 *****************************************************************************
 * @file	drv8825.c
 * @author 	Arnaud C.
 * @brief	drv8825 library for STM32
 *****************************************************************************
 */

#include "drv8825.h"

/* Prototypes ---------------------------------------------------------------*/
void _drv8825_setStepFrequency(drv8825* drv8825, uint16_t freq);

/* Functions ----------------------------------------------------------------*/

/**
 * drv8825_init: structure initialization
 * @param drv8825 		drv8825 structure
 * @param Dir_GPIOx 	GPIO port
 * @param Dir_GPIO_Pin 	GPIO pin
 * @param En_GPIOx 		GPIO port
 * @param En_GPIO_Pin 	GPIO pin
 * @param htim 			TIM_HandleTypeDef
 * @param channel 		TIM Channel used
 */
void drv8825_init(drv8825 *drv8825,
		GPIO_TypeDef* Dir_GPIOx, uint16_t Dir_GPIO_Pin,
		GPIO_TypeDef * En_GPIOx, uint16_t En_GPIO_Pin,
		TIM_HandleTypeDef* htim, uint32_t channel)
{
	// Dir
	drv8825->Dir_GPIOx = Dir_GPIOx;
	drv8825->Dir_GPIO_Pin = Dir_GPIO_Pin;

	// En
	drv8825->En_GPIOx = En_GPIOx;
	drv8825->En_GPIO_Pin = En_GPIO_Pin;

	// Step
	drv8825->Timer_Handle = htim;
	drv8825->Timer_Channel = channel;

	// Default configuration
	drv8825_setMode(drv8825, MS_RES_32);
	drv8825_setDir(drv8825, DIR_FORWARD);
	drv8825_setEn(drv8825, EN_STOP);
}

/**
 * drv8825_setMode: set drv8825 mode
 * @param drv8825 	drv8825 structure
 * @param mode		mode set to M0, M1 and M2 pins
 * @return 0 if mode is valid
 */
uint8_t drv8825_setMode(drv8825 *drv8825, uint8_t mode)
{
	switch(mode)
	{
		case MS_RES_1:
			drv8825->mode = MS_RES_1;
			break;
		case MS_RES_2:
			drv8825->mode = MS_RES_2;
			break;
		case MS_RES_4:
			drv8825->mode = MS_RES_4;
			break;
		case MS_RES_8:
			drv8825->mode = MS_RES_8;
			break;
		case MS_RES_16:
			drv8825->mode = MS_RES_16;
			break;
		case MS_RES_32:
			drv8825->mode = MS_RES_32;
			break;
		default:
			drv8825->mode = MS_RES_32;
			return -1;
	}

	return 0;
}

/**
 * drv8825_setDir: set direction
 * @param drv8825	drv8825 structure
 * @param dir_state	direction state
 */
void drv8825_setDir(drv8825* drv8825, uint8_t dir_state){
	HAL_GPIO_WritePin(drv8825->Dir_GPIOx, drv8825->Dir_GPIO_Pin, dir_state);
}

/**
 * drv8825_setEn
 * @param drv8825	drv8825 structure
 * @param en_state	enable/disable state
 */
void drv8825_setEn(drv8825* drv8825,uint8_t en_state){
	HAL_GPIO_WritePin(drv8825->En_GPIOx, drv8825->En_GPIO_Pin, en_state);
}

/**
 * drv8825_setSpeedRPM
 * @param drv8825 drv8825 structure
 * @param rpm revolutions per minute
 */
void drv8825_setSpeedRPM(drv8825* drv8825, uint8_t rpm){
	float rps = (float)rpm / 60;
	uint16_t freq = STEPS_PER_REV * drv8825->mode * rps;
	_drv8825_setStepFrequency(drv8825, freq);
}

/**
 * _drv8825_setStepFrequency
 * @param drv8825 drv8825 structure
 * @param freq pwm frequency
 */
void _drv8825_setStepFrequency(drv8825* drv8825, uint16_t freq)
{
	// pwm_freq = (tim_clock)/{(PSC+1)(ARR+1)})

	uint16_t new_arr = TIMER_CLOCK / ((209 + 1) * freq);
	__HAL_TIM_SET_PRESCALER(drv8825->Timer_Handle, 209);
	__HAL_TIM_SET_AUTORELOAD(drv8825->Timer_Handle, new_arr);

	if (HAL_TIM_Base_Init(drv8825->Timer_Handle) != HAL_OK){
		Error_Handler();
	}

	if (HAL_TIM_PWM_Init(drv8825->Timer_Handle) != HAL_OK){
		Error_Handler();
	}

	HAL_TIM_PWM_Start(drv8825->Timer_Handle, drv8825->Timer_Channel);

	// Set duty cycle to 50%
	switch(drv8825->Timer_Channel)
	{
		case TIM_CHANNEL_1:
			drv8825->Timer_Handle->Instance->CCR1 =
					(50*drv8825->Timer_Handle->Init.Period)/100;
			break;

		case TIM_CHANNEL_2:
			drv8825->Timer_Handle->Instance->CCR2 =
					(50*drv8825->Timer_Handle->Init.Period)/100;
			break;

		case TIM_CHANNEL_3:
			drv8825->Timer_Handle->Instance->CCR3 =
					(50*drv8825->Timer_Handle->Init.Period)/100;
			break;

		case TIM_CHANNEL_4:
			drv8825->Timer_Handle->Instance->CCR4 =
					(50*drv8825->Timer_Handle->Init.Period)/100;
			break;
	}
}

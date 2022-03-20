/**
 *****************************************************************************
 * @file	drv8825.h
 * @author 	Arnaud C.
 * @brief	drv8825 library for STM32
 *****************************************************************************
 *
 	// In the IOC: TIM PWM with PSC = 209 and ARR = 799, Internal clock

 	// drv8825 structure creation
	drv8825 drv;

	// drv8825 structure initialization
	drv8825_init(&drv, Dir_G_GPIO_Port, Dir_G_Pin,
			En_G_GPIO_Port, En_G_Pin, &htim2, TIM_CHANNEL_1);

	// drv8825 mode
	drv8825_setMode(&drv, MS_RES_32);

	// drv8825 direction
	drv8825_setDir(&drv, DIR_FORWARD);

	// drv8825 enable
	drv8825_setEn(&drv, EN_START);

	// drv8825 speed
	drv8825_setSpeedRPM(&drv, 30);
 *
 *****************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef INC_DRV8825_H_
#define INC_DRV8825_H_

/* Includes -----------------------------------------------------------------*/
#include "tim.h"

/* Exported types -----------------------------------------------------------*/

typedef struct drv8825 {
	// GPIO Dir
    GPIO_TypeDef * Dir_GPIOx;
    uint16_t Dir_GPIO_Pin;


    // GPIO En
    GPIO_TypeDef * En_GPIOx;
    uint16_t En_GPIO_Pin;

    // TIM Step
    TIM_HandleTypeDef* Timer_Handle;
    uint32_t Timer_Channel;

    // Mode
    uint8_t mode;
} drv8825;

/* End of exported types ----------------------------------------------------*/

/* Exported macros ----------------------------------------------------------*/

#define STEPS_PER_REV	200
#define TIMER_CLOCK		84000000

// Mode macros (Microstep Resolution)
#define MS_RES_1	1
#define MS_RES_2	2
#define MS_RES_4	4
#define MS_RES_8	8
#define MS_RES_16	16
#define MS_RES_32	32

// Dir macros
#define DIR_FORWARD 	1
#define DIR_BACKWARD 	0

// En macros
#define EN_START	0
#define EN_STOP 	1

/* End of exported macros ---------------------------------------------------*/


/* Exported functions -------------------------------------------------------*/
void drv8825_init(drv8825 *drv8825,
		GPIO_TypeDef* Dir_GPIOx, uint16_t Dir_GPIO_Pin,
		GPIO_TypeDef * En_GPIOx, uint16_t En_GPIO_Pin,
		TIM_HandleTypeDef* Timer_Handle, uint32_t Timer_Channel);
uint8_t drv8825_setMode(drv8825 *drv8825, uint8_t mode);
void drv8825_setDir(drv8825* drv8825, uint8_t dir);
void drv8825_setEn(drv8825* drv8825, uint8_t enable);
void drv8825_setSpeedRPM(drv8825* drv8825, uint8_t rpm);

/* End of exported functions ------------------------------------------------*/

#endif /* INC_DRV8825_H_ */

/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

//software timer variable
uint16_t flag_timer[NUM_OF_TIMER];
uint16_t timer2_counter[NUM_OF_TIMER];
uint16_t timer2_MUL[NUM_OF_TIMER];


/**
  * @brief  Init timer interrupt
  * @param  None
  * @retval None
  */
void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
	for(uint8_t i = 0; i < NUM_OF_TIMER; i++){
		flag_timer[i] = 0;
		timer2_MUL[i] = 0;
		timer2_counter[i] = 0;
	}
}

/**
  * @brief  Set duration of software timer interrupt
  * @param  duration Duration of software timer interrupt
  * @retval None
  */
void setTimer2(uint16_t duration, uint8_t index){
	timer2_MUL[index] = duration/TIMER_CYCLE_2;
	timer2_counter[index] = timer2_MUL[index];
	flag_timer[index] = 0;
}

/**
  * @brief  Timer interrupt routine
  * @param  htim TIM Base handle
  * @note	This callback function is called by system
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		for(uint8_t i = 0; i < NUM_OF_TIMER; i++){
			if(timer2_counter[i] > 0){
				timer2_counter[i]--;
				if(timer2_counter[i] == 0) {
					flag_timer[i] = 1;
					timer2_counter[i] = timer2_MUL[i];
				}
			}
		}
		// 1ms interrupt here
		//button_Scan();
	}
}


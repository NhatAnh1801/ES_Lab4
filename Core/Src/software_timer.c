/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1

//software timer variable
uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;

uint16_t flag_timer1 = 0;
uint16_t timer1_counter = 0;
uint16_t timer1_MUL = 0;

uint16_t flag_timer0 = 0;
uint16_t timer0_counter = 0;
uint16_t timer0_MUL = 0;

/**
  * @brief  Init timer interrupt
  * @param  None
  * @retval None
  */
void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}


/**
  * @brief  Set duration of software timer interrupt
  * @param  duration Duration of software timer interrupt
  * @retval None
  */
void setTimer2(uint16_t duration){
	timer2_MUL = duration/TIMER_CYCLE_2;
	timer2_counter = timer2_MUL;
	flag_timer2 = 0;
}
void setTimer1(uint16_t duration){
	timer1_MUL = duration/TIMER_CYCLE_2;
	timer1_counter = timer1_MUL;
	flag_timer1 = 0;
}
void setTimer0(uint16_t duration){
	timer0_MUL = duration/TIMER_CYCLE_2;
	timer0_counter = timer0_MUL;
	flag_timer0 = 0;
}
/**
  * @brief  Timer interrupt routine
  * @param  htim TIM Base handle
  * @note	This callback function is called by system
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		if(timer1_counter > 0){
			timer1_counter--;
			if(timer1_counter == 0) {
				flag_timer1 = 1;
				timer1_counter = timer1_MUL;
			}
		}
		if(timer0_counter > 0){
			timer0_counter--;
			if(timer0_counter == 0) {
				flag_timer0 = 1;
				timer0_counter = timer0_MUL;
			}
		}
		// 1ms interrupt here
		led7_Scan();
	}
}

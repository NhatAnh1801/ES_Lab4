/*
 * fsm.c
 *
 *  Created on: Dec 9, 2023
 *      Author: ADMIN
 */


#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "fsm.h"

#define INIT 1
#define NORMAL_MODE 2
#define MOD_MODE 3
#define TIMER_MODE 4

#define SET_YEAR 40
#define SET_MONTH 41
#define SET_DATE 42
#define SET_DAY 43
#define SET_HOUR 44
#define SET_MIN 45
#define SET_SEC 46

int status = INIT;

int toggle_state = 0;
void GREEN_BLACK_Toggle(){
	if(flag_timer0 == 1){
		setTimer0(500);
		//TODO
		toggle_state = 1- toggle_state;
	}
	if(toggle_state == 0){
		lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	}
	else{
		lcd_ShowIntNum(70, 100, ds3231_hours, 2, BLACK, BLACK, 24);
	}
}
void displayBlinkyHour(){
    GREEN_BLACK_Toggle();
    lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
    lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
    lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
    lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
    lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
    lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

//void fsm_run(){
//	switch(status){
//		case INIT:
//			status = NORMAL_MODE;
//			break;
//		case NORMAL_MODE:
//			if(isButtonDown() == 1){
//				status = SET_YEAR;
//			}
//			  ds3231_ReadTime();
//			  displayTime();
//			break;
//		case SET_YEAR:
//			//Stop
//			ds3231_StopClock();
//			//press up to add by 1
//			if(isButtonUp() == 1){
//				uint8_t temp = ds3231_year;
//				checkButtonState(temp);
//				  ds3231_ReadTime();
//				  displayTime();
//				  if(isButtonE() == 1){
//					  //SAVE and switch state
//					  ds3231_year = temp;
//					  status = SET_MONTH;
//				  }
//			}
//			break;
//		default:
//			break;
//	}
//}

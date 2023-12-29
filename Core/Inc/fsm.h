/*
 * fsm.h
 *
 *  Created on: Dec 29, 2023
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#define ON 1
#define OFF 0

#define TRUE 1
#define FALSE 0

#define NORMAL_MODE 0
#define CHANGE_MODE 1
#define ALARM_MODE 9
#define UART_MODE 10

#define CHANGE_HOUR_MODE 2
#define CHANGE_MINUTE_MODE 3
#define CHANGE_SECOND_MODE 4
#define CHANGE_DAY_MODE 5
#define CHANGE_DATE_MODE 6
#define CHANGE_MONTH_MODE 7
#define CHANGE_YEAR_MODE 8
#define CLK_UART_TIMEOUT 1000

#define HOUR 0
#define MINUTE 1
#define SECOND 2
#define DATE 3
#define DAY 4
#define MONTH 5
#define YEAR 6
#define HOUR_A 7
#define MIN_A 8

#define ALARM_INIT 0
#define ALARM_HOUR 1
#define ALARM_MIN 2

int clk_state;
#endif /* INC_FSM_H_ */

void Real_time_clk();
void Uart_clk();

/*
 * fsm.c
 *
 *  Created on: Dec 29, 2023
 *      Author: ADMIN
 */

#include "fsm.h"
#include "ds3231.h"
#include "button.h"
#include "main.h"
#include "lcd.h"
//init value
// Global Variables


// Initialize variables with meaningful names and use the defined enums

int counter_blink1 = 0;
int counter_blink2 = 0;
int counter_alert1 = 0;
int counter_alert2 = 0;

uint8_t counterBlink1 = 0;
uint8_t counterBlink2 = 0;

uint8_t counterAlert1 = 0;
uint8_t counterAlert2 = 0;

uint8_t hourTemp = 0;
uint8_t minTemp = 0;
uint8_t secTemp = 0;
uint8_t dateTemp = 0;
uint8_t dayTemp = 0;
uint8_t monthTemp = 0;
uint8_t yearTemp = 0;

uint8_t temp = 0;

uint8_t alarmMinTemp = 0;
uint8_t alarmHourTemp = 0;

uint8_t statusNormalAlarm = 0;

uint8_t statusHourAlarm1 = 0;
uint8_t statusHourAlarm2 = 0;
uint8_t statusHourAlarm3 = 0;

uint8_t statusMinAlarm1 = 0;
uint8_t statusMinAlarm2 = 0;
uint8_t statusMinAlarm3 = 0;

uint8_t msgError;
uint8_t uartTimeoutCount = 0;

int clk_state = NORMAL_MODE;
int statusHOUR_ALARM_1 = INIT
int clk_substate = 0;
int statusNORMAL_ALARM = 0;

void initClkTemp(){
	hourTemp = ds3231_hours;
	minTemp = ds3231_min;
	secTemp = ds3231_sec;
	dateTemp = ds3231_date;
	dayTemp = ds3231_day;
	monthTemp = ds3231_month;
	yearTemp = ds3231_year;
}

void initAlarmTemp(){
	alarmHourTemp = ds3231_alarm_hour;
	alarmMinTemp = ds3231_alarm_min;
}

//initialize functions
void displayTime(){
	lcd_ShowIntNum(70, 70, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 70, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 70, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 100, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 100, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_year, 2, YELLOW, BLACK, 24);
}
void initializeClockTemp() {
    hourTemp = ds3231_hours;
    minTemp = ds3231_min;
    secTemp = ds3231_sec;
    dateTemp = ds3231_date;
    dayTemp = ds3231_day;
    monthTemp = ds3231_month;
    yearTemp = ds3231_year;
}

void initializeAlarmTemp() {
    alarmHourTemp = ds3231_alarm_hour;
    alarmMinTemp = ds3231_alarm_min;
}
//Binky function
void blinkDisplay(uint8_t unit) {
    counter_blink1 = (counter_blink1 + 1) % 5;
    counter_blink2 = (counter_blink2 + 1) % 10;

    switch(unit) {
        case HOUR:
            blinkTimeComponent(70, 70, hourTemp, GREEN);
            break;
        case MINUTE:
            blinkTimeComponent(110, 70, minTemp, GREEN);
            break;
        case SECOND:
            blinkTimeComponent(150, 70, secTemp, GREEN);
            break;
        case DAY:
            blinkTimeComponent(20, 100, dayTemp, YELLOW);
            break;
        case DATE:
            blinkTimeComponent(70, 100, dateTemp, YELLOW);
            break;
        case MONTH:
            blinkTimeComponent(110, 100, monthTemp, YELLOW);
            break;
        case YEAR:
            blinkTimeComponent(150, 100, yearTemp, YELLOW);
            break;
        case HOUR_A:
            blinkTimeComponent(70, 170, alarmHourTemp, BLUE);
            break;
        case MIN_A:
            blinkTimeComponent(110, 170, alarmMinTemp, BLUE);
            break;
        default:
            break;
    }
}

void blinkTimeComponent(uint8_t x, uint8_t y, uint8_t value, uint16_t color) {
    if (counter_blink1 == 0) {
        lcd_ShowIntNum(x, y, value, 2, color, BLACK, 24);
    }
    if (counter_blink2 == 0) {
        lcd_ShowIntNum(x, y, 0, 2, BLACK, BLACK, 24);
    }
}
// Alarm alert func
void alarmAlert() {
    counter_alert1 = (counter_alert1 + 1) % 5;
    counter_alert2 = (counter_alert2 + 1) % 10;

    if (counter_alert1 == 0) {
        showAlertScreen(BLUE);
    }
    if (counter_alert2 == 0) {
        showAlertScreen(RED);
    }
}

void showAlertScreen(uint16_t color) {
    lcd_Clear(color);
}
void displayAlarm(void){
    lcd_ShowIntNum(70, 170, ds3231_alarm_hour, 2, BLUE, BLACK, 24);
    lcd_ShowIntNum(110, 170, ds3231_alarm_min, 2, BLUE, BLACK, 24);
}
//clock automatic run
void clkAutoRun() {
    if (clk_state == NORMAL_MODE) {
        ds3231_ReadTime();
        displayTime();
    }

    switch (clk_state) {
        case NORMAL_MODE:
            handleNormalMode();
            break;
        case CHANGE_MODE:
            handleChangeMode();
            break;
        case UART_MODE:
            handleUARTMode();
            break;
        default:
            break;
    }
}

void handleNormalMode() {
    if (isButtonDown()) {
        clk_state = CHANGE_MODE;
        initClkTemp();
    }
    if (button_count[11] == 1) {
        clk_state = UART_MODE;
    }
}

void handleChangeMode() {
    if (isButtonDown()) {
        clk_state = NORMAL_MODE;
    }
}

void handleUARTMode() {
    if (button_count[11] == 1) {
        clk_state = NORMAL_MODE;
    }
}
// clock manual Run
void changeModeHandler(uint8_t* tempVar, uint8_t maxLimit, uint8_t address, uint8_t unit) {
    blinky(unit);

    if (isButtonUp() || (button_count[3] > 40 && button_count[3] % 4 == 0)) {
        (*tempVar)++;
        if (*tempVar >= maxLimit) {
            *tempVar = 0;
        }
    }

    if (isButtonE()) {
        ds3231_Write(address, *tempVar);
    }
}

void updateDisplay(uint8_t x, uint8_t y, uint8_t value, uint16_t color) {
    lcd_ShowIntNum(x, y, value, 2, color, BLACK, 24);
}

void clkManRun() {
    if (clk_state == CHANGE_MODE) {
        switch (clk_substate) {
            case CHANGE_HOUR_MODE:
                changeModeHandler(&hourTemp, 24, ADDRESS_HOUR, HOUR);
                break;
            case CHANGE_MINUTE_MODE:
                changeModeHandler(&minTemp, 60, ADDRESS_MIN, MINUTE);
                break;
            case CHANGE_SECOND_MODE:
                changeModeHandler(&secTemp, 60, ADDRESS_SEC, SECOND);
                break;
            case CHANGE_DAY_MODE:
                changeModeHandler(&dayTemp, 7, ADDRESS_DAY, DAY);
                break;
            case CHANGE_DATE_MODE:
                changeModeHandler(&dateTemp, 30, ADDRESS_DATE, DATE);
                break;
            case CHANGE_MONTH_MODE:
                changeModeHandler(&monthTemp, 12, ADDRESS_MONTH, MONTH);
                break;
            case CHANGE_YEAR_MODE:
                changeModeHandler(&yearTemp, 255, ADDRESS_YEAR, YEAR);
                break;
            default:
                break;
        }

        switch (clk_substate) {
            case CHANGE_HOUR_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_MINUTE_MODE;
                    updateDisplay(70, 70, hourTemp, GREEN);
                }
                break;
            case CHANGE_MINUTE_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_SECOND_MODE;
                    updateDisplay(110, 70, minTemp, GREEN);
                }
                break;
            case CHANGE_SECOND_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_DAY_MODE;
                    updateDisplay(150, 70, secTemp, GREEN);
                }
                break;
            case CHANGE_DAY_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_DATE_MODE;
                    updateDisplay(20, 100, dayTemp, GREEN);
                }
                break;
            case CHANGE_DATE_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_MONTH_MODE;
                    updateDisplay(70, 100, dateTemp, GREEN);
                }
                break;
            case CHANGE_MONTH_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_YEAR_MODE;
                    updateDisplay(110, 100, monthTemp, YELLOW);
                }
                break;
            case CHANGE_YEAR_MODE:
                if (isButton1()) {
                    clk_substate = CHANGE_HOUR_MODE;
                    clk_state = NORMAL_MODE;
                    updateDisplay(150, 100, yearTemp, YELLOW);
                }
                break;
        }
    }
}
// clock alarm func
void handleAlarmInitState() {
    switch (statusNORMAL_ALARM) {
        case INIT:
            statusNORMAL_ALARM = NORMAL;
            break;
        case NORMAL:
            lcd_StrCenter(0, 150, "ALARM(HH:MM)", BLUE, WHITE, 16, 1);
            ds3231_ReadAlarm();
            displayAlarm();
            if (ds3231_alarm_hour == ds3231_hours && ds3231_alarm_min == ds3231_min) {
                alarmAlert();
            }
            break;
        default:
            break;
    }
}

void handleAlarmHourState() {
    switch (statusHOUR_ALARM_1) {
        case INIT:
            statusHOUR_ALARM_1 = NORMAL;
            break;
        case NORMAL:
            blinky(HOUR_A);
            break;
        default:
            break;
    }

    switch (statusHOUR_ALARM_2) {
        case INIT:
            statusHOUR_ALARM_2 = NORMAL;
            break;
        case NORMAL:
            handleAlarmTimeChange(&alarmHourTemp, 24, ADDRESS_ALARM_HOUR, HOUR_A);
            break;
        default:
            break;
    }

    switch (statusHOUR_ALARM_3) {
        case INIT:
            statusHOUR_ALARM_3 = NORMAL;
            break;
        case NORMAL:
            handleAlarmWrite(ADDRESS_ALARM_HOUR, &alarmHourTemp);
            break;
        default:
            break;
    }
}

void handleAlarmMinState() {
    switch (statusMIN_ALARM_1) {
        case INIT:
            statusMIN_ALARM_1 = NORMAL;
            break;
        case NORMAL:
            blinky(MIN_A);
            break;
        default:
            break;
    }

    switch (statusMIN_ALARM_2) {
        case INIT:
            statusMIN_ALARM_2 = NORMAL;
            break;
        case NORMAL:
            handleAlarmTimeChange(&alarmMinTemp, 60, ADDRESS_ALARM_MIN, MIN_A);
            break;
        default:
            break;
    }

    switch (statusMIN_ALARM_3) {
        case INIT:
            statusMIN_ALARM_3 = NORMAL;
            break;
        case NORMAL:
            handleAlarmWrite(ADDRESS_ALARM_MIN, &alarmMinTemp);
            break;
        default:
            break;
    }
}

void handleAlarmTimeChange(uint8_t* tempVar, uint8_t maxLimit, uint8_t address, uint8_t unit) {
    if (isButtonUp() || (button_count[3] > 40 && button_count[3] % 4 == 0)) {
        (*tempVar)++;
        if (*tempVar >= maxLimit) {
            *tempVar = 0;
        }
    }
}

void handleAlarmWrite(uint8_t address, uint8_t* tempVar) {
    if (isButtonE()) {
        ds3231_Write(address, *tempVar);
    }
}

void clkAlarm() {
    if (alarm_state == ALARM_INIT) {
        handleAlarmInitState();
    }

    if (alarm_state == ALARM_HOUR) {
        handleAlarmHourState();
    }

    if (alarm_state == ALARM_MIN) {
        handleAlarmMinState();
    }

    switch (alarm_state) {
        case ALARM_INIT:
            if (isButton4()) {
                alarm_state = ALARM_HOUR;
                initAlarmTemp();
            }
            break;
        case ALARM_HOUR:
            if (isButton4()) {
                alarm_state = ALARM_MIN;
                ds3231_ReadAlarm();
                lcd_ShowIntNum(70, 170, alarmHourTemp, 2, BLUE, BLACK, 24);
            }
            break;
        case ALARM_MIN:
            if (isButton4()) {
                alarm_state = ALARM_INIT;
                lcd_ShowIntNum(110, 170, alarmMinTemp, 2, BLUE, BLACK, 24);
            }
            break;
        default:
            break;
    }
}
// UART func
void handleUartModeChange(uint8_t* tempVar, uint8_t maxLimit, uint8_t address, uint8_t unit, const char* successMsg, const char* errorMsg) {
    if (receiveFlag) {
        receiveFlag = 0;
        setTimer2(10000, CLK_UART_TIMEOUT);

        if (msgError == FALSE) {
            temp = atoi(msg);
            if (1 <= temp && temp <= maxLimit) {
                *tempVar = temp;
                ds3231_Write(address, *tempVar);

                if (unit != CHANGE_YEAR_MODE) {
                    clk_uart_state = unit + 1;
                    uart_Rs232SendString(successMsg);
                    uartTimeoutCount = 0;
                } else {
                    clk_uart_state = CHANGE_HOUR_MODE;
                    uartTimeoutCount = 0;
                }
            } else {
                uart_Rs232SendString(errorMsg);
            }
        } else {
            msgError = FALSE;
            uart_Rs232SendString("ERROR!!! ");
            uart_Rs232SendString(errorMsg);
        }
    }

    if (flag_timer[CLK_UART_TIMEOUT]) {
        setTimer2(10000, CLK_UART_TIMEOUT);
        uart_Rs232SendString(errorMsg);
        uartTimeoutCount++;

        if (uartTimeoutCount == 3) {
            lcd_ShowStr(20, 170, "ERROR!!!", RED, BLACK, 32, 0);
            clk_state = NORMAL_MODE;
        }
    }
}

void clkUartRun() {
    if (clk_state == UART_MODE) {
        switch (clk_uart_state) {
            case CHANGE_HOUR_MODE:
                handleUartModeChange(&ds3231_hours, 24, ADDRESS_HOUR, CHANGE_MINUTE_MODE, "MIN", "HOUR");
                break;
            case CHANGE_MINUTE_MODE:
                handleUartModeChange(&ds3231_min, 60, ADDRESS_MIN, CHANGE_SECOND_MODE, "SEC", "MIN");
                break;
            case CHANGE_SECOND_MODE:
                handleUartModeChange(&ds3231_sec, 60, ADDRESS_SEC, CHANGE_DAY_MODE, "DAY", "SEC");
                break;
            case CHANGE_DAY_MODE:
                handleUartModeChange(&ds3231_day, 8, ADDRESS_DAY, CHANGE_DATE_MODE, "DATE", "DAY");
                break;
            case CHANGE_DATE_MODE:
                handleUartModeChange(&ds3231_date, 30, ADDRESS_DATE, CHANGE_MONTH_MODE, "MONTH", "DATE");
                break;
            case CHANGE_MONTH_MODE:
                handleUartModeChange(&ds3231_month, 12, ADDRESS_MONTH, CHANGE_YEAR_MODE, "YEAR", "MONTH");
                break;
            case CHANGE_YEAR_MODE:
                handleUartModeChange(&ds3231_year, 99, ADDRESS_YEAR, CHANGE_HOUR_MODE, "HOUR", "YEAR");
                break;
            default:
                break;
        }
    }
}
// Functions will be invoked in main
void Real_time_clk(){
	while(!flag_timer[0]);
	flag_timer[0] = 0;
	button_Scan();
	clkAutoRun();
	clkManRun();
	clkAlarm();
	setTimer2(50, 0);
}
void Uart_clk(){
	while(!flag_timer[0]);
	flag_timer[0] = 0;
	button_Scan();
	clkAutoRun();
	clkUartRun();
	setTimer2(50, 0);
}

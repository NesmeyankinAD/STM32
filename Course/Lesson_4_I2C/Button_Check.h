#pragma once
#include "stm32f4xx.h"

//Если состояние кнопки неизменно в течении BTN_PRESS_CNT*BTN_CHECK_MS, считаем что кнопка нажата.
#define BTN_PRESS_CNT  10   //Счётчик опроса кнопки для антидребезга
#define BTN_CHECK_MS   10  //Шаг проверки кнопки, мс

extern uint16_t ms_counter;  //Счётчик мс для опроса кнопки

extern char KEY_0_cnt;  //Счётчики длительности нажатия кнопки
extern char KEY_1_cnt;
extern char KEY_UP_cnt;

extern char KEY_0_state;  //Состояния кнопок (0- отпущена, 1- нажата)
extern char KEY_1_state;
extern char KEY_UP_state;

void BTN_Check(void);
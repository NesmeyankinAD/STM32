#include "Button_Check.h"

uint16_t ms_counter =  0;  //Счётчик мс для опроса кнопки

char KEY_0_cnt  =      0;  //Счётчики длительности нажатия кнопки
char KEY_1_cnt  =      0;
char KEY_UP_cnt =      0;

char KEY_0_state  =    0;  //Состояния кнопок (0- отпущена, 1- нажата)
char KEY_1_state  =    0;
char KEY_UP_state =    0;

void BTN_Check(void)
{
  //Если кнопка нажата, в регистре IDR для GPIO будет 0, а _state = 1;

  if(ms_counter > BTN_CHECK_MS)
  {
    //Опрос кнопки KEY_0
    if((GPIOE -> IDR & GPIO_IDR_ID4) == 0) //Если кнопка нажата
    {
      if(KEY_0_cnt < BTN_PRESS_CNT)
      {
        KEY_0_state = 0; //Кнопка не нажата
        KEY_0_cnt++;
      }
      else
      {
        KEY_0_state = 1; //Кнопка нажата
      }
    }
    else
    {
      KEY_0_state = 0;
      KEY_0_cnt   = 0;
    }
    
    //Опрос кнопки KEY_1
    if((GPIOE -> IDR & GPIO_IDR_ID3) == 0) //Если кнопка нажата
    {
      if(KEY_1_cnt < BTN_PRESS_CNT)
      {
        KEY_1_state = 0; //Кнопка не нажата
        KEY_1_cnt++;
      }
      else
      {
        KEY_1_state = 1; //Кнопка нажата
      }
    }
    else
    {
      KEY_1_state = 0;
      KEY_1_cnt   = 0;
    }

    ////Опрос кнопки KEY_UP
    if((GPIOA -> IDR & GPIO_IDR_ID0) != 0) //Если кнопка нажата
    {
      if(KEY_UP_cnt < BTN_PRESS_CNT)
      {
        KEY_UP_state = 0; //Кнопка не нажата
        KEY_UP_cnt++;
      }
      else
      {
        KEY_UP_state = 1; //Кнопка нажата
      }
    }
    else
    {
      KEY_UP_state = 0;
      KEY_UP_cnt   = 0;
    }
  }
  
  
}
/*
По кнопке KEY_0 происходит запись массива данных из 4-х байт в память EEPROM по заданному адресу.
По кнопке KEY_UP происходит изменение адреса, по котору обращаемся в EEPROM. Происходит циклически 8 раз, отображается в глобальной переменной addChange.
По кнопке KEY_1 происходит чтение данных из памяти по новому адресу ячейки; запись в массив данных.
*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

#include "Button_Check.h"

//Адресация микросхемы EEPROM на шине I2C
#define I2C_DEV_ADDR          0xA0  //Адрес микросхемы EEPROM = 1010 0000. Используется старшие 7 бит.
                              
#define I2C_WR_BIT            0x00  //Запрос на запись данных в I2C-устройство (в EEPROM)
#define I2C_RD_BIT            0x01  //Запрос на чтение данных из I2C-устройства (из EEPROM)
#define I2C_DEV_ADDR_RD       (I2C_DEV_ADDR + I2C_RD_BIT) //Младший бит ставим в 1
#define I2C_DEV_ADDR_WR       (I2C_DEV_ADDR + I2C_WR_BIT) //Младший бит ставим в 0

//Адресация по ячейкам и страницам в EEPROM
//Страница = 8 ячеек, ячейка = 1 байт
#define EEPROM_WR_START_ADDR  0x08  //Запись с 1-ой ячейки в странице 2 
#define EEPROM_WR_LEN         8     //Кол-во байт(ячеек) записи
#define EEPROM_PAGE_LEN_BYTES 8     //Кол-во байт(ячеек) в странице. Применяем для очистки памяти
#define EEPROM_RD_START_ADDR  0x08  //Чтение с 1-ой ячейки в странице 2
#define EEPROM_RD_LEN         8     //Кол-во считываемых байт

char addChange = 0;

char i2c_tx_array[] = { 0x01, 
                        0x02, 
                        0x03, 
                        0x04, 
                        0x05, 
                        0x06, 
                        0x07, 
                        0x3};      //Массив, из которого записываем данные в EEPROM
char i2c_rx_array[8] = {};          //Массив для записи принятых из EEPROM байт

//Состояния конечного автомата
enum states
{
  IDLE         = 0,
  EEPROM_WRITE = 1,
  EEPROM_READ  = 2,
  ADDR_INC     = 3
};

//FSM-флаги состояния, по которым происходит переключение состояния
char IDLE_FLAG         = 0;
char EEPROM_WRITE_FLAG = 0;
char EEPROM_READ_FLAG  = 0;
char ADDR_INC_FLAG     = 0;

//Флаги сброса состояний - взводятся, когда отработаны действия для данного состояния
//(чтобы не циклиться в одном состоянии при долгом нажатии кнопки)
char IDLE_OUT         = 0;
char EEPROM_WRITE_OUT = 0;
char EEPROM_READ_OUT  = 0;
char ADDR_INC_OUT     = 0;

//Объявление функций
void RCC_Init(void);
void GPIO_Init(void);
//void BTN_Check(void);

void I2C_2_Init(void);
void I2C_Write(char start_addr, char data[], uint16_t data_lenght);
void I2C_Read(char read_start_addr, char rx_data[], uint16_t data_lenght);
void I2C_START_Gen(void);
void I2C_STOP_Gen(void);
void I2C_Tx_Device_Addr(char device_addr, char RW_bit);

void State_Flag_Gen(void)
{
  //Если кнопки не нажаты, все состояния сбрасываются

  if(!KEY_0_state) EEPROM_WRITE_OUT = 0;
  else EEPROM_WRITE_FLAG = KEY_0_state & ~(EEPROM_WRITE_OUT);

  if(!KEY_1_state) EEPROM_READ_OUT = 0;
  else EEPROM_READ_FLAG = KEY_1_state & ~(EEPROM_READ_OUT);

  if(!KEY_UP_state) ADDR_INC_OUT = 0;
  else ADDR_INC_FLAG = KEY_UP_state & ~(ADDR_INC_OUT);
}

//Прерывания
void SysTick_Handler(void)
{
/*Прерывание по SysTick таймеру (24-bit downcounter).
Частота срабатывания прерывания настраивается вызовом SysTlick_Config(x);
            AHB_FREQ
где x = ------------------
        TIME_INTERRUPT_REF

TIME_INTERRUPT_REF = 0.001s
AHB_FREQ = 168 MHz
x = 168000
*/    
  ms_counter++;
}



int main(void)
{
  enum states FSM_state = IDLE; //Первоначальное состояние
  
  char eeprom_addr = 0; //Адрес для чтения/записи в EEPROM
  char addr_offset = 0; //Смещение адреса относительно начального значения

  RCC_Init();
  GPIO_Init();
  I2C_2_Init();

  SysTick_Config(168000); //Прерывание по SysTick каждые 1 мс.


  while(1)
  {
    BTN_Check();      //Опрос кнопок
    State_Flag_Gen(); //Выставление флагов

  // ----- Конечный автомат (FSM): блок переключения состояний ------ //
    if(EEPROM_WRITE_FLAG)     FSM_state = EEPROM_WRITE;
    else if(EEPROM_READ_FLAG) FSM_state = EEPROM_READ;  
    else if(ADDR_INC_FLAG)    FSM_state = ADDR_INC;
    else                      FSM_state = IDLE;
  // ----- Конечный автомат (FSM): блок переключения состояний ------ //

  // ----- Конечный автомат (FSM): блок основной логики ------ //
    switch(FSM_state)
    {
      case IDLE: break;

      case EEPROM_WRITE:
        {
        //Запись данных в EEPROM по адресу eeprom_addr
        I2C_Write(eeprom_addr, i2c_tx_array, EEPROM_WR_LEN);

        EEPROM_WRITE_OUT = 1;//Флаг указывает на то, что мы вышли из этого сотояния
        break;
        }

      case EEPROM_READ:
        {
        //Чтение данных из EEPROM по адресу eeprom_addr
        I2C_Read(eeprom_addr, i2c_rx_array, EEPROM_RD_LEN);

        EEPROM_READ_OUT = 1;
        break;
        }

      case ADDR_INC:
        {
        if(addr_offset < EEPROM_WR_LEN) addr_offset++;
        else                            addr_offset = 0;

        ADDR_INC_OUT = 1;
        break;
        }
    }//switch(FSM_state)

    eeprom_addr = EEPROM_RD_START_ADDR + addr_offset;

    addChange = addr_offset;//debug

  // ----- Конечный автомат (FSM): блок основной логики ------ //


  }//while(1)
}//main




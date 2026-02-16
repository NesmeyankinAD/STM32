#include "stm32f4xx.h"

/* Программа отправляет в flash-память данные, затем читает их в mem_read
Что отправили, то и должны получить в mem_read*/

/*---Объявление команд для flash-памяти по datasheet---*/
#define EN_RST  0x66      //Enable reset
#define RST     0x99      //Reset
#define WR_EN   0x06      //Write enable
#define SECT_ER 0x20      //Sector erase
#define RD_SR1  0x05      //Read register SR1
#define PG_PROG 0x02      //Page programming
#define RD_DATA 0x03      //Read data
#define ADDR    0x20FF00  //Addres
/*---Объявление команд для flash-памяти по datasheet---*/

#define IND1    0x01
#define IND2    0x02
#define IND3    0x03
#define CLEAR   0xFF

#define CSLOW   GPIOB -> BSRR |= GPIO_BSRR_BR0; //Активный режим NSS
#define CSHIGH  GPIOB -> BSRR |= GPIO_BSRR_BS0; //Не активный режим NSS

//Глобальные переменные
uint16_t s_counter = 0; //Счётчик секунд

uint8_t mem_read = 0;   //Переменная для чтения из памяти

//Объявление функций
void RCC_Init(void);
void GPIO_Init(void);
void SPI1_Init(void);

uint8_t W25_send(uint8_t);

//Прерывания
void SysTick_Handler(void)
{/*Прерывание по SysTick таймеру (24-bit downcounter).
Частота срабатывания прерывания настраивается вызовом SysTlick_Config(x);
            AHB_FREQ
где x = ------------------
        TIME_INTERRUPT_REF

TIME_INTERRUPT_REF = 0.001s
AHB_FREQ = 168 MHz
x = 168000
*/    
  s_counter++;
}


int main(void)
{
  SystemInit();
  RCC_Init();
  GPIO_Init();  
  SPI1_Init();

  SysTick_Config(168000000); //Прерывание по SysTick каждые 1с.

  /*---Запись в память---*/
  CSLOW                               
  W25_send(PG_PROG);                  
  W25_send((ADDR >> 16) & 0xFF);       //Отправляем адрес сектора для записи
  W25_send((ADDR >> 8)  & 0xFF);       //отправляем по 8 бит
  W25_send((ADDR)       & 0xFF);                                
  W25_send(112);                      //Отправка данных
  CSHIGH

  CSLOW
  W25_send(RD_SR1);
  while((W25_send(0x00) & 0x01) == 1); //Ждём пока не завершится запись (пока BUSY = 1)
  CSHIGH
  /*---Запись в память---*/

  while(1)
  {
    /*---Читаем из памяти---*/
    CSLOW                               
    W25_send(RD_DATA);                  
    W25_send((ADDR >> 16) & 0xFF); //Отправляем адрес сектора для чтения
    W25_send((ADDR >> 8)  & 0xFF); //отправляем по 8 бит
    W25_send((ADDR)       & 0xFF);                          
    mem_read = W25_send(0x00);     //Чтение данных
    CSHIGH
    /*---Читаем из памяти---*/
  }
}
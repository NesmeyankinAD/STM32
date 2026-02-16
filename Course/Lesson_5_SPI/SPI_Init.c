#include "stm32f4xx.h"

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

#define CSLOW   GPIOB -> BSRR |= GPIO_BSRR_BR0; //Активный режим NSS
#define CSHIGH  GPIOB -> BSRR |= GPIO_BSRR_BS0; //Не активный режим NSS


uint8_t W25_send(uint8_t data)
{
  SPI1 -> DR = data;

  while((SPI1 -> SR & SPI_SR_TXE) == 0); //Ждём, пока не очистится буфер передачи
                                         //т.е. пока данные не будут переданы в память

  while((SPI1 -> SR & SPI_SR_RXNE) == 0);//Ждём, пока буфер приёма станет не пустым
                                         //т.е. пока не придут данные из памяти
  
  return SPI1 -> DR;
  
}

void SPI1_Init(void)
{
  /*Пины SPI1
  PB0 - NSS(F_CS)
  PB3 - SCK
  PB4 - MISO
  PB5 - MOSI
  */
  
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;          //Включение тактирования порта E

  GPIOB -> MODER |= GPIO_MODER_MODE3_1 
                  | GPIO_MODER_MODE4_1 
                  | GPIO_MODER_MODE5_1;           //PB3, 4, 5 Alternate function

  GPIOB -> AFR[0] |= (5 << GPIO_AFRL_AFSEL3_Pos); // Альт. функция 5 SPI:SCK
  GPIOB -> AFR[0] |= (5 << GPIO_AFRL_AFSEL4_Pos); // Альт. функция 5 SPI:MISO
  GPIOB -> AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos); // Альт. функция 5 SPI:MOSI

  GPIOB ->PUPDR |= GPIO_PUPDR_PUPD3_1
                 | GPIO_PUPDR_PUPD4_1
                 | GPIO_PUPDR_PUPD5_1;        //Подтяжка PB3, PB4, PB5 к GND
                                              
  GPIOB -> MODER |= GPIO_MODER_MODE0_0;       //PB0 работа на выход, CS
                                              
  GPIOB -> OTYPER &= ~(GPIO_OTYPER_OT0);      //PB0 Push-pull
                                              
  GPIOB -> PUPDR |= GPIO_PUPDR_PUPD0_1;       //PB0 подтяжка к GND
                                              
  GPIOB -> BSRR |= GPIO_BSRR_BS0;             //PB0 = 1, неактивный chip select
                                              
  /*---Настройка модуля SPI1---*/             
                                              
  RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;       //Тактирование SPI1
                                              
  SPI1 -> CR1 |= SPI_CR1_BR;                  //Baud rate = 328kHz (fpclk/256)            
  SPI1 -> CR1 |= SPI_CR1_MSTR;                //Режим работы - мастер
  SPI1 -> CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;   //Программное управление NSS(Negative Slave Select)
  SPI1 -> CR1 &= ~(SPI_CR1_DFF);              //Фрейм данных 8-бит

  SPI1 -> CR1 |= SPI_CR1_SPE;                 //Включение модуля SPI1


  //Посылаем команды в память

  CSLOW                                //Активируем обращение к памяти
  W25_send(EN_RST);                    //Отправка команды
  CSHIGH                               //Деактивируем обращение к памяти
                                       
  CSLOW                                
  W25_send(RST);                       
  CSHIGH                               
                                       
  CSLOW                                
  W25_send(WR_EN);                     //Разрешение на запись
  CSHIGH                              
                                      
  CSLOW                               
  W25_send(SECT_ER);                  
  W25_send((ADDR >> 16) & 0xFF);       //Отправляем адрес сектора для стирания
  W25_send((ADDR >> 8)  & 0xFF);       //отправляем по 8 бит
  W25_send((ADDR)       & 0xFF);       
  CSHIGH                               
                                       
  CSLOW                                
  W25_send(RD_SR1);                    //Читаем регистр памяти SR1
  while((W25_send(0x00) & 0x01) == 1); //Ждём пока память занята BUSY=1
  CSHIGH                               
                                       
  CSLOW                                
  W25_send(WR_EN);                     //Разрешение на запись
  CSHIGH
}
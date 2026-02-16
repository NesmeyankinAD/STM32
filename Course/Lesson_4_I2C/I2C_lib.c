#include "stm32f4xx.h"

#define I2C_DEV_ADDR          0xA0  //Адрес микросхемы EEPROM = 1010 0000. Используется старшие 7 бит.
                              
#define I2C_WR_BIT            0x00  //Запрос на запись данных в I2C-устройство (в EEPROM)
#define I2C_RD_BIT            0x01  //Запрос на чтение данных из I2C-устройства (из EEPROM)

void I2C_START_Gen(void)
{
  //Генерация START-условия

  I2C2 -> CR1 |= I2C_CR1_START;
  while((I2C2 -> SR1 & I2C_SR1_SB) == 0){}; //Ожидание выставления бита SB, сигнализирующего о наличии на шине START-условия
                                            //пока равно нулю, бит не выставлен, START-условия нет
}

void I2C_STOP_Gen(void)
{
  //Генерация STOP-условия

  I2C2 -> CR1 |= I2C_CR1_STOP;
}

void I2C_Tx_Device_Addr(char device_addr, char RW_bit)
{
  //Отправка адреса устройства на шине I2C

  I2C2 -> DR = (device_addr | RW_bit);//(device_addr + RW_bit);        //Запись 7-битного адреса устройства + 1-бита чтение/запись
  
  while((I2C2 -> SR1 & I2C_SR1_ADDR) == 0){}; //Ждём пока не будет выставлен бит готовности адреса на шине
                                              //(пока подчинённое устройство не ответило битом ACK)
  
  (void)I2C2 -> SR1; //Очистка бита ADDR
  (void)I2C2 -> SR2; //чтением регистров SR1, SR2
}

void I2C_Write(char start_addr, char data[], uint16_t data_lenght)
{
  //Функция записи данных в I2C-устройство

  I2C2 -> CR1 |= I2C_CR1_ACK;                   //Включить генерацию битов ACKnowledge

  while((I2C2 -> SR2 & I2C_SR2_BUSY) != 0){};   //Проверка, свободна ли шина I2C
                                                //Пока шина занята, бит BUSY = 1 в SR2 - условие не равно нулю

  I2C_START_Gen();                              //Генерация START-условия

  I2C_Tx_Device_Addr(I2C_DEV_ADDR, I2C_WR_BIT); //Отправка адреса устройства I2C

  I2C2 -> DR = start_addr;                      //Отправка адреса начальной ячейки памяти, куда произведём запись
  while((I2C2 -> SR1 & I2C_SR1_TXE) == 0){};    //ждём флаг I2C_SR1_TXE = 1

  //Цикл записи данных в устройство
  for(uint16_t i = 0; i < data_lenght; i++)
  {
    I2C2 -> DR = data[i];

    while((I2C2 -> SR1 & I2C_SR1_TXE) == 0){}; //Ждём, пока регистр отправки не станет пустым (I2C_SR1_TXE = 1)
                                               //(пока не отправится байт данных)
  }

  I2C_STOP_Gen();                              //Генерация STOP-условия, освобождение шины
}


void I2C_Read(char read_start_addr, char rx_data[], uint16_t data_lenght)
{
  //Функция чтения данных из I2C-устройства

  I2C2 -> CR1 |= I2C_CR1_ACK;                   //Включить генерацию битов ACKnowledge

  while((I2C2 -> SR2 & I2C_SR2_BUSY) != 0){};   //Проверка, свободна ли шина I2C
                                                //Пока шина занята, бит BUSY = 1 в SR2 - условие не равно нулю

  I2C_START_Gen();                              //Генерация START-условия

  I2C_Tx_Device_Addr(I2C_DEV_ADDR, I2C_WR_BIT); //Передача адреса I2C-устройства и бита WR

  I2C2 -> DR = read_start_addr;                 //Передача адреса ячейки памяти EEPROM, с которой начинаем чтение
  while((I2C2 -> SR1 & I2C_SR1_TXE) == 0){};

  /* Пример отправки 2-байтного адреса ячейки памяти
      
      I2C2 -> DR |= (read_start_addr >> 8);     //Передача старшего байта адреса
      while((I2C2 -> SR1 & I2C_SR1_TXE) == 0){};//Ожидание передачи
      
      I2C2 -> DR |= (read_start_addr >> 8);     //Передача младшего байта адреса
      while((I2C2 -> SR1 & I2C_SR1_TXE) == 0){};//Ожидание передачи

  */

  
  I2C_START_Gen();                              //Повторное START-условие

  I2C_Tx_Device_Addr(I2C_DEV_ADDR, I2C_RD_BIT); //Передача адреса I2C-устройства и бита RD

  //Цикл чтения данных data_lenght-1
  for(uint16_t i = 0; i < data_lenght-1; i++)
  {
    while((I2C2 -> SR1 & I2C_SR1_RXNE) == 0){}; //Ожидание, пока регистр DR не заполнится полученными данными
                                                //бит RXNE=1 - регистр DR заполнен 

    rx_data[i] = I2C2 -> DR;                    //Чтение байта данных
  }


  I2C2 -> CR1 &= ~(I2C_CR1_ACK);                //Отключение генерации ACK

  while((I2C2 -> SR1 & I2C_SR1_RXNE) == 0){};   //Принять байт data_lenght
  rx_data[data_lenght - 1] = I2C2 -> DR;

  I2C_STOP_Gen();                               //Генерация STOP-условия
}
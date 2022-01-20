#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // подключение билиотеки работы с радиопередатчиком 

RF24 radio(9, 10); // порты D9, D10: CSN CE радиопередатчика

const uint32_t pipe = 111156789; // адрес "рабочей трубы" радиопередатчика

void setup() {
  Serial.begin(9600);
  Serial.println("ReceiverTester ON"); // сигнал об успешном включении передатчика
  radio.begin();   // инициализация работы радиопередатчика
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit 
  radio.setChannel(0x6f);         // установка канала
  radio.setAutoAck(false);       // автоответ
  radio.openReadingPipe(1, pipe); // открыть "трубу" на приём
  radio.startListening();        // приём
}

void loop() {
  if (radio.available())
  {
    byte A;
    radio.read(A, 4); // чтение данных высоты по радиоканалу
    Serial.println(String(A)); // вывод данных в монитор порта с преобразованием из byte в строку
  }
}

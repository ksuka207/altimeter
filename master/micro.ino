#include <SFE_BMP180.h> // подключение билиотеки работы с барометрицеским датчиком
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // подключение билиотеки работы с радиопередатчиком
RF24 radio(9, 10); // порты D9, D10: CSN CE радиопередатчика

const uint32_t pipe = 111156789; // адрес "рабочей трубы" радиопередатчика
SFE_BMP180 pressure; 
#define ALTITUDE 0.0 // стартовая высота

void setup()
{
  Serial.begin(9600);
  Serial.println("TransmitterTester ON"); // сигнал об успешном включении передатчика
  if (pressure.begin())
    Serial.println("BMP180 ON"); // сигнал об успешном включении барометрического датчика
  radio.begin();                // инициализация работы радиопередатчика
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit
  radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя
  radio.setChannel(0x6f);         // установка канала
  radio.setAutoAck(false);       // автоответ
  radio.powerUp();               // включение
  radio.stopListening();  //вкючение только на передачу
  radio.openWritingPipe(pipe);   // открыть "трубу" для отправки
}

void loop()
{
  char status;
  double T, P, p0, a;
  Serial.println();
  status = pressure.startTemperature(); // начало процесса измерения температуры
  if (status != 0) {
    delay(status);
    status = pressure.getTemperature(T); // получение значения температуры
    if (status != 0) {
      status = pressure.startPressure(3); // начало процесса измерения давления
      if (status != 0) {
        delay(status);
        status = pressure.getPressure(P, T); // получение значения давления
        if (status != 0)
        {
          p0 = pressure.sealevel(P, ALTITUDE); // давление на уровне моря
          a = pressure.altitude(P, p0); // получение значения относительной высоты
        }
      }
    }
  }
  byte A;
  A = round(a); // преобразование высоты в формат byte для передачи по радиоканалу
  radio.write(&A, 4); // отправка значения высоты по радиоканалу
  delay(100);  // задержка 0,1 с
}

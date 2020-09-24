#include <OneWire.h>
#include <DallasTemperature.h>

// Шина данных подключена к выводу D2 на ESP8266
#define ONE_WIRE_BUS 4

// Настройка объекта oneWire для связи с любыми устройствами OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Передаем ссылку на объект oneWire объекту Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Переменная для хранения адресов устройств
DeviceAddress Thermometer;

int deviceCount = 0;

void setup(void)
{
  // запуск последовательного порта
  Serial.begin(9600);

  // запуск библиотеке
  sensors.begin();

  // определение устройств на шине
  Serial.println("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  
  Serial.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    sensors.getAddress(Thermometer, i);
    printAddress(Thermometer);
  }
}

void loop(void)
{ }

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}

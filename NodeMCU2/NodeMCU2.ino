// Elegant OTA UPDATES
#include <elegantWebpage.h>
#include <ElegantOTA.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

// Blynk
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

// Temp 
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 4  //D2 pin of nodemcu
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature tempSensors(&oneWire);  
int temp_0; // переменная для 1 датчика
int temp_1; // переменная для 2 датчика

// адреса сенсоров 
uint8_t sensor1[8] = {0x28, 0x17, 0xBC, 0xBC, 0x33, 0x20, 0x01, 0x88}; 
uint8_t sensor2[8] = {0x28, 0x2F, 0x39, 0xB9, 0x33, 0x20, 0x01, 0x5C};

// Blynk token
char auth[] = "_nOwxvkIMWBc0eil-gk-e6x8Yd5ql91-";

// Your WiFi credentials.
char ssid[] = "CoffeLink";
char pass[] = "qqww1122";
ESP8266WebServer server(80);


void setup()
{
  Serial.begin(115200); // Debug console
  Blynk.begin(auth, ssid, pass, IPAddress(188,225,35,45), 8080); // run blynk
  tempSensors.begin(); // run DS18B20 sensors
  timer.setInterval(3000L, getSendData); // run blynk timer update 5 min
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "Hi! I am ESP8266.");
  });
  
  
  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  timer.run();
  Blynk.run();
  server.handleClient();
}

void getSendData() 
{
  tempSensors.requestTemperatures(); // запрос информации с датчиков
  temp_0 = tempSensors.getTempC(sensor1); // Sensor0 показания для датчика 1
  temp_1 = tempSensors.getTempC(sensor2); // Sensor1 показания для датчика 2 

  Serial.print("Temp_0: ");
  Serial.print(temp_0);
  Serial.print(" oC . Temp_1: ");
  Serial.print(temp_1);
  Serial.println(" oC");

  // Два поля со значениями
  Blynk.virtualWrite(10, temp_0); //выврд данных на виртуальный пин V10
  Blynk.virtualWrite(11, temp_1); //вывод данных навиртуальный пин V11

  // Таблица со значениями
  Blynk.virtualWrite(V0, "add", 1, "Температура на улице",temp_0 = tempSensors.getTempC(sensor2));
  Blynk.virtualWrite(V0, "add", 2, "Температура в зале",temp_0 = tempSensors.getTempC(sensor1));
}

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

// Blynk token
char auth[] = "lP8CXFCFmjsxNnQ93EBUBUoBlnh9VmP5";

// Your WiFi credentials.
char ssid[] = "CoffeLink";
char pass[] = "qqww1122";
ESP8266WebServer server(80);


void setup()
{
  Serial.begin(115200); // Debug console
  Blynk.begin(auth, ssid, pass); // run blynk
  tempSensors.begin(); // run DS18B20 sensors
  timer.setInterval(2000L, getSendData); // run blynk timer update 2 second
  
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
  tempSensors.requestTemperatures(); 
  temp_0 = tempSensors.getTempCByIndex(0); // Sensor 0 показания для датчика 1 в цельсиях
  temp_1 = tempSensors.getTempCByIndex(1); // Sensor 0 показания для датчика 2 в цельсиях 

  Serial.print("Temp_0: ");
  Serial.print(temp_0);
  Serial.print(" oC . Temp_1: ");
  Serial.print(temp_1);
  Serial.println(" oC");
   
  Blynk.virtualWrite(10, temp_0); //выврд данных на виртуальный пин V10
  Blynk.virtualWrite(11, temp_1); //вывод данных навиртуальный пин V11
}

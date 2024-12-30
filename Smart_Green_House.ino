/* The source Code from : https://github.com/riyadhasan24
 * By Md. Riyad Hasan
 */

#define BLYNK_TEMPLATE_ID "TMPL688Vu0QOG"
#define BLYNK_TEMPLATE_NAME "smart"
#define BLYNK_AUTH_TOKEN "uaoHKqd97sanXcLXln4DMdFuDA8zE1N9"

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <BlynkSimpleEsp8266.h>

// Define pins and variables
int Soil_Value;
int Fan_State;
int Pump_State;
float Humidity;
float Temperature;

#define DHT_Pin D3
#define DHT_Type DHT11

#define Fan_Pin D5
#define Pump_Pin D2
#define Soil_Pin A0

DHT DHT_Sensor(DHT_Pin, DHT_Type);

// Your WiFi credentials
char ssid[] = "IOT";
char pass[] = "12345678";

BlynkTimer timer;

void sendSensorData() 
{
  Humidity = DHT_Sensor.readHumidity();
  Temperature = DHT_Sensor.readTemperature();

  Soil_Value = analogRead(Soil_Pin);
  Soil_Value = map(Soil_Value, 0, 1023, 0, 100);

  Blynk.virtualWrite(V0, Soil_Value);
  Blynk.virtualWrite(V1, Humidity);
  Blynk.virtualWrite(V2, Temperature);

  Serial.print("Soil: ");
  Serial.print(Soil_Value);
  Serial.print("%, Temp: ");
  Serial.print(Temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(Humidity);
  Serial.println("%");
}

void setup() 
{
  DHT_Sensor.begin();
  Serial.begin(115200);

  pinMode(Soil_Pin, INPUT);
  pinMode(Fan_Pin, OUTPUT);
  pinMode(Pump_Pin, OUTPUT);

  digitalWrite(Fan_Pin, LOW);
  digitalWrite(Pump_Pin, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send sensor data every second
  timer.setInterval(1000L, sendSensorData);
}

void loop() 
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V3) 
{
  Pump_State = param.asInt(); // Get value from app
  digitalWrite(Pump_Pin, Pump_State ? LOW : HIGH); // Control pump
}

BLYNK_WRITE(V4) 
{
  Fan_State = param.asInt(); // Get value from app
  digitalWrite(Fan_Pin, Fan_State ? LOW : HIGH); // Control fan
}

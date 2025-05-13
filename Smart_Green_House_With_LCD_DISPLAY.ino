/* The source Code from : https://github.com/riyadhasan24
 * By Md. Riyad Hasan
 */

#define BLYNK_TEMPLATE_ID "TMPL6L_8T7045"
#define BLYNK_TEMPLATE_NAME "Smart Irrigration"
#define BLYNK_AUTH_TOKEN "9sjUpZYktFSxglayyiCfWxH3a_AqEL66"

#include <DHT.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>

// I2C Address can be 0x27 or 0x3F; try 0x27 first
LiquidCrystal_I2C lcd(0x27, 16, 2); // address, columns, rows

// Define pins and variables
int Soil_Value;
int Fan_State;
int Pump_State;
int Humidity;
int Temperature;

#define DHT_Pin D5
#define DHT_Type DHT11

#define Fan_Pin D3
#define Pump_Pin D7
#define Soil_Pin A0

DHT DHT_Sensor(DHT_Pin, DHT_Type);

// Your WiFi credentials
char ssid[] = "KRISHITECH";
char pass[] = "21522152";

BlynkTimer timer;

void sendSensorData() 
{
  Humidity = DHT_Sensor.readHumidity();
  Temperature = DHT_Sensor.readTemperature();

  Soil_Value = analogRead(Soil_Pin);
  Soil_Value = map(Soil_Value, 1023, 0, 0, 100);

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

//  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(2, 0);
  lcd.print(Temperature);
  lcd.setCursor(5, 0);
  lcd.print("C");
  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.setCursor(11, 0);
  lcd.print(Humidity);
  lcd.setCursor(14, 0);
  lcd.print("%");

  lcd.setCursor(3, 1);
  lcd.print("Soil M:");
  lcd.setCursor(10, 1);
  lcd.print(Soil_Value);
  lcd.setCursor(13, 1);
  lcd.print("%");
}

void setup() 
{
  DHT_Sensor.begin();
  Serial.begin(115200);
  
  lcd.init();          // initialize the LCD
  lcd.backlight();      // turn on backlight

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

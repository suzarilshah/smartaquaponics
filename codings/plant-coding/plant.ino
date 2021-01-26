/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h> //Library for Humidity Sensor
#include <OneWire.h> // Library for DS18B20 - Temperature Sensor
#include <DallasTemperature.h> // Library for DS18B20 - Temperature Sensor

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "STqyq8Gn9rDiIS6ofa3wQdCn_wYoP9xq";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "UniKL JPE";
char pass[] = "";

#define DHTPIN 2          // Digital Pin 4 - for Humidity Sensors
#define DHTTYPE DHT11     // DHT 11

const int pin = 4;

OneWire oneWire(pin); // Digital Pin 2 - For Temperature Sensors
DallasTemperature sensors(&oneWire); // encapsulate temp.Reading to object, sensors
DHT dht(DHTPIN, DHTTYPE); // Declare DHT11 for pin type
float temp = 0; //Initial Temperature
BlynkTimer timer;

// for water level sensors
#define WaterSensor A0 // Water Level Sensors - Analog Pin 0
int SensorValue = 0; // Initial Value of Water Level Sensor
float WaterHeight = 0; // Initial Value of Water Level in Tank
float SensorVoltage = 0;
int MaxValue = 1000; // max=1023
float SensorHeight = 4.0;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// Method for DHT11 - Temperature Sensor
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  // float percent = h / * 100;

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    Serial.println(h);
    Serial.println(t);
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

// Method for Water Level Sensor
void getSendData(){
  SensorValue = analogRead(WaterSensor);
  WaterHeight = SensorValue * SensorHeight / MaxValue;
  Serial.println(WaterHeight);
  Blynk.virtualWrite(V7, WaterHeight);  
  
  }

// Method for Temperature Sensor
void sendTemps(){
  
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);  
  //Serial.println(String("Temperature= )"+temp1+ String(" C"));
  Blynk.virtualWrite(V8, temp);
  }

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(DHTPIN, OUTPUT);

  dht.begin(); // Beginning operation method for Humidity Sensor
  sensors.begin(); // Beginning operation method for Temperature Sensor

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, getSendData);
  timer.setInterval(1000L, sendTemps);
}

void loop()
{
  Blynk.run();
  timer.run();
}

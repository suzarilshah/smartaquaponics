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
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <ESP8266WiFi.h> // WiFi Module library, required by Blynk Applications
#include <BlynkSimpleEsp8266.h> // required by Blynk Applications to sync with 8266
#include <ESP8266mDNS.h>    // For OTA w/ ESP8266
#include <WiFiUdp.h>        // For OTA
#include <ArduinoOTA.h>     // For OTA

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "J9-7KFQilB5jItVhISr1MyJL7H-2H0Yb"; // Auth token for Fish Tanks 

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "UniKL JPE";
char pass[] = "";

#include <SimpleTimer.h>
SimpleTimer timer;

#define waterSensor A0
int sensorValue = 0;
float waterHeight = 0;
float sensorVoltage = 0;
int maxValue = 100;
float sensorHeight = 4.0;

//Relay configurations
const int Relay_1 = 16;       //pin D0 on ESP8266


void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  timer.setInterval(1000L, getSendData);

  //Relay config
  pinMode(Relay_1, OUTPUT);
  digitalWrite(Relay_1, HIGH);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void getSendData(){
  sensorValue = analogRead(waterSensor);
  waterHeight = sensorValue * sensorHeight / maxValue;
  Serial.println(waterHeight);
  Blynk.virtualWrite(3, waterHeight);  
  
  }

// Relay Method
BLYNK_WRITE(V20)
  {
  int pinValue = param.asInt();
  if (pinValue == 1)
    {
    digitalWrite(Relay_1, HIGH);
    Serial.println("Relay 1 On");
    }
  else if (pinValue == 0)
    {
    digitalWrite(Relay_1, LOW);
    Serial.println("Relay 1 Off");
    }
  }

/***************************************************
 Created by: regi18
 Version: 3.1.2
 Github: https://github.com/regi18/Sonoff-Blynk
 **************************************************/

/* Libraries */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/*** Here are the things that you have to change ***/
/***************************************************/

/* Blynk AUTH key */
char const AUTH[] = "BLYNK AUTH TOKEN";

/* Wi-Fi Details */
char const SSID[] = "SSID";
char const PASS[] = "WIFI PASSWORD";
char const HOSTNAME[] = "HOSTNAME";
char const OTAPSW[] = "OTA PASSWORD";

/* Blynk Virtual Pins */
#define RELAYVPIN V5

/* Device Pins */
#define RELAY 12
#define LED 13
#define BUTTON 0

/**************************************************/



/* General variables and declarations */
BlynkTimer timer;
volatile bool state;


void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(RELAY,OUTPUT);
  pinMode(BUTTON,INPUT);
  
  WiFi.hostname(HOSTNAME);
  Blynk.begin(AUTH, SSID, PASS);

  ArduinoOTA.setPassword(OTAPSW);
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.begin();

  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButton, FALLING);
  
  digitalWrite(LED, LOW);
  delay(700);
  digitalWrite(LED, HIGH);
  delay(700);
  digitalWrite(LED, LOW);
  delay(700);
  digitalWrite(LED, HIGH);
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void loop() {
  if (Blynk.connected()) { Blynk.run(); }
  else { Blynk.connect(); }
  timer.run();
  ArduinoOTA.handle();
}

/***************************************************
 * Read virtual pin 0 and turn on/off relay
 **************************************************/
BLYNK_WRITE(RELAYVPIN){
  state = param.asInt();
  digitalWrite(RELAY, state);
}

/***************************************************
 * Handle the interrupt
 **************************************************/
void handleButton()
{
  state = !state;
  Blynk.virtualWrite(RELAYVPIN, state);
  digitalWrite(RELAY, state);
}

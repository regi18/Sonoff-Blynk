/***************************************************
 Created by: regi18
 Version: 2.1.4
 Github: https://github.com/regi18/Sonoff-Blynk
 **************************************************/

/* Libraries */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
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

/* Thingspeak */
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY ="THINGSPEAK API KEY";

/* Blynk Virtual Pins */
#define RELAYVPIN V0
#define TEMPVPIN V1
#define HUMVPIN V2

/* Temperature sensor */
#define DHTPIN  14
#define DHTTYPE DHT22

/* Device Pins */
#define RELAY 12
#define LED 13
#define BUTTON 0

/**************************************************/



/* General variables and declarations */
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
BlynkTimer timer;

volatile bool state;
float hum = 0;
float temp = 0;


void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(RELAY,OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(BUTTON,INPUT);
  
  WiFi.hostname(HOSTNAME);
  Blynk.begin(AUTH, SSID, PASS);

  ArduinoOTA.setPassword(OTAPSW);
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.begin();

  dht.begin();
  timer.setInterval(15000L, getDhtData);

  /* Set the interval for sending data to Thingspeak.com (now is 1200000 = 20 minutes) */
  timer.setInterval(1200000L, sendDataTS);

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
 * Get DHT data
 **************************************************/
void getDhtData(void)
{
  hum = dht.readHumidity();              // Read humidity (percent)
  temp = dht.readTemperature(false);     // Read temperature as Celsius
  if (isnan(hum) || isnan(temp))         // Check if any reads failed and exit early (to try again).
  {
    return;
  }

  Blynk.virtualWrite(TEMPVPIN, temp);
  Blynk.virtualWrite(HUMVPIN, hum);
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


/***************************************************
 * Sending Data to Thinkspeak Channel
 **************************************************/
void sendDataTS(void)
{
   if (client.connect(TS_SERVER, 80)) 
   { 
     String postStr = TS_API_KEY;
     postStr += "&field1=";
     postStr += String(temp);
     postStr += "&field2=";
     postStr += String(hum);
     postStr += "\r\n\r\n";
   
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     delay(1000); 
   }
   client.stop();
}

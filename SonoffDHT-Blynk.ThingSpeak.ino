/***************************************************
 Created by: regi18
 Version: 2.1.4
 Github: https://github.com/regi18/Sonoff-Blynk
 **************************************************/

/* ESP */
#include <ESP8266WiFi.h>
WiFiClient client;

/* Blynk */
#include <BlynkSimpleEsp8266.h>
char auth[] = "BLYNK AUTH TOKEN";
char ssid[] = "SSID";
char pass[] = "WIFI PASSWORD";

/* Thingspeak */
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY ="THINGSPEAK API KEY";

/* DHT22 */
#include <DHT.h>
#define DHTPIN  14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;

/* OTA Updates */
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/* General variables */
int i;
int relay = 12;
int led = 13;

BlynkTimer timer;

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup()
{
  pinMode(led,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(DHTPIN, INPUT);
  Blynk.begin(auth, ssid, pass);
  ArduinoOTA.setPassword((const char *)"OTA PASSWORD");
  ArduinoOTA.setHostname("OTA USERNAME");
  ArduinoOTA.begin();
  dht.begin();
  timer.setInterval(1000L, getDhtData);
  timer.setInterval(1000L, sendUptime);
  /* Set the interval for sending data to Thingspeak.com (now is 300000 = 5 minutes) */
  timer.setInterval(300000L, sendDataTS);
  timer.setInterval(10000L, CheckConnection);
  digitalWrite(led, LOW);
  delay(700);
  digitalWrite(led, HIGH);
  delay(700);
  digitalWrite(led, LOW);
  delay(700);
  digitalWrite(led, HIGH);
}

void loop() {
  if(Blynk.connected()){
    Blynk.run();
  }
  timer.run();
  ArduinoOTA.handle();
}

/***************************************************
 * Get DHT data
 **************************************************/
void getDhtData(void)
{
  hum = dht.readHumidity();          // Read humidity (percent)
  temp = dht.readTemperature(false);     // Read temperature as Celsius
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    return;
  }
}

/***************************************************
 * Send DHT data to Blynk
 **************************************************/
void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, hum);
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

/***************************************************
 * Read virtual pin 0 and turn on/off relay and led
 **************************************************/
BLYNK_WRITE(V0){
  i = param.asInt();
  digitalWrite(12, i);
}

/***************************************************
 * check every 10s if connected to Blynk server
 **************************************************/
void CheckConnection(){
  if(!Blynk.connected()){
    Blynk.connect();
  }
}

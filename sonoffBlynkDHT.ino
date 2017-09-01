#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "BLYNK AUTH TOKEN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "WIFI PASSOWRD";


int i;
int relay = 12;
int led = 13;
int buttonstate;
float humidity, temp;

#define DHTTYPE DHT22
#define DHTPIN  14
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void setup()
{
  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false)
  {
    // Wait until connected
  }
  pinMode(led,OUTPUT);
  digitalWrite(led, LOW);
  delay(700);
  digitalWrite(led, HIGH);
  delay(700);
  digitalWrite(led, LOW);
  delay(700);
  digitalWrite(led, HIGH);
  delay(700);
  digitalWrite(led, LOW);
  delay(700);
  digitalWrite(led, HIGH);
  pinMode(relay,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(14, INPUT);
  dht.begin();
  timer.setInterval(1000L, myTimerEvent);
}

void myTimerEvent()
{
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp = dht.readTemperature(false);     // Read temperature as Celsius
    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2, humidity);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
}

BLYNK_WRITE(V0){    //Read virtual pin 0 and turn on/off relay and led
  i = param.asInt();
  if (i == 1){
        digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
        
     }  
    else if (i == 0){
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
    }
}

void reconnectBlynk() {
    if (!Blynk.connected()) {
      if(Blynk.connect()) {
          BLYNK_LOG("Reconnected");
      }
      else {
          BLYNK_LOG("Not reconnected");
      }
 }
}


void loop()
{
  if (Blynk.connected()) {
    Blynk.run();
  }
  timer.run();
}


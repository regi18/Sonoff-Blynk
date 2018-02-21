/* 
Code written by regi18
Github: https://github.com/regi18/Sonoff-Blynk
Steemit: https://steemit.com/@regi18
*/


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "BLYNK AUTH TOKEN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "WIFI PASSWORD";

int i;                       //integer for virtual pin
int relay = 12;              //relay switch
int led = 13;                //led pin
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

BlynkTimer timer;
void checkPhysicalButton();
  
void setup()
{
  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false)
  {
    // Wait until connected
  }
  pinMode(led,OUTPUT);        //blink for visual effect that is connected
  digitalWrite(led, LOW);
  delay(450);
  digitalWrite(led, HIGH);
  delay(450);
  digitalWrite(led, LOW);
  delay(450);
  digitalWrite(led, HIGH);
  delay(450);
  digitalWrite(led, LOW);
  delay(450);
  digitalWrite(led, HIGH);
  pinMode(relay,OUTPUT);
  pinMode(14, INPUT);
  timer.setInterval(100L, checkPhysicalButton);
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

//Read virtual pin 0 and turn on/off relay and led
BLYNK_WRITE(V0){
  i = param.asInt();
  digitalWrite(relay, i);
  digitalWrite(led, i);
}

//void for check physical switch    
void checkPhysicalButton(){
    buttonState = digitalRead(14);
    if (buttonState != lastButtonState) {
       Blynk.virtualWrite(V0, !i);
       digitalWrite(led, !i);
       digitalWrite(relay, !i);
       i = !i;
    }
    lastButtonState = buttonState;
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


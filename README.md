# Sonoff-Blynk
Connect your sonoff outlet to the internet and control it using the blynk app.
There are 2 variant of the code:
* SonoffDHT-Blynk.ThingSpeak; it allow you to connect a DHT22 (or DHT11) to the sonoff, read the temperature in the blynk app and log it in Thingspeak.com 

* SonoffBlynkSwitch; it allow you  to connect a wall switch (or any switch) to the sonoff to toggle the relay


## Getting Started

### Prerequisites

Arduino IDE Preparation

```
You can follow this instructions:
https://github.com/arendst/Sonoff-Tasmota/wiki/Prerequisite#arduino-ide-preparation
```

### Installing

You'll have to change:

The WI-Fi Password and Credential, the Blynk auth token, the ThingSpeak api key, the OTA username and password

```
For example:

/* Thingspeak */
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY ="THINGSPEAK API KEY";

You have to change "THINGSPEAK API KEY"
```

## Authors

* **regi18** [Github](https://github.com/regi18)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

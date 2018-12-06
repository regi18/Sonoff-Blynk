# Sonoff-Blynk
Connect your sonoff outlet to the internet and control it using the blynk app.
There are 3 variants of the code:
* SonoffDHT-Blynk.ThingSpeak; it allow you to connect a DHT22 (or DHT11) to the sonoff, read the temperature in the blynk app and log it in Thingspeak.com 

* SonoffBlynkSwitch; it allow you  to connect a wall switch (or any switch) to the sonoff to toggle the relay

* Sonoff+Button; it's a simple sketch for just controlling a Sonoff via the Blynk app and via the physical button


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

/*** Here are the things that you have to change ***/
/***************************************************/

/* Blynk AUTH key */
char const AUTH[] = "BLYNK AUTH TOKEN";

You have to change "BLYNK AUTH TOKEN"
```

## Authors

* **regi18** [Github](https://github.com/regi18)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

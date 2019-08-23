#include <ThingerESP32.h>

#define USERNAME "afif_ishamsyah"
#define DEVICE_ID "device_001"
#define DEVICE_CREDENTIAL "U5a!bzxwZDIX"

#define SSID "Nyari Koneksi"
#define SSID_PASSWORD "Ampastahu"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//float tempCelcius = 0;
int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);

  thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["led"] << digitalPin(ledPin);

  // resource output example (i.e. reading a sensor value)
  //thing["tempCelcius"] >> outputValue(tempCelcius);

  // more details at http://docs.thinger.io/arduino/
}

void loop() {
  thing.handle();
//  float temperature = analogRead(A0);
//  float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
//  tempCelcius = voltage * 0.1;
//  float tempFahrenheit = (tempCelcius * 1.8) + 32;
}

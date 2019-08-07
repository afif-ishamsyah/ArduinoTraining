#include <ssl_client.h>
#include <WiFiClientSecure.h>

/*
 * Updates ThingSpeak.com channel
 *
 * eg GET https://api.thingspeak.com/update?api_key=3PDRNBD973HJ572E&field1=0
 *
 * To use SSL we have to compare the secure certificate as we connect as we have no
 * space for hundreds of secure certs to be held locally (like on a PC).
 *
 * Secure cert thumbprint: f9c2656cf9ef7f668bf735fe15ea829f5f55543e
 */
#include "Arduino.h"

// Your secret Thingspeak API keys and WiFi credentials
#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>


// Allows secure (HTTPS) connection too
#include <ESP32Servo.h>

// Definitions, better than using magic numbers that have no intrinsic meaning
#define WifiTimeOutSeconds 10
#define HTTPTimeOutSeconds 5

// Place this data in the config.h file too
const char* ssid = _ssid;
const char* password = _password;

// Replace with your unique Thing Speak WRITE API KEY
const char* apiKey = ThingSpeakWriteApiKey;

// Thingspeak API server
const char* server = "api.thingspeak.com";

// Thingspeak path & params on that server
const char* parameters = "/update?api_key=";

//pin for temperature input
//const int tempPin = 13;

// On a processor with limited run time memory (SRAM) store this long string in PROGMEM (see note in code below)
// #include <avr/pgmspace.h>
//const static char fingerprint[] PROGMEM = "f9 c2 65 6c f9 ef 7f 66 8b f7 35 fe 15 ea 82 9f 5f 55 54 3e";

// -----------------------------------------------------------------------
// SETUP     SETUP     SETUP     SETUP     SETUP     SETUP     SETUP
// -----------------------------------------------------------------------
void setup() {
  // Debug at same speed as the ESP32 default speed
  Serial.begin(115200);
  delay(10);

  // Connect to your local wifi (one time operation)
  connectToWifi();
}

// -----------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------
void loop() {
  // Upload the data
  uploadDataViaHTTP();

  // Only one update per minute. This is ONLY for Proof of Concept, never do this in live code
  delay(60000);
}


// -----------------------------------------------------------------------
// Establish a WiFi connection with your router
// -----------------------------------------------------------------------
void connectToWifi() {
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Try to connect 4 times a second for X seconds before timing out
  int timeout = WifiTimeOutSeconds * 4;
  while (WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }

  // Successful connection?
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect, exiting");
    // Set some LED failure here, for example
    delay(1000);
    return;
  }

  Serial.print("\nWiFi connected with (local) IP address of: ");
  Serial.println(WiFi.localIP());
}

// -----------------------------------------------------------------------
// Make a secure (HTTPS) request to Thing Speak
// -----------------------------------------------------------------------
void uploadDataViaHTTP() {

  Serial.print("Connecting to ");
  Serial.println(server);

  // Connect securely to our Thingspeak server using the certificate's thumbprint. To get this enter the server's API URL
  // in a browser and get a (probably blank) response.
  //
  // Now, in Chrome Web Browser, press F12 (Developer tools), go to the Security tab, View Certificate
  //
  // In the pop-up window go to the Details tab and scroll down to the THUMBPRINT entry. Copy its value here.
  //
  // Note this certificate thumbprint will change over time (as the certificate gets renewed) so you may want to read this
  // off an SD card or similar, so it can be changed manually without having to re-upload all the code.
  WiFiClientSecure client;
  // client.setFingerprint("f9c2656cf9ef7f668bf735fe15ea829f5f55543e");

  // If you are short of space you can use the fingerprint string in PROGMEM as an array as shown above (before SETUP)
  //client.setFingerprint(fingerprint);

  int retries = 5;
  while (!client.connect(server, 443) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!client.connected()) {
    Serial.println("Failed to connect, going back to sleep");
  }

  // get temperature
  float temperature = analogRead(A0);
  float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
  float tempCelcius = voltage * 0.1;
  Serial.print("Sending temperature: "); Serial.println(String(tempCelcius));

  // We're communicating via REST (request/response).
  // But you can use MQTT (publish/subscribe), just not in this sketch.
  // This creates a secure, encrypted (HTTPS) request (as a multi-line string) like this:
  //    GET /update?api_key=989JHJH76GJJS6&field1=42 HTTP/1.1
  //    Host: api.thingspeak.com
  //    Connection: close
  //    Newline
  Serial.print("Issue GET request to update the Channel's field value: ");
  Serial.println(parameters);
  client.print(
      String("GET ") + parameters + apiKey + "&field1=" + String(tempCelcius)
          + " HTTP/1.1\r\n" + "Host: " + server + "\r\n"
          + "Connection: close\r\n\r\n");

  // Check whether we have a response 10 times a second for X seconds
  int timeout = HTTPTimeOutSeconds * 10;
  while (!client.available() && (timeout-- > 0)) {
    delay(100);
  }

  // Did we get a response?
  if (!client.available()) {
    Serial.println("No response, exiting");
    // Set a different error LED (colour) and exit
    delay(1000);
    return;
  }

  // Display the server's response messages to serial monitor (debug window), good for initial debugging
  Serial.println("---- Server Response ----");
  while (client.available()) {
    // See https://arduino.stackexchange.com/questions/10088/what-is-the-difference-between-serial-write-and-serial-print-and-when-are-they
    Serial.write(client.read());
  }
  Serial.println("\n--- End Server Response ---");

  // Close the connection and free resources
  Serial.println("\nClosing connection");
  client.stop();
}

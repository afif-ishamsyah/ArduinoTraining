
#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingerESP32.h>

#define USERNAME "afif_ishamsyah"
#define DEVICE_ID "device_001"
#define DEVICE_CREDENTIAL "U5a!bzxwZDIX"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Replace with your network credentials
const char* ssid     = "Nyari Koneksi";
const char* password = "Ampastahu";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://18.209.76.203/LightData.php";   

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

String sensorName = "Device001";
const int ledPin = 13;
int val = 0;
int valAfter = 0;
int sensorValue = 0;
String lightStatus = "";
String deviceName = "LDR001";

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  thing.add_wifi(ssid, password);
  thing["led"] << digitalPin(ledPin);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED)
  {
    thing.handle();
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    //sensorValue = digitalRead(ldrPin);
    sensorValue = analogRead(A0);
    if(sensorValue >= 0 && sensorValue <= 300)
    {
      val = 0;
    }
    else if(sensorValue > 300)
    {
      val = 1;  
    }
    
    Serial.println(val);
    if(val != valAfter)
    { 
      if(val == 0)
      {
        lightStatus = "Off";
      }
      else
      {
        lightStatus = "On";
      }
      // Prepare your HTTP POST request data
      String httpRequestData = "name=" + deviceName + "&status=" + lightStatus + "";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
  
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      valAfter = val;
          
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();   
    } 
    
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  //delay(5000);  
}

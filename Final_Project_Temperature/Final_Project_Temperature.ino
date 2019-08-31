

#include <WiFi.h>
#include <HTTPClient.h>


// Replace with your network credentials
const char* ssid     = "Nyari Koneksi";
const char* password = "Ampastahu";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://18.209.76.203/SensorData.php";   

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

String sensorName = "Device001";
const int ledPinLow = 15;
const int ledPinNormal = 2;
const int ledPinHigh = 4;
const int buzzerPin = 13;
String statusTemp = "";
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

void setup() {
  Serial.begin(115200);
  pinMode(ledPinLow, OUTPUT);
  pinMode(ledPinNormal, OUTPUT);
  pinMode(ledPinHigh, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
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
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    digitalWrite(ledPinLow, LOW);
    digitalWrite(ledPinNormal, LOW);
    digitalWrite(ledPinHigh, LOW);
    digitalWrite(buzzerPin, LOW);
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    float temperature = analogRead(A0);
    float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
    float tempCelcius = voltage * 0.1;

    if(tempCelcius < 33)
    {
      digitalWrite(ledPinLow, HIGH);
      digitalWrite(buzzerPin, LOW);
      statusTemp = "Too Low";
    }
    else if(tempCelcius >= 33 && tempCelcius <= 35)
    {
      digitalWrite(ledPinNormal, HIGH);
      digitalWrite(buzzerPin, LOW);
      statusTemp = "Normal";
    }
    else if(tempCelcius > 35)
    {
      digitalWrite(ledPinHigh, HIGH);
      digitalWrite(buzzerPin, HIGH);
      statusTemp = "Too High";
    }
    
    // Prepare your HTTP POST request data
    String httpRequestData = "name=" + sensorName + "&temperature=" + tempCelcius + "&status=" + statusTemp + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
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
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(5000);  
}

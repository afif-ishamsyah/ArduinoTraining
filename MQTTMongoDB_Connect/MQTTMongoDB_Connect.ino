#include <WiFi.h>
#include <PubSubClient.h>
#define BROKER_IP "192.168.43.182"
#define BROKER_PORT 1883
#define WLAN_SSID "Empati 4G"
#define WLAN_PASS "ampastahu"
#define CLIENT_NAME "ANYNAME"
WiFiClient wifiClient;
PubSubClient mqttClient(BROKER_IP,BROKER_PORT,wifiClient);

void setup() {
  Serial.begin(9600);
  mqttClient.setCallback(callback);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  connectToWiFiAndBroker();
}

void connectToWiFiAndBroker() {
  Serial.print("Connecting to WIFI");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WIFI!");
  Serial.println("Connecting to broker");
  while (!mqttClient.connect(CLIENT_NAME)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to broker!");
  mqttClient.subscribe("Command/#");
}
#define MILLISEC_DELAY_BETWEEN_PUBLISH 10000
unsigned long lastTime = 0;
unsigned long currentValue = 1;
char msg[50];

void loop() {
  if (!mqttClient.connected()) {
    connectToWiFiAndBroker();
  }
  mqttClient.loop();
  if(millis() - lastTime > MILLISEC_DELAY_BETWEEN_PUBLISH) {
    float temperature = analogRead(A0);
    float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
    float tempCelcius = voltage * 0.1;
    lastTime = millis();
    Serial.println("Publishing a new value");
    snprintf (msg, 75, "%f", tempCelcius);
    Serial.println(currentValue);
    mqttClient.publish("Suhu/iot", msg);
    currentValue++;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message received: ");
  Serial.println(topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

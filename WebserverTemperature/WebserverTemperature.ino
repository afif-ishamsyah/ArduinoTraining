#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Empati 4G";
const char* password = "ampastahu";

WebServer server(80);

const int tempPin = 15;
float tempCelcius;

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handle_OnConnect);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  float temperature = analogRead(tempPin);
  float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
  tempCelcius = voltage * 0.1;
}

String SendHTML(){
  String teks = "<!DOCTYPE html> <html>\n";

  teks += "<head><meta http-equiv=\"refresh\" content=\"6\"></head>";
  teks += "<body><h1>" + String(tempCelcius) + "</h1></body>";

  teks += "</html>";
  
  return teks;
}

#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

//define the pins used by the transceiver module

#define ss 5
#define rst 14
#define dio0 2

const char* ssid     = "xxxxxx";
const char* password = "xxxxxx";
String LoRaData;

WiFiServer server(80);

void setup() {
//initialize Serial Monitor
  Serial.begin(115200);

  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module

  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF5);
  Serial.println("LoRa Initializing OK!");

  //set server
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  // try to parse packet
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
  // received a packet
    Serial.print("Received packet '");

  // read packet
    while (LoRa.available()) {
       LoRaData = LoRa.readString();
       Serial.print(LoRaData); 
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /index")) {
                       // GET /H turns the LED off
                        client.println(SendHTML(LoRaData));
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

String SendHTML(String s){
  String teks = "<!DOCTYPE html> <html>\n";

  teks += "<head><meta http-equiv=\"refresh\" content=\"1\"></head>";
  teks += "<body><h1>" + String(s) + "</h1></body>";

  teks += "</html>";
  
  return teks;
}

#include <WiFi.h>

const char* ssid     = "Empati 4G";
const char* password = "ampastahu";
const int ledPin = 13;
const int buzzerPin = 15;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    delay(10);

    Serial.println();
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


void loop(){
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
        if (currentLine.endsWith("GET /on")) {
                       // GET /H turns the LED on
                        digitalWrite(ledPin, HIGH);
                        client.println("LED ON");
        }
        else if (currentLine.endsWith("GET /off")) {
                       // GET /H turns the LED off
                        digitalWrite(ledPin, LOW);
                        client.println("LED OFF");
        }
        else if (currentLine.endsWith("GET /yes")) {
                       // GET /H turns the Buzzer on
                        digitalWrite(buzzerPin, HIGH);
                        client.println("SOUND ON");
        }
        else if (currentLine.endsWith("GET /no")) {
                       // GET /H turns the Buzzer off
                        digitalWrite(buzzerPin, LOW);
                        client.println("SOUND OFF");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

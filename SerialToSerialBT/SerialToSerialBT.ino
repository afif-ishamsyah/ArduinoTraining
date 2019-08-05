#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
const int ledPin = 25;
const int tempPin = 13;

String message = ""; char incomingChar; //pesan masuk
float suhu; // Variabel suhu  
// Timer: untuk sampling suhu
unsigned long previousMillis = 0;
const long interval = 10000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(tempPin, INPUT); // pin output LM-35
  Serial.begin(115200);
  // Bluetooth device name
  SerialBT.begin("Siluman Armadillo");
  Serial.println("The device started, now you can pair it with bluetooth!");
}
void loop() {
  unsigned long currentMillis = millis();
  // Kirim pembacaan temperatur
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    suhu = (analogRead(tempPin)/2048.0)*3300;
    float suhuCelcius = suhu * 0.1;
    SerialBT.print(suhuCelcius);
    SerialBT.print("C ");
    float suhuFahrenheit = (suhuCelcius * 1.8) + 32;
    SerialBT.println(suhuFahrenheit);
    SerialBT.print("F"); 
  }
  // Baca pesan dari Bluetooth (LED control command)
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
     if (incomingChar != '\n'){
        message += String(incomingChar);
    }
    else{
        message = "";
    }
    Serial.write(incomingChar);
  }
  // Cek pesan yang diterima dan lakukan aksi pada LED
  if (message =="led_on"){
    digitalWrite(ledPin, HIGH);
  }
  else if (message =="led_off"){
    digitalWrite(ledPin, LOW);
  }
  delay(20);
}

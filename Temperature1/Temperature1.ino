//float vref = 3.3;
//float resolution = vref/2048;
const int tempPin = 13;
const int ledPin = 15;
const int ledPin2 = 2;
const int buzzerPin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = analogRead(tempPin);
  float voltage = (temperature / 2048.0) * 3300; // 5000 to get millivots.
  float tempCelcius = voltage * 0.1;
  float tempFahrenheit = (tempCelcius * 1.8) + 32;
  //temperature = (temperature*resolution);
  //temperature = temperature*100;


  
  if(tempCelcius < 35)
  {
    digitalWrite(ledPin, HIGH);   
                                                           
    delay(300);
      
    digitalWrite(ledPin, LOW);
    delay(300);
    
  }
  else if(tempCelcius >= 35 && tempCelcius <=40)
  {
    digitalWrite(ledPin2, HIGH);   
                                                           
    delay(300);
      
    digitalWrite(ledPin2, LOW);
    delay(300);
    
  }
  else if(tempCelcius > 40)
  {
    digitalWrite(buzzerPin, HIGH);   
                                                           
    delay(300);
      
    digitalWrite(buzzerPin, LOW);
    delay(300);
    
  }

Serial.println("Celcius : ");
Serial.println(tempCelcius);
//  Serial.println("Fahrenheit : ");
//  Serial.println(tempFahrenheit);
//  delay(1000);
}

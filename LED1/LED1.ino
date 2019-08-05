const int ledPin = 16; 
const int ledPin2 = 17; 
const int freq = 10000;
const int ledChannel = 0;
const int ledChannel2 = 1;
const int resolution = 8;
 
void setup(){

  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
 
  ledcAttachPin(ledPin, ledChannel);
  ledcAttachPin(ledPin2, ledChannel2);
  
}
 
void loop(){

  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
   
    ledcWrite(ledChannel, dutyCycle);
    
  }

  delay(500);
    
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    
    ledcWrite(ledChannel, dutyCycle);   
    
  }
  delay(500);

   for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
   
    ledcWrite(ledChannel2, dutyCycle);
    
  }

  delay(500);
    
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    
    ledcWrite(ledChannel2, dutyCycle);   
    
  }
  delay(500);
}

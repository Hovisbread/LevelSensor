#include <Wire.h>

int i2c_address = 40;


void setup(){
  Serial.begin(9600);
  //Serial.setTimeout(1000);
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}


void loop()
{
  char newData[4];
    
  Wire.requestFrom(i2c_address,4);
  //Serial.println(Wire.available()); 
  for(int i=0; i<4; i++){
    Serial.println(Wire.available());
    newData[i] = Wire.read();  
  }
  Serial.println("The data is: " + String(newData));
  /*if(Wire.available()){
    byte1 = Wire.read();
    byte2 = Wire.read();  
    
  }
  rawData = String(byte1) + String(byte2);
  Serial.println(rawData);
  */

  
  if(digitalRead(LED_BUILTIN) == LOW){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(1000);
}

#include <Wire.h>
#include <TimerThree.h>

//global variables
int i2c_address = 0x28; //address for differential sensor
//int i2c_address = 0x38; //address for gauge sensor
int sample_rate = 100000; //i2c polling rate in microseconds (set to default 100ms)
const int led = LED_BUILTIN;  // the pin with a LED

void setup() {
  Serial.begin(9600); //open USB serial port for debug printout
  Serial1.begin(9600); //open HW UART 1 for embedded comms
  Wire.begin(); // join i2c bus as master
  pinMode(led, OUTPUT); //enable LED for use to visualize sample timing
  Timer3.initialize(sample_rate); //initialize timer fire at sample_rate
  Timer3.attachInterrupt(takeSample); //attach timer to takeSample ISR
}

int sampleFlag = false; //sample flag will notify main loop when it should request data
int overflowFlag = false; //set in the event that another sample is requested before previous one is processed

void takeSample(void){
  if(sampleFlag == true){
    overflowFlag = true;
  }
  else{
    sampleFlag = true; //set sample flag
    digitalWrite(led, HIGH); //turn on LED when timer fires, will turn off when all data is received
  }
}


void loop() {
  byte newSample[4]; //an array to hold the most recent sample
  
  // Test to see if an overflow has occured. Shut off interupts if it has
  if(overflowFlag == true){ 
    Timer3.detachInterrupt();
    overflowFlag = false; //disable overflow flag
    Serial.println("A data overflow has occured. Sample rate is too fast!");
  }

  //Poll for sample request
  if(sampleFlag == true){
    Wire.requestFrom(i2c_address, 4); //request 4 bytes from sensor
    int numBytes = 0; //counter for the number of bytes received from sensor

    //read out data from I2C
    for(i = 0; i<4; i++){
      newSample[i] = Wire.read(); //put data into newSample array
    }

    //test to see if all bytes were received
    if(numBytes != 3){ 
      //Serial.println("Wrong number of bytes received"); //if not all bytes are received throw error
    }

    //reset the flags
    //Serial.println(newSample);
    sampleFlag = false; //clear sample flag
    digitalWrite(led, LOW); //if all bytes received, clear LED
  }
}
  
  
  /*
  byte status = byte1 >> 6 ;
  
  byte1 = byte1 & 0x3F;
  
  int output = int(byte1 << 8 | byte2);
  
  float pressure = ((((float)output - 1638)*60)/13107);
  
  pressure *= 6.89475;
  if(true)
  {
    //Serial.print("status: ");
    //Serial.print(status);
  
    //Serial.print("Pressure: ");
    Serial.println(pressure);
    //Serial.print(" kPa");
    //Serial.println();
  }

  int incomingByte = 0;
  
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();
  
    // say what you got:
    if(incomingByte == 49)
    {
      // Serial.print("initial Value: ");
      Serial.print(index);
      Serial.print(": ");
      Serial.print(pressure);
      Serial.print("                      ");
    }
    if(incomingByte == 50)
    {
      //Serial.print("Final Value: ");
      Serial.print(pressure);
      Serial.print("\n");
      index++;
     }
   }*/

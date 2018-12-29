#include <Wire.h>
#include <TimerThree.h>

/*
* Global variables
*/
int i2c_address = 0x28; //address for differential sensor
//int i2c_address = 0x38; //address for gauge sensor
int sample_rate = 1000000; //i2c polling rate in microseconds (set to default 1000ms)
const int led = LED_BUILTIN;  // the pin with a LED
bool sampleEnable = true; //used to enable/disable sample gathering and transmitting
int sampleFlag = false; //sample flag will notify main loop when it should request data
int overflowFlag = false; //set in the event that another sample is requested before previous one is processed


/*
* Setup function
*/
void setup() {
  Serial1.begin(115200); //open HW UART 1 for embedded comms
  Wire.begin(); // join i2c bus as master
  pinMode(led, OUTPUT); //enable LED for use to visualize sample timing
  Timer3.initialize(sample_rate); //initialize timer fire at sample_rate
  Timer3.attachInterrupt(requestSample); //attach timer to takeSample ISR
}


/*
* Main loop
*/
void loop() {
  byte newSample[4]; //an array to hold the most recent sample
  byte timeSample[4]; // byte array to hold timeStamp for transmission
  unsigned long timeStamp = 0;
  
  //check for overflow condition
  if(overflowFlag == true) { 
    //Serial1.println("before overflow alert");
    overflowAlert();
  }

  //poll for sample request
  else if(sampleFlag == true && sampleEnable == true) {
    //Serial1.println("before takeSample");
    timeStamp = takeSample(newSample, 4);
    long2byteArray(timeSample, timeStamp);
    transmitSample(timeSample, newSample); //pass 4 byte timeStamp and 4 byte data
  }

}


/*
* ISR for timer peripheral
*/
void requestSample(void){
  if(sampleFlag == true){
    overflowFlag = true;
  }
  else{
    sampleFlag = true; //set sample flag
    digitalWrite(led, HIGH); //turn on LED when timer fires, will turn off when all data is received
  }
}


/*
* Notification if overflow has occured
*/
void overflowAlert(){
  sampleEnable = false; //disable sampling
  overflowFlag = false; //disable overflow flag
  Serial1.flush();
  Serial1.println("A data overflow has occured. Sample rate is too fast!");
  return;
}


/*
* Function which takes one sample from the of the PT sensor
* Bytes 0-1: Pressure
* Bytes 2-3: Temperature
*/
unsigned long takeSample(byte inputArray[], int asize){
  unsigned long currentTime = millis();
    
  Wire.requestFrom(i2c_address, asize); //request 4 bytes from sensor
    
  //read out data from I2C
  for(int i = 0; i<asize; i++){
    inputArray[i] = Wire.read(); //put data into array
  }

  return currentTime; 
}


/*
* Function to transmit raw data from sample measurement back to host
*/
//void transmitSample(byte outputArray[], unsigned long timeStamp){
void transmitSample(byte timeSample[], byte outputArray[]){
  Serial1.flush();
  //Serial1.print("Timestamp: ");
  //Serial1.print(timeStamp, DEC);
  //Serial1.print("     Data: ");

  //Transmit time stamp
  for(int i=0; i<4; i++){
    Serial1.write(timeSample[i]);
  }

  //Transmit 2 bytes pressure, 2 bytes temperature
  for(int i=0; i<4; i++){
    Serial1.write(outputArray[i]);
  }
  Serial1.print("\n");
  
  sampleFlag = false; //clear sample flag
  digitalWrite(led, LOW); //if all bytes received, clear LED
}


/*
* Function which converts long datatype into byte array for transmission
*/
void long2byteArray(byte buf[], unsigned long n){          
  buf[0] = (byte) n;
  buf[1] = (byte) (n >> 8);
  buf[2] = (byte) (n >> 16);
  buf[3] = (byte) (n >> 24);
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

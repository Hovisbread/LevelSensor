void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long test = 4294967294;
  byte buf[4];
  long2byteArray(buf, test);
  Serial1.flush();
  Serial1.print("bytes: ");
  Serial1.println(test, HEX);
  
  for(int i=0; i<4; i++){
    Serial1.write(buf[i]);
  }
  Serial1.write('\n');

  delay(1000);
}

void long2byteArray(byte buf[], unsigned long n){          
  buf[0] = (byte) n;
  buf[1] = (byte) (n >> 8);
  buf[2] = (byte) (n >> 16);
  buf[3] = (byte) (n >> 24);
}

struct testStruct{
  uint32_t id = 1;          // can identifier
  uint16_t timestamp = 0;   // FlexCAN time when message arrived
  uint8_t idhit = 2; // filter that id came from

    bool extended = 0; // identifier is extended (29-bit)
    bool remote = 1;  // remote transmission request packet type
    bool overrun = 1; // message overrun
    bool reserved = 0;
  
  uint8_t len = 8;      // length of data
  uint8_t buf[8] = {1,2,3,4,5,6,7,8 };       // data
  int8_t mb = 2;       // used to identify mailbox reception
  uint8_t bus = 0;      // used to identify where the message came from when events() is used.
 
};
testStruct ts;

byte testArray[(sizeof(ts))];

void setup() {
  Serial.begin(115200);
}

void loop() {
  testFunction();
}

void testFunction(){
     for (byte i = 0; i < sizeof(testStruct); i++) {   
         testArray[i] = ((byte*)&ts)[i];   
      }
      testWrite();
      delay (2000);
}

void testWrite(){
  Serial.write((byte*)&testArray,sizeof(testArray));
}

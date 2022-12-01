struct testStruct
{
  uint32_t id = 0;          // can identifier
  uint16_t timestamp = 0;   // FlexCAN time when message arrived
  uint8_t idhit = 0; // filter that id came from

    bool extended = 0; // identifier is extended (29-bit)
    bool remote = 0;  // remote transmission request packet type
    bool overrun = 0; // message overrun
    bool reserved = 0;
  
  uint8_t len = 8;      // length of data
  uint8_t buf[8] = { };       // data
  int8_t mb = 0;       // used to identify mailbox reception
  uint8_t bus = 0;      // used to identify where the message came from when events() is used.
};
testStruct ts;

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello");
}

void loop()
{
  testReadData();
}

void testReadData()
{
  byte *ptr = (byte*)&ts;
  if (Serial.available() >= sizeof ts)
  {
    for (byte lp = 0; lp < sizeof ts; lp++)
    {
      ptr[lp] = Serial.read();  
    }
    //Serial.println("print vars");
    Serial.println(ts.id);
    Serial.println(ts.timestamp);
    Serial.println(ts.idhit);
    Serial.println(ts.extended);
    Serial.println(ts.remote);
    Serial.println(ts.overrun);
    Serial.println(ts.reserved);
    Serial.println(ts.len);
    Serial.println(ts.buf[8]);
    Serial.println(ts.bus);
  
  }
}

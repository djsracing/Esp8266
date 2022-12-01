
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Redmi"
#define WIFI_PASSWORD "00000000"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCmD00KYTCLpZuvKNxgINjtPiNm9M3y7vc"

/* 3. Define the RTDB URL */
#define DATABASE_URL "nodemcutesting-568cd-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "harshkumbhat4@gmail.com"
#define USER_PASSWORD "Racing123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String idPath = "/id";
String timePath = "/timestamp";
String extendedPath = "/extended";
String remotePath = "/remote";
String overrunPath = "/overrun";
String reservedPath = "/reserved";
String lengthPath = "/len";
String dataPath = "/buf[8]";
String mailboxPath = "/mb";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

unsigned long sendDataPrevMillis = 0;

struct CAN_message_t{
  uint32_t id = 1;          // can identifier
  uint16_t timestamp = 2;   // FlexCAN time when message arrived
 
   byte extended = 1; // identifier is extended (29-bit)
   byte remote = 1;  // remote transmission request packet type
   byte overrun = 1; // message overrun
   byte reserved = 1;
 
  uint8_t len = 8;      // length of data
  uint8_t buf[8] = {1,2,3,4,5,6,7,8};       // data
  uint8_t mb = 1;       // used to identify mailbox reception
}CAN_message_t;


void setup()
{

    Serial.begin(115200);
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Or use legacy authenticate method
    // config.database_url = DATABASE_URL;
    // config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(&config, &auth);

    // Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);
   
}

void loop()
{

    // Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
    // in all Firebase and FirebaseJson functions, unless F() macro is not supported.

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

      readData();
    }
}

void readData(){
   byte *ptr = (byte*)&CAN_message_t;
  if (Serial.available() >= sizeof CAN_message_t)
  {
    for (byte lp = 0; lp < sizeof CAN_message_t; lp++)
    {
      ptr[lp] = Serial.read();  
    }
   parentPath= databasePath + "/" + String(CAN_message_t.timestamp);

   json.set(idPath.c_str(), String(CAN_message_t.id));
    json.set(timePath.c_str(), String( CAN_message_t.timestamp));
    json.set(extendedPath.c_str(), String( CAN_message_t.extended));
    json.set(remotePath.c_str(), String( CAN_message_t.remote));
    json.set(overrunPath.c_str(), String( CAN_message_t.overrun));
    json.set(reservedPath.c_str(), String( CAN_message_t.reserved));
    json.set(lengthPath.c_str(), String( CAN_message_t.len));
    json.set(dataPath.c_str(), String(CAN_message_t.buf[8]));
    json.set(mailboxPath.c_str(), String(CAN_message_t.mb));
    
    
    FirebaseJsonArray arr;
    arr.setFloatDigits(8);
    arr.add(CAN_message_t.buf[8]);
        Serial.print(" ");
     }
  }

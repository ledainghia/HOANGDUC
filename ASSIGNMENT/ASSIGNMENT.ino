// FINGERPRINT SENSOR USE 3V3 !!!!
// GPIO 13 -> 33 normal

// Thư viện và module được sử dụng
#include <Arduino.h>
#include <BlynkMultiClient.h>
#include <Adafruit_Fingerprint.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <WiFiClientSecure.h>
#include <Wire.h>  // must be included here so that Arduino library object file references work
#include <LiquidCrystal_I2C.h>
#include <ESP.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6H8G-hxTz"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "FO4OefQeSeEoITYvJTbu60mcO8FIl1NU"


#define touchSen 39
#define servo 26
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Khai báo Serial ảo
#define MODEM_RX 16
#define MODEM_TX 17
#define mySerial Serial2  // use for ESP32

//FINGER
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//realtime
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 3600);

// Thông tin kết nối WiFi
const char* ssid = "S6060619";
const char* pass = "khongchopass";

//FIREBASE INFORMATION
#define HOST "iot102-127bd-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define AUTH "A2CVjh34r6RfoM9maWX1wxyay1WFU0nVWaVE5XyQ	"
FirebaseData firebaseData;
String path = "/";

int btnAddFinger = 0, id = 0;
String terminalRead = "";
int wrongPassCount = 1;
int mode = 0;
bool changePass = false;
String password = "";
String keyPad = "";
bool keyMode = false;
const int ADDR = 2;      // Địa chỉ EEPROM để lưu trữ chuỗi
const int MAX_LEN = 20;  // Độ dài tối đa của chuỗi
unsigned long KEYPAD_INTERVAL = 30000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
bool keypadMode = false;
bool ok1 = false, ok2 = false, ok3 = false, ok4 = false, ok5 = false;
bool in = true;
String ids[50];
String names[50];
String times[50];
String tFingerPrint[50];
String studentCodes[50];
uint8_t fingerTemplate[512];
int count = 0;
int idtmp = 1;
String templateFingerData = "";

#define SLAVE_ADDRESS 8  // Địa chỉ của Arduino Uno

// Khai báo đối tượng WiFiClient để sử dụng trong Blynk
static WiFiClient blynkWiFiClient;
WidgetLCD lcdB(V4);
WidgetTerminal terminal(V0);
// Khai báo đối tượng BlynkTimer để định thời gian gửi dữ liệu
BlynkTimer timer;

void setup() {

  // put your setup code here, to run once:

  myservo.attach(servo);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  Serial.begin(115200);
  //password = readEEPROM();
  // if (password == "") password = "12345";
  // Serial.println("PASS " + password);

  printLCD("WAITING STARTUP", ".........");
  // Khởi tạo kết nối Serial để ghi log

  pinMode(touchSen, INPUT);

  // connect wifi
  print("Connecting to ");
  println(ssid);

  // Kết nối WiFi với SSID và Password
  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }
  // Chờ cho đến khi kết nối thành công
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    print(".");
  }

  // Thêm kết nối Blynk Client với tên "WiFi", sử dụng WiFiClient blynkWiFiClient và cổng 80
  Blynk.addClient("WiFi", blynkWiFiClient, 80);
  // Cấu hình thông tin xác thực Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);


  //connect firebase
  Firebase.begin(HOST, AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData, path)) {
    println("REASON: " + firebaseData.errorReason());
    println();
  }

  //Khởi chạy finger
  finger.begin(57600);
  delay(50);
  //check finger sensor
  if (finger.verifyPassword()) {
    println("Found fingerprint sensor!");
  } else {
    printLCD("RUN FINGER", "FAIL, PLS RESET");
    println("Did not find fingerprint sensor :(");
    println("Check sensor then reset :(");
    while (true) {
      delay(1);
    }
  }
  Serial.print("FIREBASE ");
  getPasswordFromFirebase();

  if (millis() > KEYPAD_INTERVAL) {
    currentMillis = millis();
  } else {
    currentMillis = KEYPAD_INTERVAL;
  }

  finger.emptyDatabase();
  syncFingerDataToSensor();

  printLCD("STARTUP DONE", ".........");

  //keypad.addEventListener(keypadEvent);  // Add an event listener for this keypad
  // Thiết lập chu kỳ thời gian cho sự kiện timer
  timer.setInterval(1L, event);
  //timer.setInterval(10L, checkFinger);
  timer.setInterval(1000L, getRealTime);
}

void loop() {
  // Kiểm tra kết nối WiFi, nếu mất kết nối thì kết nối lại
  if (WiFi.status() != WL_CONNECTED) {
    print("Connecting to ");
    println(ssid);

    // Kết nối WiFi với SSID và Password
    if (pass && strlen(pass)) {
      WiFi.begin((char*)ssid, (char*)pass);
    } else {
      WiFi.begin((char*)ssid);
    }
    // Chờ cho đến khi kết nối thành công
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      print(".");
    }
    return;
  }

  // Chạy các hoạt động của Blynk
  Blynk.run();
  // Chạy timer của Blynk
  timer.run();
}

String nameSeparation(String fullName) {
  // Bỏ dấu

  Serial.println(fullName);
  // Tách họ và tên
  fullName.trim();
  String name = "";
  String middleName = "";
  int firstSpaceIndex = fullName.indexOf(' ');
  int lastSpaceIndex = fullName.lastIndexOf(' ');

  if (firstSpaceIndex != -1 && lastSpaceIndex != -1 && firstSpaceIndex != lastSpaceIndex) {
    middleName = fullName.substring(firstSpaceIndex + 1, lastSpaceIndex);
    Serial.println("midname " + middleName);
    name = fullName.substring(lastSpaceIndex + 1);
  } else {
    // Trường hợp không tìm thấy họ và tên đệm, chỉ có tên
    middleName = "";
    name = fullName;
  }
  return name;
}

void checkFinger() {
  printLCD("CHECK FINGER", "");
  id = getFingerprintID();

  if (id > 0 && !ok5) {
    getFullDataFromFirebase();
    for (int i = 0; i < count; i++) {
      Serial.println(ids[i].substring(2));
      if (id == ids[i].substring(2).toInt()) {
        println("WELCOME " + names[i]);

        String strtime = getRealTime();
        printLCD("WELCOME " + nameSeparation(names[i]), strtime);
        saveToFirebaseAt(strtime, names[i], id);
        printLCD("OPEN DOOR", "");
        myservo.write(90);
        delay(5000);
        printLCD("CLOSE DOOR", "");
        delay(1000);
        myservo.write(0);
        //saveLogToFirebase("THE DOOR OPENED BY FINGERPRINT");
        delay(1500);
        return;
      }
    }
  } else {
    println("Your fingerprint is not registered on the database");
    printLCD("ERORR!!!!", "CHECK AGAIN!");
    delay(2000);
    printLCDDefault();
  }
  id = 0;
  delay(500);
}

void updatePassword() {
  String newPass = "";
  char keyP = getKey2();
  unsigned long startTime = millis();
  const unsigned long timeoutDuration = 30000;  // 30 seconds

  while (keyP != '#') {

    keyP = getKey2();

    if (keyP != 'N') {
      if (changePass == true) {
        if (keyP == 'A') {
          newPass = newPass.substring(0, newPass.length() - 1);
        } else if (keyP == 'B') {
          newPass = "";
        } else if (keyP == '#') {
          password = newPass;
          newPass = "";
          keyP = NULL;
          Serial.println(password.toInt());

          savePassToFirebase(password);

          saveLogToFirebase("Password was changed to: " + password);
          printLCD("CHANGE SUCCESS", "");
          return;
        } else {
          newPass += keyP;
        }
        printLCD("INPUT NEW PASS", newPass);
      }

      // Kiểm tra thời gian đã trôi qua
      unsigned long currentTime = millis();
      if (currentTime - startTime >= timeoutDuration) {
        printLCD("TIMEOUT", "");
        delay(2000);
        printLCDDefault();
        return;
      }
    }
  }
}

void checkPassFromKeypad(char key) {

  if (key == '#') {
    if (mode == 1) {
      int index = keyPad.indexOf(password);
      if (index >= 0) {
        printLCD("OPEN DOOR", "");
        myservo.write(90);
        delay(5000);
        printLCD("CLOSE DOOR", "");
        delay(1000);
        myservo.write(0);
        saveLogToFirebase("THE DOOR OPENED BY PASSWORD");
        printLCDDefault();
        keyPad = "";
        wrongPassCount = 1;
        mode = 0;
      } else {
        if (wrongPassCount == 3) {

          printLCD("KEYPAD LOCK", "OPEN AFTER 5S");
          saveLogToFirebase("THE KEYPAD LOCK BECAUSE USER INPUT WRONG 3 TIME");
          wrongPassCount = 1;
          keyPad = "";
          delay(5000);
          printLCDDefault();
          mode = 0;
        } else {
          printLCD("WRONG PASS", "WRONG " + String(wrongPassCount));
          saveLogToFirebase("User enters wrong password for " + String(wrongPassCount) + " time");
          keyPad = "";
          wrongPassCount++;
          delay(2000);
          printLCDDefault();
          mode = 0;
        }
      }
    }
  } else if (key == 'D') {
    printLCDDefault();
    keyMode = false;
    keyPad = "";
    mode = 0;
  } else {
    keyPad += key;
    printLCD("INPUT PASSWORD", keyPad);
    mode = 1;
  }
}

void checkPassFromKeypadUpdate(char key) {

  if (key == '#') {
    if (mode == 1) {
      int index = keyPad.indexOf(password);
      if (index >= 0) {
        printLCD("Change Pass", "");

        delay(1000);

        printLCDDefault();
        keyPad = "";
        wrongPassCount = 1;
        mode = 0;
        changePass = true;
      } else {
        if (wrongPassCount == 3) {

          printLCD("KEYPAD LOCK", "OPEN AFTER 5S");
          saveLogToFirebase("THE KEYPAD LOCK BECAUSE USER INPUT WRONG 3 TIME");
          wrongPassCount = 1;
          keyPad = "";
          delay(5000);
          printLCDDefault();
          mode = 0;
        } else {
          printLCD("WRONG PASS", "WRONG " + String(wrongPassCount));
          saveLogToFirebase("User enters wrong password for " + String(wrongPassCount) + " time");
          keyPad = "";
          wrongPassCount++;
          delay(2000);
          printLCDDefault();
          mode = 0;
        }
        changePass = false;
      }
    }
  } else if (key == 'D') {
    printLCDDefault();
    keyMode = false;
    keyPad = "";
    mode = 0;
  } else {
    keyPad += key;
    printLCD("INPUT OLD PASSWORD", keyPad);
    mode = 1;
  }
}

void addNewFingerprint() {
  while (getFingerprintEnroll() > 0 && !ok1)
    ;
  Blynk.virtualWrite(V1, 0);
}

void event() {
  if (btnAddFinger == 1 && !ok1) addNewFingerprint();
  else if (!ok5 && digitalRead(touchSen)) checkFinger();
  else {
    currentMillis = millis();

    if (getKey()) {
      previousMillis = currentMillis;
    }
    if (currentMillis - previousMillis <= KEYPAD_INTERVAL && getKey()) {
      previousMillis = currentMillis;
    }
    if (currentMillis - previousMillis > KEYPAD_INTERVAL) {
      mode = 0;
      keyPad = "";
      printLCDDefault();
    }
  }
}

IoT123456789
iot102assign@gmail.com

Tài khoản mail, blynk, firebase

BLYNK có app điện thoại đó
FIREBASE là database -> vô REALTIME DB mà xem

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6H8G-hxTz"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "FO4OefQeSeEoITYvJTbu60mcO8FIl1NU"

Sài rất nhiều thư viện :))) Cài ĐỦ thư viện nhá
KEYPAD : https://playground.arduino.cc/Code/Keypad/
ServoESP32 https://github.com/RoboticsBrno/ServoESP32/
LiquidCrystal_I2C https://github.com/johnrickman/LiquidCrystal_I2C
Firebase ESP32 https://github.com/mobizt/Firebase-ESP32
NTPClient https://github.com/arduino-libraries/NTPClient
Blynk https://blynk.io/
FingerPrint https://github.com/AsifKhan991/Adafruit-Fingerprint-Sensor-Library/tree/template_transfer_feature/examples/template_transfer


Vào preferences ở ô additional boards ... dán 2 thằng dười vào 
https://arduino.esp8266.com/stable/package_esp8266com_index.json
https://dl.espressif.com/dl/package_esp32_index.json

sài 2 boards UNO và esp32
thầy hỏi tại sao thì do thằng ESP32 giá trị hiệu điện thế ở các chân GPIO có 3.3V nên đọc giá trị keypad thường bị lỗi.

Có 2 thư mục 
ASSIGNMENT mở ASSIGNEMNT.ino để nạp code cho esp32. Lúc nạp chọn board là esp32 dev modloe gì đó
còn cái còn lại có UNO thì nạp cho UNO



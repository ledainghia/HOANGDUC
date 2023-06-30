#include <Wire.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;


#define SLAVE_ADDRESS 8  // Địa chỉ của Arduino Uno

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char keyP = "";
void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Khởi động giao tiếp I2C
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);          /* register receive event */
  Wire.onRequest(requestEvent);          /* register request event */
  keypad.addEventListener(keypadEvent);  // Add an event listener for this keypad
}

void loop() {

  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
  }


delay(100);  // Chờ 1 giây trước khi gửi lại
}

void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
  Serial.println();             /* to newline */
}

void requestEvent() {
  Wire.write(keyP); /*send string on request */
  keyP='N';
}
// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      keyP = key;
       break;


    case HOLD:
      if (key == '*') {
        keyP = 'U';
      }
      break;
  }
}
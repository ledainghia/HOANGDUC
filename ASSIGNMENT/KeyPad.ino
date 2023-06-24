bool getKey() {
  int dataToSend = 1;  // Số nguyên muốn gửi

  // Gửi số nguyên tới Arduino Uno
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(dataToSend);
  Wire.endTransmission();

  delay(100);  // Chờ 500ms

  // Đọc phản hồi từ Arduino Uno
  Wire.requestFrom(SLAVE_ADDRESS, 1);  // Yêu cầu 1 byte từ Arduino Uno
  while (Wire.available()) {
    char receivedData = Wire.read();
    if (receivedData != 'N') {
      Serial.print("Received data from Arduino Uno: ");
      Serial.println(receivedData);
      if (receivedData == 'U') {

        mode = 2;
        updatePassword();
        mode = 0;
        keyPad = "";
        return true;
      } else {
        checkPassFromKeypad(receivedData);
      }
    } else return false;
  }

  delay(100);  // Chờ 1 giây trước khi gửi lại
}

char getKey2() {
  int dataToSend = 1;  // Số nguyên muốn gửi

  // Gửi số nguyên tới Arduino Uno
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(dataToSend);
  Wire.endTransmission();

  delay(100);  // Chờ 500ms

  // Đọc phản hồi từ Arduino Uno
  char receivedData = NULL;
  Wire.requestFrom(SLAVE_ADDRESS, 1);  // Yêu cầu 1 byte từ Arduino Uno
  while (Wire.available()) {
    receivedData = Wire.read();
    if (receivedData != 'N') {
      Serial.print("Received data from Arduino Uno: ");
      Serial.println(receivedData);
    }

    delay(100);  // Chờ 1 giây trước khi gửi lại
    return receivedData;
  }
}

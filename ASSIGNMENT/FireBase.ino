void getPasswordFromFirebase() {
  if (Firebase.getString(firebaseData, path + "/password")) {
    password = firebaseData.stringData();
    Serial.println(password);
  } else {
    Serial.println(firebaseData.errorReason());
  }
}

void savePassToFirebase(String ps) {
  // Send data to FIREBASE
  Firebase.setString(firebaseData, path + "/password", ps);
  println("Save password to Firebase success");
  println("========================");
}

void saveToFirebase(String time, String name, int id, String templateFinger) {
  // Send data to FIREBASE
  Firebase.setString(firebaseData, path + "/fingerData" + "/" + String(id - 1) + "/time", time);
  Firebase.setString(firebaseData, path + "/fingerData" + "/" + String(id - 1) + "/name", name);
  Firebase.setString(firebaseData, path + "/fingerData" + "/" + String(id - 1) + "/id", "ID" + String(id));
  Firebase.setString(firebaseData, path + "/fingerData" + "/" + String(id - 1) + "/templateFinger", templateFinger);
  println("========================");
  println("ID" + String(id));
  println("Name: " + name);
  println("Time: " + time);
  println("Template Fingerprint" + templateFinger);
  println("Save to Firebase success");
  println("========================");
}



void saveToFirebaseAt(String time, String name, int id) {
  // Gửi dữ liệu lên Firebase
  FirebaseJson json;
  json.add("time", time);
  json.add("name", name);
  json.add("log","THE DOOR OPENED BY FINGERPRINT");
  json.add("id", "ID" + String(id));
  Firebase.pushJSON(firebaseData, path + "/logOpenDoor", json);
  println("========================");
  println("ID" + String(id));
  println("Name: " + name);
  println("Time: " + time);
  println("Save to Firebase success");
  println("========================");
}

void saveLogToFirebase( String log) {
  // Gửi dữ liệu lên Firebase
  String time= getRealTime();
  FirebaseJson json;
  json.add("time", time);
  json.add("log", log);

  Firebase.pushJSON(firebaseData, path + "/logOpenDoor", json);
  println("========================");
  println("log: " + log);
  println("Time: " + time);
  println("Save to Firebase success");
  println("========================");
}

//Tách chuỗi JSON
void parseJSON(const String& jsonString) {

  //Serial.println(jsonString);
  //   // Bỏ qua ký tự đầu '[' và cuối ']'
  String trimmedJson = jsonString.substring(1, jsonString.length() - 1);

  // Lưu trữ số lượng đối tượng JSON
  count = 0;

  memset(ids, 0, sizeof(ids) / sizeof(ids[0]));
  memset(names, 0, sizeof(names) / sizeof(names[0]));
  memset(times, 0, sizeof(times) / sizeof(times[0]));
  memset(tFingerPrint, 0, sizeof(tFingerPrint) / sizeof(tFingerPrint[0]));

  // Tìm các dấu '{' trong chuỗi JSON để đếm số lượng đối tượng
  for (int i = 0; i < trimmedJson.length(); i++) {
    if (trimmedJson.charAt(i) == '{') {
      count++;
    }
  }

  // Khởi tạo mảng các biến để lưu trữ giá trị từ chuỗi JSON



  // Lưu trữ chỉ mục hiện tại trong mảng
  int currentIndex = 0;
  Serial.println(" deptrai");
  // Bỏ qua phần tử đầu tiên nếu là null
  if (trimmedJson.startsWith("null")) {
    int startIndex = trimmedJson.indexOf('{');
    if (startIndex >= 0) {
      trimmedJson = trimmedJson.substring(startIndex);
    } else {
      Serial.println("Không tìm thấy chuỗi JSON sau null.");
      return;
    }
  }

  // Tìm các đối tượng JSON và trích xuất giá trị
  int startIndex = trimmedJson.indexOf('{');
  int endIndex = trimmedJson.indexOf('}');
  while (startIndex >= 0 && endIndex >= 0) {
    String jsonObject = trimmedJson.substring(startIndex, endIndex + 1);

    // Trích xuất giá trị của trường 'id'
    int idStart = jsonObject.indexOf("\"id\":\"") + 6;
    int idEnd = jsonObject.indexOf("\",", idStart);
    ids[currentIndex] = jsonObject.substring(idStart, idEnd);

    // Trích xuất giá trị của trường 'name'
    int nameStart = jsonObject.indexOf("\"name\":\"") + 8;
    int nameEnd = jsonObject.indexOf("\",", nameStart);
    names[currentIndex] = jsonObject.substring(nameStart, nameEnd);


    // Trích xuất giá trị của trường 'templateFingerData'
    int templateFingerDataStart = jsonObject.indexOf("\"templateFinger\":\"") + 18;
    Serial.println(templateFingerDataStart);
    int templateFingerDataEnd = jsonObject.indexOf("\",", templateFingerDataStart);
    tFingerPrint[currentIndex] = jsonObject.substring(templateFingerDataStart, templateFingerDataEnd);

    // Trích xuất giá trị của trường 'time'
    int timeStart = jsonObject.indexOf("\"time\":\"") + 8;
    int timeEnd = jsonObject.indexOf("\"}", timeStart);
    times[currentIndex] = jsonObject.substring(timeStart, timeEnd);

    // Di chuyển đến vị trí bắt đầu của đối tượng JSON tiếp theo
    startIndex = trimmedJson.indexOf('{', endIndex);
    if (startIndex >= 0) {
      endIndex = trimmedJson.indexOf('}', startIndex);
      currentIndex++;
    } else {
      // Kết thúc vòng lặp nếu không còn đối tượng JSON
      break;
    }
  }
}

void getFullDataFromFirebase() {
  if (Firebase.getString(firebaseData, path + "/fingerData")) {
    parseJSON(firebaseData.stringData());
    idtmp = 1;
    for (int i = 0; i < count; i++) {
      if (idtmp == ids[i].substring(2).toInt()) {
        idtmp++;
      } else break;
    }
  } else {
    Serial.println(firebaseData.errorReason());
  }
}
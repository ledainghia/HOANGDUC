//------------------------------------------------Blynk function-------------------------------------------------
//Blynk function


BLYNK_WRITE(V1) {
  btnAddFinger = param.asInt();
  Serial.println("btnAddFinger " + String(btnAddFinger));
  ok1 = false;
}

BLYNK_WRITE(V0) {
  terminalRead = param.asStr();
  Serial.println(terminalRead);
}
String getRealTime()
{
  timeClient.update();
  time_t t = timeClient.getEpochTime();
  //Serial.println("Unix timestamp: " + String(t));
  struct tm *tm = localtime(&t);
  char date[50];
  strftime(date, sizeof(date), "%A, %B %d %Y %H:%M:%S", tm);
  //Serial.println("Week day month year format: " + String(date));
  return String(date);
}
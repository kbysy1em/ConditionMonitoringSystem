#include "util.h"

int getChecksumOf(const char *message) {
  char str[3];
  int checksum = 0;
  byte msgLength = strlen(message);
  for (int i = 1; i < msgLength; i = i + 2)
  {
    str[0] = message[i];
    str[1] = message[i + 1];
    str[2] = '\0';
    checksum += strtol(str, NULL, 16);
  }
  return 256 - (checksum % 256);
}

void toUpperStr(const char *message) {
  char *p = (char *)message;
  while (*p)
  { 
    *p = toupper(*p);
    p++;
  }
}

void writeToSerial(const char *txMessage) {
  digitalWrite(LED_PIN, HIGH);
  Serial.print("TX: ");
  Serial.println(txMessage);
  mySerial.println(txMessage);
  digitalWrite(LED_PIN, LOW);
}

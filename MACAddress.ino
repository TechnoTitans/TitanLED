#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  WiFi.mode(WIFI_STA);
  delay(500);
  Serial.println(WiFi.macAddress());
}

void loop() {}

//Mac Addresses:
// 1:
// 6:
// 8: 20:43:A8:F1:D5:F4
// 3: 88:13:BF:E5:98:38

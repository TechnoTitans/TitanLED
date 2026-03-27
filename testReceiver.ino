#include <esp_now.h>
#include <WiFi.h>

typedef struct Message {
  int ledValue;
} Message;

Message incomingMessage;

void onReceive(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingMessage, data, sizeof(incomingMessage));
  Serial.print("Received value: ");
  Serial.println(incomingMessage.ledValue);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);
  Serial.println("WiFi OK");

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  Serial.println("ESP-NOW OK");

  esp_now_register_recv_cb(onReceive);
  Serial.println("Ready to receive!");
}

void loop() {}
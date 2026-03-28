#include <esp_now.h>
#include <WiFi.h>

uint8_t receiverMAC[] = {0x88, 0x13, 0xBF, 0xE5, 0x98, 0x38};

typedef struct Message {
  int ledValue;
} Message;

Message myMessage;
esp_now_peer_info_t peerInfo;

void onSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent OK" : "Send Failed");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onSent);

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  myMessage.ledValue = random(0, 255);
  esp_now_send(receiverMAC, (uint8_t *)&myMessage, sizeof(myMessage));
  delay(2000);
}
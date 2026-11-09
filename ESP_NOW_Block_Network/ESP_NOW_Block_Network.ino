
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <string.h>

// BCA188 - Laboratory Activity 10
// ESP-NOW Block Network

#define CHANNEL 1
#define MAX_BOARDS 100

uint8_t broadcastAddress[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const char message[] = "BCA188-LAB10";

uint8_t myMAC[6];
uint8_t heardMACs[MAX_BOARDS][6];

volatile int boardCount = 0;

unsigned long lastSend = 0;
unsigned long lastDisplay = 0;

// Print MAC address
void printMAC(const uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(":");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
  }
}

// Receive messages from other boards
void onReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len
) {
  if (info == nullptr || data == nullptr) return;

  // Accept messages from the same block sketch
  if (len != sizeof(message)) return;
  if (memcmp(data, message, sizeof(message)) != 0) return;

  const uint8_t *mac = info->src_addr;

  // Ignore own MAC
  if (memcmp(mac, myMAC, 6) == 0) return;

  // Avoid counting duplicates
  for (int i = 0; i < boardCount; i++) {
    if (memcmp(heardMACs[i], mac, 6) == 0) {
      return;
    }
  }

  // Save new MAC address
  if (boardCount < MAX_BOARDS) {
    memcpy(heardMACs[boardCount], mac, 6);
    boardCount++;
  }
}

// Display results
void showResults() {
  int total = boardCount;

  Serial.println();
  Serial.println("================================");
  Serial.println("    ESP-NOW BLOCK NETWORK");
  Serial.println("================================");

  Serial.print("My Board MAC: ");
  printMAC(myMAC);
  Serial.println();

  Serial.print("Number of Boards Heard: ");
  Serial.println(total);

  Serial.println("MAC Addresses Heard:");

  if (total == 0) {
    Serial.println("No boards detected.");
  }

  for (int i = 0; i < total; i++) {
    Serial.print(i + 1);
    Serial.print(". ");
    printMAC(heardMACs[i]);
    Serial.println();
  }

  Serial.println("================================");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Set ESP-NOW channel
  if (esp_wifi_set_channel(
    CHANNEL,
    WIFI_SECOND_CHAN_NONE
  ) != ESP_OK) {
    Serial.println("Channel setup failed!");
    return;
  }

  esp_wifi_get_mac(WIFI_IF_STA, myMAC);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  // Receive callback
  esp_now_register_recv_cb(onReceive);

  // Add broadcast peer
  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = CHANNEL;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add broadcast peer!");
    return;
  }

  Serial.println("ESP-NOW Ready!");
  showResults();
}

void loop() {
  unsigned long now = millis();

  // Broadcast every 2 seconds
  if (now - lastSend >= 2000) {
    lastSend = now;

    esp_now_send(
      broadcastAddress,
      (const uint8_t *)message,
      sizeof(message)
    );
  }

  // Display results every 5 seconds
  if (now - lastDisplay >= 5000) {
    lastDisplay = now;
    showResults();
  }
}

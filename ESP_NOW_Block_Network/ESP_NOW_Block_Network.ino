#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <string.h>
#include <ctype.h>

// BCA188 - Laboratory Activity 10
// ESP-NOW Block 2 Network (B186.1)

#define CHANNEL 1
#define MAX_BOARDS 100
#define NAME_LENGTH 32

// CHANGE THIS FOR EACH STUDENT
#define STUDENT_LASTNAME "LASTNAME"

uint8_t broadcastAddress[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const char networkID[] = "B186.1-BLOCK2";

struct Message {
  char network[16];
  char lastname[NAME_LENGTH];
};

struct Board {
  uint8_t mac[6];
  char lastname[NAME_LENGTH];
};

Message outgoingMessage;
Board heardBoards[MAX_BOARDS];

uint8_t myMAC[6];
int boardCount = 0;

portMUX_TYPE boardMux = portMUX_INITIALIZER_UNLOCKED;

unsigned long lastSend = 0;
unsigned long lastDisplay = 0;

void printMAC(const uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(":");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
  }
}

int compareNames(const char *a, const char *b) {
  while (*a && *b) {
    int ca = tolower((unsigned char)*a);
    int cb = tolower((unsigned char)*b);

    if (ca != cb) return ca - cb;

    a++;
    b++;
  }

  return (unsigned char)*a - (unsigned char)*b;
}

void onReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len
) {
  if (info == nullptr || data == nullptr) return;
  if (len != sizeof(Message)) return;

  Message received;
  memcpy(&received, data, sizeof(received));

  received.network[sizeof(received.network) - 1] = '\0';
  received.lastname[NAME_LENGTH - 1] = '\0';

  if (strcmp(received.network, networkID) != 0) return;
  if (received.lastname[0] == '\0') return;

  const uint8_t *mac = info->src_addr;

  if (memcmp(mac, myMAC, 6) == 0) return;

  portENTER_CRITICAL(&boardMux);

  int index = -1;

  for (int i = 0; i < boardCount; i++) {
    if (memcmp(heardBoards[i].mac, mac, 6) == 0) {
      index = i;
      break;
    }
  }

  if (index == -1 && boardCount < MAX_BOARDS) {
    index = boardCount++;
    memcpy(heardBoards[index].mac, mac, 6);
  }

  if (index != -1) {
    memcpy(
      heardBoards[index].lastname,
      received.lastname,
      NAME_LENGTH
    );
  }

  portEXIT_CRITICAL(&boardMux);
}

void showResults() {
  Board sortedBoards[MAX_BOARDS];
  int total;

  portENTER_CRITICAL(&boardMux);

  total = boardCount;
  memcpy(sortedBoards, heardBoards, total * sizeof(Board));

  portEXIT_CRITICAL(&boardMux);

  // Alphabetical sorting (A-Z)
  for (int i = 1; i < total; i++) {
    Board current = sortedBoards[i];
    int j = i - 1;

    while (
      j >= 0 &&
      compareNames(
        sortedBoards[j].lastname,
        current.lastname
      ) > 0
    ) {
      sortedBoards[j + 1] = sortedBoards[j];
      j--;
    }

    sortedBoards[j + 1] = current;
  }

  Serial.println();
  Serial.println("================================");
  Serial.println(" ESP-NOW BLOCK 2 NETWORK (B186.1)");
  Serial.println("================================");

  Serial.print("Name: ");
  Serial.println(STUDENT_LASTNAME);

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
    Serial.print(sortedBoards[i].lastname);
    Serial.print(" - ");
    printMAC(sortedBoards[i].mac);
    Serial.println();
  }

  Serial.println("================================");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (
    esp_wifi_set_channel(
      CHANNEL,
      WIFI_SECOND_CHAN_NONE
    ) != ESP_OK
  ) {
    Serial.println("Channel setup failed!");
    return;
  }

  esp_wifi_get_mac(WIFI_IF_STA, myMAC);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = CHANNEL;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add broadcast peer!");
    return;
  }

  memset(&outgoingMessage, 0, sizeof(outgoingMessage));

  strncpy(
    outgoingMessage.network,
    networkID,
    sizeof(outgoingMessage.network) - 1
  );

  strncpy(
    outgoingMessage.lastname,
    STUDENT_LASTNAME,
    NAME_LENGTH - 1
  );

  Serial.println("ESP-NOW Ready!");
  showResults();
}

void loop() {
  unsigned long now = millis();

  if (now - lastSend >= 2000) {
    lastSend = now;

    esp_now_send(
      broadcastAddress,
      (const uint8_t *)&outgoingMessage,
      sizeof(outgoingMessage)
    );
  }

  if (now - lastDisplay >= 5000) {
    lastDisplay = now;
    showResults();
  }
}

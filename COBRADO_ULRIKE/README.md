# ESP-NOW Block 2 Network (B186.1)

A wireless ESP32 board discovery system using **ESP-NOW** for Laboratory Activity 10 in BCA188. It allows ESP32 boards within the same network group to automatically detect one another, exchange student names, and display their MAC addresses.

## Features

- Wireless communication using ESP-NOW.
- Automatic discovery of nearby ESP32 boards.
- Broadcast messaging without a Wi-Fi router.
- Student name and MAC address identification.
- Network ID filtering to detect only boards in the same group.
- Alphabetical sorting of detected student names (A–Z).
- Periodic message broadcasting every 2 seconds.
- Automatic results display every 5 seconds.
- Supports up to 100 detected boards.

## Requirements

- ESP32 development board
- Arduino IDE
- ESP32 board package
- USB cable
- Serial Monitor

## Configuration

Before uploading the code, change the student name:

```cpp
#define STUDENT_LASTNAME "LASTNAME"
```

Replace `"LASTNAME"` with your actual last name.

The network configuration is:

```cpp
#define CHANNEL 1
#define MAX_BOARDS 100

const char networkID[] = "B186.1-BLOCK2";
```

All ESP32 boards must use the same Wi-Fi channel and network ID to communicate.

## How It Works

1. The ESP32 initializes Wi-Fi in Station Mode.
2. ESP-NOW is initialized on Wi-Fi Channel 1.
3. The board broadcasts its student name and network ID every 2 seconds.
4. Nearby ESP32 boards receive and validate the messages.
5. Detected boards are identified using their MAC addresses.
6. The system stores detected boards and updates their information.
7. Every 5 seconds, the Serial Monitor displays the discovered boards in alphabetical order.

## Serial Monitor Output

Set the Serial Monitor baud rate to **115200**.

Example output:

```text
ESP-NOW Ready!

================================
 ESP-NOW BLOCK 2 NETWORK (B186.1)
================================
Name: LASTNAME
My Board MAC: XX:XX:XX:XX:XX:XX
Number of Boards Heard: 3
MAC Addresses Heard:
1. DELA CRUZ - XX:XX:XX:XX:XX:XX
2. REYES - XX:XX:XX:XX:XX:XX
3. SANTOS - XX:XX:XX:XX:XX:XX
================================
```

## Laboratory Information

- **Course:** BCA188
- **Activity:** Laboratory Activity 10
- **Network:** ESP-NOW Block 2 Network
- **Network ID:** B186.1-BLOCK2
- **Communication Protocol:** ESP-NOW
- **Wi-Fi Channel:** 1

## Notes

- All participating ESP32 boards must have the same network ID and channel.
- Each student must configure their own last name.
- The system uses broadcast communication, so no Wi-Fi router or internet connection is required.
- Detected boards must be within ESP-NOW communication range.

## Author

Developed for BCA188 Laboratory Activity 10.

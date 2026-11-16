# ESP32 NOW BLOCK 2 NETWORK (B186.1)

## Student Information

- **Name:** REDELOSA
- **My Board MAC Address:** `4C:C3:82:E6:1A:E8`
- **Number of Boards Heard:** 17
- **Date:** November 16, 2026
- **Block:** ESP-NOW BLOCK 2 NETWORK (B186.1)
- **Block Window:** [Insert scheduled time]

## MAC Addresses Heard

| No. | Last Name | MAC Address |
|---:|---|---|
| 1 | ANTONIO | `20:50:0D:5A:28:6C` |
| 2 | ARIAS | `BC:DD:C2:CB:EC:64` |
| 3 | CANETE | `7C:D5:44:35:0F:90` |
| 4 | COBRADO | `74:4D:BD:77:38:D0` |
| 5 | ENAD | `E0:8C:FE:5C:EA:40` |
| 6 | ESTORES | `E0:72:A1:DA:1E:94` |
| 7 | GALLARDE | `30:30:F9:20:3D:04` |
| 8 | HAYAHAY | `E0:72:A1:D4:E3:88` |
| 9 | LABOR | `E0:72:A1:D5:80:44` |
| 10 | LOMO | `E0:72:A1:DA:0F:A4` |
| 11 | MACASERO | `E0:72:A1:D6:B3:4C` |
| 12 | MARTIN | `68:09:47:59:87:94` |
| 13 | PANSOY | `20:50:0D:E6:FC:34` |
| 14 | QUITORIANO | `3C:0F:02:D3:F1:8C` |
| 15 | REUYAN | `30:76:F5:F7:5B:D8` |
| 16 | TAER | `3C:0F:02:D3:ED:E0` |
| 17 | TAER | `E0:72:A1:D4:EE:60` |

## Laboratory Description

This laboratory activity demonstrates communication between ESP32 boards using ESP-NOW without requiring a wireless router or internet connection.

The ESP32 board discovers nearby boards and records their MAC addresses through the Arduino IDE Serial Monitor.

## Results and Observation

My ESP32 successfully detected **17 boards** within the Block 2 network.

The Serial Monitor displayed my board's MAC address and the MAC addresses of the other detected ESP32 boards.

This demonstrates that ESP32 devices can discover nearby devices and exchange information using ESP-NOW communication without connecting to a Wi-Fi router.

## Submission Evidence

The following files are included in this repository:

1. `README.md` – Laboratory results and documentation.
2. `serial-monitor.png` – Screenshot of the Serial Monitor showing the detected boards and MAC addresses.
3. `serial-monitor-video.mp4` – 30-second screen recording of the Serial Monitor.

---

**Platform:** ESP32 Dev Module  
**IDE:** Arduino IDE 2.3.7  
**Communication Protocol:** ESP-NOW  
**Serial Monitor Baud Rate:** 115200

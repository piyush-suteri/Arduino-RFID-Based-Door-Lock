# RFID & Keypad Digital Door Lock
![image](https://github.com/user-attachments/assets/44426976-0580-4559-ac41-4491f668b4a1)

This project implements a secure digital door lock system using an **Arduino Uno**, an **RFID-RC522 module**, a **4x4 keypad**, and an **OLED display**. The system grants access by either scanning a pre-programmed RFID card or entering a correct password on the keypad. It features a servo motor for the locking mechanism, a buzzer for audio feedback, and an LED for visual indicators.

## Features

- **Dual Access Control:** Users can unlock the door using a designated RFID card or a 4-digit passcode.
- **Visual Feedback:** A **128x64 OLED display** shows the status, such as "Access Granted" or "Access Denied."
- **Audio Alerts:** A buzzer provides distinct sounds for successful access and failed attempts.
- **Security Lockout:** The system will lock out further attempts for 60 seconds after 5 consecutive failed access trials (either by card or keypad).
- **Easy Customization:** The correct RFID tag UID and keypad password can be easily changed within the code.

## Demo

YouTube Video: https://youtu.be/ZZptKUvmciM

## Components Required

- Arduino Uno R3
- RS522 RFID Reader/Writer Module with tags
- 4x4 Keypad Module
- SG90 Servo Motor
- 128x64 OLED Display (I2C)
- Buzzer
- LED
- Breadboard and jumper wires

## Circuit Connections

Here's how to connect the components to your Arduino Uno.

### RFID Module
The RFID module uses the SPI protocol for communication.

| RFID Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- |
| **SDA** | 10 | SPI Slave Select (SS) |
| **SCK** | 13 | SPI Clock |
| **MOSI** | 11 | SPI Master Out Slave In |
| **MISO** | 12 | SPI Master In Slave Out |
| **RST** | A3 | Reset Pin |
| **3.3V** | 3.3V | Power Supply |
| **GND** | GND | Ground |

### 4x4 Keypad
The keypad is connected using digital pins for rows and columns.

| Keypad Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- |
| **R1** | 2 | Row 1 |
| **R2** | 3 | Row 2 |
| **R3** | 4 | Row 3 |
| **R4** | 5 | Row 4 |
| **C1** | 6 | Column 1 |
| **C2** | 7 | Column 2 |
| **C3** | 8 | Column 3 |
| **C4** | 9 | Column 4 |

### Other Components
| Component | Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- | :--- |
| **OLED Display** | **SDA** | A4 | I2C Data Line |
| | **SCL** | A5 | I2C Clock Line |
| | **VCC** | 5V | Power Supply |
| | **GND** | GND | Ground |
| **SG90 Servo** | **Signal** | A2 | Controls the motor |
| | **VCC** | 5V | Power Supply |
| | **GND** | GND | Ground |
| **Buzzer** | | A0 | Audio Feedback |
| **LED** | | A1 | Visual Indicator |

## Setup Instructions

1.  **Install Libraries:** Open the Arduino IDE and go to `Sketch` -> `Include Library` -> `Manage Libraries...`. Search for and install the following libraries:
    -   **MFRC522:** by Miguel Balboa
    -   **Adafruit SSD1306:** by Adafruit
    -   **Keypad:** by Mark Stanley, Alexander Brevig
    -   **Servo:** This library is included with the Arduino IDE.
    -   **SPI:** This library is included with the Arduino IDE.

2.  **Clone the Repository:** Download this code and open it in the Arduino IDE.

3.  **Customize the Code:**
    -   Change the **`MasterTag`** variable to your desired RFID card's UID. You can find your card's UID by uncommenting the `Serial.print(tagID);` line in the `loop()` function, uploading the code, and scanning your card while monitoring the Serial Monitor.
    -   Change the **`PASSWARD`** variable to your desired 4-digit passcode.

4.  **Upload to Arduino:** Connect your Arduino Uno to your computer and upload the code.

5.  **Enjoy:** Your digital door lock system is now ready to use!
---
Developed By: Piyush Suteri

RFID-Based Smart Car Parking System:

1. Overview:

This project is a **Smart Parking System** built using **Arduino + RFID + IR sensors + Python**.
It monitors parking slots in real time, allows **authorized entry using RFID**, detects exits, and logs all events with timestamps into a CSV file.

2: Features:

i)   Real-time parking slot detection (5 slots)
ii)  RFID-based secure entry
iii) Exit detection using IR sensor
iv)  Live display on 16×2 LCD (available slots + status)
v)   Python serial integration
vi)  CSV logging of ENTRY / EXIT with timestamps

3. Components Required:

* Arduino Uno
* 5 × IR Sensors (slot detection)
* 1 × IR Sensor (exit detection)
* RC522 RFID Module
* RFID Cards/Tags
* 16×2 LCD with I2C module
* Breadboard + Jumper wires

4. Circuit Connections:

IR Sensors (Slots)

| Slot    | Arduino Pin |
| ------- | ----------- |
| S1      | D2          |
| S2      | D3          |
| S3      | D4          |
| S4      | D5          |
| S5      | D6          |
| Exit IR | D7          |

RFID (RC522)

| RC522 Pin | Arduino |
| --------- | ------- |
| VCC       | 3.3V ⚠️ |
| GND       | GND     |
| RST       | D8      |
| SDA (SS)  | D10     |
| MOSI      | D11     |
| MISO      | D12     |
| SCK       | D13     |

LCD (I2C)

| LCD Pin | Arduino |
| ------- | ------- |
| VCC     | 5V      |
| GND     | GND     |
| SDA     | A4      |
| SCL     | A5      |

5. How It Works:

1. IR sensors detect whether slots are occupied
2. LCD shows available parking slots
3. RFID card is scanned for entry
4. If authorized → entry allowed
5. Exit sensor detects car leaving
6. Arduino sends data to Python via Serial
7. Python logs ENTRY / EXIT events in CSV

6. Software Setup:

# Install Arduino IDE

Download from: https://www.arduino.cc/en/software

# Install Required Libraries

In Arduino IDE:

* MFRC522
* LiquidCrystal_I2C
* SPI (built-in)

# Upload Arduino Code

* Open `RFIDBasedCarParking.ino`
* Select Board: **Arduino Uno**
* Select correct Port
* Click **Upload**

# Python Setup

1. Install Python:

Check:

```bash
python --version
```

2. Install dependencies:

```bash
pip install pyserial
```

3. Update Serial Port:

Find your port:

```bash
ls /dev/tty.*
```

Then update in `parking.py`:

```python
PORT = '/dev/tty.usbserial-110'
```

4. Run Python Script:

```bash
python parking.py
```

7. Output:

# Terminal

```
2026-04-20 10:45:21 | ENTRY,S1,UID
2026-04-20 10:45:25 | EXIT,S1
```

# CSV File (`parking_log.csv`)

```
Time,Event
2026-04-20 10:45:21,ENTRY,S1,UID
2026-04-20 10:45:25,EXIT,S1
```

# How to Run (Quick Steps)

```bash
git clone https://github.com/tanishq-pandey5/RFID-Based-Car-Parking-System.git
cd RFID-Based-Car-Parking-System
pip install pyserial
python parking.py
```

# Troubleshooting:

i) Serial Port Busy Error:

* Close Arduino Serial Monitor
* Use `/dev/tty.usbserial-XXX` instead of `/dev/cu...`

ii) RFID Not Working:

* Check wiring
* Ensure using **3.3V (NOT 5V)**
* Verify UID in code

iii) IR Sensor Always Detecting:

* Adjust potentiometer
* Implement state-change logic

8. Future Improvements:

* Mobile app integration 
* Cloud database (Firebase/AWS) 
* Servo motor gate automation 
* Live dashboard (GUI) 

9.Demo: 

All the images are attached in image folder.

10.Author: Tanishq Pandey

# Contribute:

Feel free to fork, improve, and submit pull requests!

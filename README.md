# Automotive CAN Bus Dashboard System

A multi-node automotive embedded system simulating an in-vehicle network using Controller Area Network (CAN 2.0B) protocol. Built with Microchip PIC18F4580 microcontrollers in MPLAB X IDE with the XC8 compiler, this project demonstrates real-time inter-ECU communication between powertrain, body control, and central instrument cluster nodes.

---

## 📌 Architecture Overview

The system consists of three Electronic Control Units (ECUs) interconnected via a differential two-wire CAN bus:

```
                      +-----------------------------+
                      |         CAN Bus Line        |
                      |   (CAN_H / CAN_L - 8MHz)    |
                      +--+-----------+-----------+--+
                         |           |           |
            +------------+     +-----+-----+     +------------+
            |                  |                 |            |
     +------+------+    +------+------+   +------+------+
     |    ECU 1    |    |    ECU 2    |   |    ECU 3    |
     | Powertrain  |    | Body/Engine |   |  Dashboard  |
     +------+------+    +------+------+   +------+------+
            |                  |                 |
     • Speed (ADC)      • Engine RPM      • 16x2 CLCD
     • Gear (Matrix     • Indicators      • LED Flashers
       Keypad)            (Pushbuttons)   • Timer0 ISR
```

- **ECU 1 (Powertrain Node)**: Reads throttle/vehicle speed via ADC and manages transmission gear position via matrix keypad switches.
- **ECU 2 (Engine & Body Control Node)**: Measures engine RPM via ADC and captures left/right turn indicator requests via digital push buttons.
- **ECU 3 (Central Instrument Cluster / Dashboard)**: Listens to incoming CAN frames, parses telemetry data, updates the 16x2 Character LCD (CLCD), and flashes turn signal LEDs via Timer0 interrupt service routines.

---

## 📡 CAN Bus Communication Protocol

- **Microcontroller**: Microchip PIC18F4580 with integrated ECAN module
- **Operating Frequency**: 20 MHz crystal oscillator
- **CAN Pins**: `RB2` (CAN_TX), `RB3` (CAN_RX)
- **CAN Operation Mode**: Normal Mode (Configured via `BRGCON1`, `BRGCON2`, `BRGCON3`)
- **Frame Type**: Standard 11-bit Identifier (CAN 2.0B)

### Message Identifiers & Payload Format

| CAN ID | Macro Name | Transmitter | DLC | Payload Description | Values / Encoding |
|:---:|:---|:---:|:---:|:---|:---|
| `0x10` | `SPEED_MSG_ID` | ECU 1 | 2 bytes | Vehicle Speed (km/h) | 2 ASCII characters (`"00"` – `"99"`) |
| `0x20` | `GEAR_MSG_ID` | ECU 1 | 2 bytes | Transmission Gear | 2 ASCII characters: `ON`, `GN`, `G1`, `G2`, `G3`, `G4`, `G5`, `GR`, `GC` |
| `0x30` | `RPM_MSG_ID` | ECU 2 | 4 bytes | Engine Speed (RPM) | 4 ASCII characters (`"0000"` – `"6000"`) |
| `0x40` | `ENG_TEMP_MSG_ID` | ECU 2 | 2 bytes | Engine Coolant Temp | 2 ASCII characters |
| `0x50` | `INDICATOR_MSG_ID`| ECU 2 | 1 byte | Turn Signal Indicator | ASCII character: `'1'` (Left), `'2'` (Off), `'3'` (Right) |

---

## 🧩 ECU Subsystems

### 1. ECU 1: Powertrain & Transmission (`ECU1.X`)
- **Speed Sensing**: Potentiometer connected to ADC `CHANNEL4`, scaled to 0–99 km/h.
- **Gear Selector**: 4x3 Matrix Keypad:
  - `MK_SW1`: Upshift (`GN` &rarr; `G1` &rarr; ... &rarr; `G5`)
  - `MK_SW2`: Downshift (`G5` &rarr; ... &rarr; `G1` &rarr; `GN` &rarr; `GR`)
  - `MK_SW3`: Collision Warning / Emergency (`GC`)
- Periodically broadcasts CAN frames with IDs `0x10` and `0x20`.

### 2. ECU 2: Engine & Body Control (`ECU2.X`)
- **RPM Simulation**: Potentiometer connected to ADC `CHANNEL4`, calibrated to represent 0–6000 RPM.
- **Turn Signal Switches**: Digital Keypad:
  - `SWITCH1`: Left turn signal request (`'1'`)
  - `SWITCH2`: Hazard / Indicators OFF (`'2'`)
  - `SWITCH3`: Right turn signal request (`'3'`)
- Broadcasts CAN frames with IDs `0x30` and `0x50`.

### 3. ECU 3: Dashboard Display (`ECU3_Dashboard.X`)
- **CAN Receiver**: Monitors the bus in standard receive mode (`RXB0CON`).
- **16x2 Character LCD (CLCD)**: Real-time dashboard view:
  ```text
  Line 1:  SP GR  RPM  IND 
  Line 2:  45 G3  2500 ->  
  ```
  - `SP`: Vehicle Speed in km/h
  - `GR`: Current Gear selection
  - `RPM`: Engine revolutions per minute
  - `IND`: Turn signal status (`<- `, `-> `, or `OFF`)
- **Blinker Controller**: Timer0 8-bit periodic interrupt drives synchronous blinking of indicator LEDs on `PORTB` (`RB0`/`RB1` for left, `RB6`/`RB7` for right).

---

## 🛠️ Hardware & Tools Requirements

- **Microcontrollers**: 3x PIC18F4580
- **CAN Transceivers**: 3x MCP2551 or MCP2562 (5V High-Speed CAN Transceiver)
- **Displays & Inputs**:
  - 16x2 HD44780-compatible Character LCD
  - Matrix Keypad & Pushbutton switches
  - 10kΩ Potentiometers (for Speed and RPM simulation)
  - Blinker LEDs
- **Development Environment**:
  - Microchip MPLAB X IDE (v5.35 or later)
  - Microchip XC8 C Compiler (v2.00 or later)
  - PICkit 3 / PICkit 4 / ICD 3 programmer or PICSimLab simulation platform

---

## 🚀 Building & Programming

Each folder contains an independent MPLAB X project:

1. Open **MPLAB X IDE**.
2. Select **File &rarr; Open Project...** and choose any of:
   - `ECU1.X`
   - `ECU2.X`
   - `ECU3_Dashboard.X`
3. Set the active configuration compiler to **XC8** and target device to **PIC18F4580**.
4. Build the project using `Clean and Build Project` (`Shift + F11`).
5. Flash the generated `.hex` files from `dist/default/production/` onto the respective PIC18F4580 chips.

---

## 📂 Project Directory Structure

```
Automotive_CAN_Dashboard/
├── ECU1.X/                     # ECU 1 (Speed & Gear Node)
│   ├── ecu1_main.c             # Main task loop
│   ├── ecu1_sensor.c           # Speed & Gear acquisition
│   ├── adc.c / adc.h           # ADC driver
│   ├── can.c / can.h           # ECAN driver
│   ├── matrix_keypad.c / .h    # Keypad driver
│   └── msg_id.h                # CAN Message IDs
├── ECU2.X/                     # ECU 2 (RPM & Indicator Node)
│   ├── ecu2_main.c             # Main task loop
│   ├── ecu2_sensor.c           # RPM & Indicator control
│   ├── adc.c / adc.h           # ADC driver
│   ├── can.c / can.h           # ECAN driver
│   ├── digital_keypad.c / .h   # Pushbutton switch driver
│   └── msg_id.h                # CAN Message IDs
├── ECU3_Dashboard.X/           # ECU 3 (Central Instrument Cluster)
│   ├── main.c                  # Dashboard main controller
│   ├── can.c / can.h           # ECAN receiver
│   ├── clcd.c / clcd.h         # Character LCD driver
│   ├── isr.c / isr.h           # Timer0 ISR for blinkers
│   ├── timer0.c / timer0.h     # Timer0 configuration
│   └── msg_id.h                # CAN Message IDs
└── README.md                   # Project documentation
```

---

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

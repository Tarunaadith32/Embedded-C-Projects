# DriveSense : Real-Time Intelligent Vehicle Parameter Monitoring and Diagnostics using Embedded CAN Communication

A multi-ECU embedded system built on the **NXP LPC2129 ARM7 microcontroller** that monitors critical vehicle parameters — fuel level, engine temperature, obstacle proximity, and ambient light — over a **CAN bus** and displays them on a central LCD dashboard.

---

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [ECU Breakdown](#ecu-breakdown)
  - [Fuel & Temperature ECU](#1-fuel--temperature-ecu)
  - [Distance & Lighting ECU](#2-distance--lighting-ecu)
  - [Display ECU](#3-display-ecu)
- [CAN Message Map](#can-message-map)
- [Hardware & Peripherals](#hardware--peripherals)
- [Circuit Overview](#circuit-overview)
- [Software Modules](#software-modules)
- [Display Output](#display-output)
- [Building & Flashing](#building--flashing)
- [Project Structure](#project-structure)

---

## Overview

DriveSense simulates how real automotive ECUs communicate on a shared CAN bus. Three independent LPC2129 nodes are connected on a single CAN2 bus at **125 kbps**. Two sensor ECUs continuously read vehicle parameters and broadcast them as CAN frames. One display ECU receives all frames, decodes them by message ID, and presents real-time readings on a 20×4 LCD.

This mimics the architecture used in real vehicles — where nodes like the Engine Control Module (ECM), Body Control Module (BCM), and instrument cluster all share a CAN bus.

---

## System Architecture

```
┌─────────────────────┐        ┌──────────────────────┐
│  Fuel & Temp ECU    │        │ Distance & Light ECU  │
│                     │        │                       │
│  MCP3204 (SPI ADC)  │        │  HC-SR04 Ultrasonic   │
│   CH0 → Fuel        │        │  LDR (Digital input)  │
│   CH1 → LM35 Temp   │        │                       │
│                     │        │  TX: 0x200 (Distance) │
│  TX: 0x100 (Fuel)   │        │  TX: 0x250 (Light)    │
│  TX: 0x150 (Temp)   │        │                       │
└────────┬────────────┘        └────────────┬──────────┘
         │                                  │
         └──────────────┬───────────────────┘
                        │  CAN Bus (125 kbps)
                        │
              ┌─────────┴──────────┐
              │    Display ECU     │
              │                   │
              │  RX all IDs       │
              │  20×4 LCD output  │
              └───────────────────┘
```

---

## ECU Breakdown

### 1. Fuel & Temperature ECU

**Sensors:** Resistive fuel sensor + LM35 temperature sensor, both read via the **MCP3204** (12-bit SPI ADC)

**What it does:**
- Reads fuel level from ADC Channel 0
- Reads LM35 temperature from ADC Channel 1
- Packages each reading into a CAN frame and transmits every 1 second
- Also prints raw ADC values on UART (for debug)

**Key files:** `main.c`, `spi.h`, `mcp3204.h`, `can.h`, `uart.h`

**SPI Configuration (MCP3204):**
- SPI Mode 3 (CPOL=1, CPHA=1)
- Clock = 100 kbps
- CS on P0.7 (manually toggled)
- 12-bit result → `adcval = ((hbyte & 0x0F) << 8) | lbyte`

---

### 2. Distance & Lighting ECU

**Sensors:** HC-SR04 ultrasonic sensor + LDR (light-dependent resistor)

**What it does:**
- Triggers the HC-SR04 with a 15 µs pulse on P0.3
- Measures echo pulse width using Timer1 (1 µs resolution at PCLK = 60 MHz)
- Converts pulse width to distance: `dist = T1TC / 58` (in cm)
- Reads ambient light from LDR on P0.5 (digital: HIGH = light present)
- Transmits both readings as CAN frames every 1 second

**Key files:** `main.c`, `distance.h`, `can.h`

**Distance Formula:**
```
Speed of sound ≈ 340 m/s
Round-trip time = echo pulse width
Distance (cm) = pulse_width_us / 58
```

**Timeout handling:** Returns 500 if echo never goes HIGH (no object) and 1000 if echo stays HIGH too long (out of range > 400 cm).

---

### 3. Display ECU

**Output:** 20×4 LCD (HD44780-compatible)

**What it does:**
- Listens on CAN2 bus, blocking on `can2_rx()` until a frame arrives
- Decodes the message by ID and routes it to the correct LCD line
- Shows a `LOW!!` warning and turns on P0.14 fuel LED when fuel < 20%
- Shows fan/cooling status indicator when temperature > 30°C

**Key files:** `main.c`, `display.h`, `can.h`, `lcd.h`

**LCD Layout:**
```
Line 1 (0x80): FUEL : 75%
Line 2 (0xC0): TEMP : 28.5c
Line 3 (0x94): DIST : 32cm
Line 4 (0xD4): LIGHT : ON
```

---

## CAN Message Map

| Message ID | Sender ECU         | Parameter     | Data Format                        |
|------------|--------------------|---------------|------------------------------------|
| `0x100`    | Fuel & Temp ECU    | Fuel Level    | `byte_a` = 12-bit ADC raw (CH0)    |
| `0x150`    | Fuel & Temp ECU    | Temperature   | `byte_a` + `byte_b` = 12-bit ADC (CH1) |
| `0x200`    | Distance & Light ECU | Distance    | `byte_a` + `byte_b` = cm value     |
| `0x250`    | Distance & Light ECU | Light Status | `byte_a` = 1 (ON) / 0 (OFF)       |

**Reconstruction of 16-bit values on the Display ECU:**
```c
data = (msg.byte_a | (msg.byte_b << 8));
```

---

## Hardware & Peripherals

| Component | Purpose | Interface |
|---|---|---|
| NXP LPC2129 × 3 | 3 ECU nodes | — |
| MCP3204 | 12-bit ADC for fuel & temp | SPI (CK, MOSI, MISO, CS) |
| LM35 | Temperature sensor | Analog → MCP3204 CH1 |
| Resistive fuel sensor | Fuel level simulation | Analog → MCP3204 CH0 |
| HC-SR04 | Ultrasonic distance sensor | GPIO (Trig/Echo) |
| LDR | Ambient light detection | GPIO digital input |
| HD44780 LCD (20×4) | Dashboard display | 4-bit parallel |
| SJA1000 / TJA1050 | CAN transceiver (physical layer) | CAN bus |

---

## Circuit Overview

### CAN Bus
- All three LPC2129 boards share the same CAN2 bus lines (CANH / CANL)
- CAN pins: **P0.23 (CAN2 TD)** and **P0.24 (CAN2 RD)** — configured via `PINSEL1`
- Bus speed: **125 kbps** (`C2BTR = 0x001C001D` at PCLK = 60 MHz)
- Acceptance filter set to `AFMR = 0x2` (bypass — accept all messages)

### SPI (Fuel & Temp ECU)
- P0.4 = SCK, P0.5 = MOSI, P0.6 = MISO → via `PINSEL0`
- P0.7 = CS (manual GPIO, active LOW)

### Distance Sensor (Distance & Light ECU)
- P0.3 = Trig (output), P0.4 = Echo (input)
- Timer1 prescaler = 59 → 1 µs tick at PCLK = 60 MHz

### Fuel Alert LED
- P0.14 on the Display ECU → active LOW (LED ON when fuel < 20%)

---

## Software Modules

| File | ECU | Description |
|---|---|---|
| `can.h` | All | CAN2 init, `can2_tx()`, `can2_rx()` — struct-based frame handling |
| `spi.h` | Fuel & Temp | SPI master init + byte transfer |
| `mcp3204.h` | Fuel & Temp | MCP3204 12-bit ADC read by channel |
| `uart.h` | Fuel & Temp | UART0 init, TX char/string/number/hex (debug) |
| `distance.h` | Distance & Light | HC-SR04 trigger + Timer1-based echo measurement |
| `display.h` | Display | LCD formatting for fuel %, temperature °C, distance cm, light status |
| `lcd.h` | Fuel & Temp, Display | HD44780 LCD driver (4-bit mode) |
| `delay.h` | All | `msdelay()` and `usdelay()` using Timer0 |

---

## Display Output

```
┌────────────────────┐
│ FUEL : 75%         │  ← Low fuel warning at <20%: "FUEL : 12% LOW!!"
│ TEMP : 28.50c  ❄ON │  ← Fan indicator when temp > 30°C
│ DIST : 32cm        │  ← Shows '-' when object is >50 cm away
│ LIGHT : ON         │  ← ON / OFF based on LDR
└────────────────────┘
```

---

## Building & Flashing

### Prerequisites
- Keil µVision 4 or 5
- Flash Magic (for UART ISP flashing)
- LPC2129 boards × 3 with CAN transceivers

### Steps

1. Open the `.uvproj` file inside each ECU folder in Keil µVision.
2. Build the project with `F7`. Each ECU has its own `.hex` output.
3. Flash each `.hex` to the respective LPC2129 using **Flash Magic** over UART at 9600 baud.
4. Connect all three boards on a common CAN bus (CANH/CANL with 120Ω termination resistors at both ends).
5. Power all boards and observe the Display ECU LCD update in real time.

---

## Project Structure

```
DriveSense/
│
├── Fuel and temp ecu/
│   ├── main.c          ← SPI ADC read + CAN TX (IDs 0x100, 0x150)
│   ├── can.h           ← CAN2 driver
│   ├── spi.h           ← SPI master driver
│   ├── mcp3204.h       ← 12-bit ADC read
│   ├── uart.h          ← Debug UART
│   ├── lcd.h           ← LCD driver
│   └── delay.h         ← Delay utilities
│
├── Distance and lighting ecu/
│   ├── main.c          ← Ultrasonic + LDR read + CAN TX (IDs 0x200, 0x250)
│   ├── can.h           ← CAN2 driver
│   ├── distance.h      ← HC-SR04 driver with Timer1
│   ├── ldr.h           ← LDR (placeholder / GPIO read inline in main)
│   └── delay.h         ← Delay utilities
│
└── Display ecu/
    ├── main.c          ← CAN RX + LCD display routing
    ├── can.h           ← CAN2 driver
    ├── display.h       ← Fuel/temp/dist/light display formatters
    ├── lcd.h           ← LCD driver
    └── delay.h         ← Delay utilities
```

---

## Concepts Demonstrated

- **CAN Bus communication** — multi-node broadcast with message ID filtering
- **SPI protocol** — master-mode communication with MCP3204 ADC (Mode 3)
- **Timer-based measurement** — microsecond-resolution echo timing with Timer1
- **ADC signal processing** — 12-bit conversion, percentage scaling, float temperature calculation
- **Modular embedded C** — header-only drivers, struct-based CAN frames, clean separation of sensor and display logic
- **Real-time polling** — blocking receive loop in Display ECU, 1-second transmit cycles in sensor ECUs

# Embedded C Projects — LPC2129

A collection of mini projects built using **Embedded C** on the **LPC2148 ARM7 microcontroller**, developed and compiled in **Keil µVision IDE** and tested on real hardware.

---

## Projects

| Project | Description |
|---|---|
| [GSM Enabled Industrial Conveyor Object Counting System](./GSM%20enabled%20industrial%20conveyor%20object%20counting%20system/) | IR sensor-based object counter on a conveyor belt with GSM SMS alerts and motor control |
| [Digital Dice](./Digital%20dice/) | Two-player digital dice using external interrupts and custom LCD dice face characters |
| [OTP Generator](./Otp%20generator/) | UART-triggered 6-digit OTP generator using timer-based pseudo-randomness |

---

## Hardware & Tools

- **Microcontroller:** NXP LPC2129 (ARM7TDMI-S)
- **IDE:** Keil µVision 4/5
- **Language:** Embedded C
- **Flasher:** Flash Magic (or any LPC-compatible ISP tool)

---

## Repository Structure

Each project lives in its own folder and is a self-contained Keil project. Every folder contains:

- Source files (`.c` / `.h`)
- Keil project files (`.uvproj`, `.uvopt`)
- Compiled output (`.hex`, `.axf`, `.map`)
- A `README.md` describing the project

---

## Getting Started

1. Clone or download this repository.
2. Open the `.uvproj` file inside any project folder using Keil µVision.
3. Build the project (`F7`) — the `.hex` file will be generated in the same folder.
4. Flash the `.hex` to the LPC2129 using Flash Magic over UART.

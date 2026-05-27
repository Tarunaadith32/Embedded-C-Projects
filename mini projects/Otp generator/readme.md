# OTP Generator

An LPC2129-based 6-digit OTP (One-Time Password) generator. The microcontroller listens on UART0 for a trigger character; when it receives `'1'`, it generates a 6-digit OTP derived from Timer 1 and sends it back over UART — simulating the backend side of a simple OTP authentication flow.

---

## How It Works

1. **Timer 1** runs as a fast free-running counter with a small prescaler (`T1PR = 15`), creating a high-resolution tick source.
2. **UART0** is configured at 9600 baud with a receive interrupt enabled (interrupt on RX data available).
3. When the character `'1'` is received via UART (from a terminal, PC, or another MCU), the UART ISR:
   - Calls `otpgen()` to generate 6 digits — each digit is `T1TC % 10 + '0'`, sampled sequentially.
   - Clears the previously displayed OTP on the terminal (sends 13 backspace characters).
   - Transmits `"otp: XXXXXX"` followed by a carriage return back over UART.
4. The main loop does nothing but keep the processor alive — all logic is interrupt-driven.

---

## Source Files

| File | Purpose |
|---|---|
| `main.c` | Timer 1 and UART init; UART ISR — receives trigger, calls OTP gen, sends result |
| `otp.c` | `otpgen()` — samples Timer 1 six times to build a 6-char digit array |
| `uart_config.c` / `header.h` | UART0 init (9600 baud), TX/RX helpers, VIC interrupt config, `otp_clr()` |
| `lcdfns.c` | 16×2 LCD driver (included in project; can be used to display OTP locally) |
| `delay.c` | Millisecond delay via Timer 0 |
| `Startup.s` | ARM7 startup assembly |

---

## Pin Mapping

| Signal | LPC2148 Pin |
|---|---|
| UART0 TX | P0.0 |
| UART0 RX | P0.1 |
| LCD Data bus (D0–D7) | P0.2–P0.9 |
| LCD RS | P0.10 |
| LCD EN | P0.11 |

Connect a USB-to-TTL serial adapter (or another UART device) to P0.0/P0.1 and open a terminal at **9600 8N1**.

---

## Usage

1. Flash the hex, open a serial terminal at 9600 baud.
2. Send the character `1`.
3. The terminal will display something like:
   ```
   otp: 473829
   ```
4. Send `1` again to get a new OTP.

---

## OTP Generation Note

The OTP digits are derived from `T1TC % 10` — the least-significant digit of the fast-running Timer 1 counter at the moment of generation. Because human reaction time and system timing introduce effective randomness, each press produces a different value in practice. This is **not** cryptographically secure and is intended as a learning / demonstration project only.

---

## Interrupt Configuration

| Interrupt | VIC Slot | Source |
|---|---|---|
| UART0 RX (IRQ 6) | Slot 0 | UART0 Receive Data Available |

`U0IER = (1<<1)|(1<<0)` enables both RDA and THRE interrupts; the ISR checks `U0IIR` to confirm it is an RDA event before processing.

---

## Build

Open `uartotp.uvproj` in Keil µVision and build `Target 1`. Flash `uartotp.hex` to your LPC2148 board.

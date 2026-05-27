# GSM Enabled Industrial Conveyor Object Counting System

An LPC2129-based embedded system that counts objects passing on a conveyor belt using an IR sensor, drives a DC motor, displays the live count on a 16×2 LCD, and sends an SMS alert via a GSM module whenever the belt is stopped by an external interrupt.

---

## How It Works

1. **IR sensor** on pin P0.12 detects objects — a HIGH→LOW transition on the pin increments the object count.
2. **DC motor** is driven in the forward direction via P0.13 and P0.14 (motor driver H-bridge inputs).
3. An **external interrupt (EINT0)** on P0.16 acts as an emergency stop / report trigger. A debounce filter (800 ms minimum gap using Timer 1) prevents spurious counts.
4. On interrupt, the count is shown on the LCD and the GSM module sends an SMS with the current count to the configured number.
5. After 3 seconds, the LCD reverts to the idle `OBJECT COUNTER` banner.

---

## Source Files

| File | Purpose |
|---|---|
| `main.c` | Application entry point — IR polling loop, motor init, event orchestration |
| `interrupt.c` / `int_config.h` | EINT0 ISR with 800 ms debounce; VIC configuration |
| `gsm.c` / `gsm.h` | AT command sequence to send an SMS via UART0 |
| `uart_config.c` / `uart_define.h` | UART0 init (9600 baud, 15 MHz PCLK), TX/RX, string & integer helpers |
| `lcdfns.c` / `lcd_define.h` | 16×2 LCD driver — init, cmd, data, string, integer display |
| `delay.c` / `delay.h` | Blocking millisecond delay using Timer 0 |
| `lcdint.c` | Standalone LCD test snippets (not linked in main build) |
| `Startup.s` | ARM7 startup assembly — stack setup and reset vector |

---

## Pin Mapping

| Signal | LPC2148 Pin |
|---|---|
| IR Sensor output | P0.12 |
| Motor IN1 | P0.13 |
| Motor IN2 | P0.14 |
| EINT0 (stop/report) | P0.16 |
| UART0 TX (GSM) | P0.0 |
| UART0 RX (GSM) | P0.1 |
| LCD Data bus (D0–D7) | P0.2–P0.9 |
| LCD RS | P0.10 |
| LCD EN | P0.11 |

---

## GSM / SMS

The `send_msg()` function in `gsm.c` uses standard Hayes AT commands:

```
AT
AT+CMGF=1          ← text mode
AT+CMGS="<number>" ← destination number
<message text>
<Ctrl+Z (0x1A)>    ← send
```

The destination number is hardcoded in `gsm.c`. Update the `uart_str("76959XXXXX")` line to your target number before flashing.

---

## Build

Open `Conveyor.uvproj` in Keil µVision and build `Target 1`. The compiled hex is `Conveyor.hex` — flash it to the LPC2148 via your preferred programmer (e.g., Flash Magic).

**Clock assumption:** PCLK = 15 MHz (`T0PR = 15000 - 1` → 1 ms per tick). Adjust if your board runs a different PCLK.

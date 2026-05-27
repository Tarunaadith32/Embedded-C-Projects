# Digital Dice

A two-player digital dice simulator on the LPC2129. Each player presses their own push button (wired to an external interrupt pin), and the rolled value (1–6) is instantly shown on a 16×2 LCD using custom CGRAM dice face characters.

---

## How It Works

1. **Timer 1** runs as a free-running counter from the moment the program starts.
2. When **Player 1** presses their button (EINT0 on P0.16), the ISR captures `T1TC % 6 + '1'` — effectively a random digit from `'1'` to `'6'` because the exact timer value at button press is unpredictable.
3. When **Player 2** presses their button (EINT1 on P0.14), the same logic runs independently.
4. Flags set inside the ISRs are polled in the main loop, which updates the corresponding row of the LCD with the rolled value.
5. Six custom CGRAM characters are pre-loaded at startup to display dice dot patterns for faces 1–6.

---

## Source Files

| File | Purpose |
|---|---|
| `main.c` | Init, flag polling loop, LCD updates for both players |
| `interrupt.c` | EINT0 and EINT1 ISRs; VIC configuration for both interrupts |
| `timer.c` | Timer 1 free-run setup (used as entropy source for dice) |
| `lcdfns.c` | LCD driver + CGRAM write for custom dice face characters |
| `delay.c` | Millisecond delay via Timer 0 |
| `header.h` | Shared function declarations |
| `Startup.s` | ARM7 startup assembly |

---

## Pin Mapping

| Signal | LPC2148 Pin |
|---|---|
| Player 1 button (EINT0) | P0.16 |
| Player 2 button (EINT1) | P0.14 |
| LCD Data bus (D0–D7) | P0.0–P0.7 |
| LCD RS | P0.8 |
| LCD EN | P0.9 |

---

## LCD Layout

```
Player 1 : [dice value]
Player 2 : [dice value]
```

The dice value is displayed as a custom CGRAM character matching the rolled face. Six patterns (1–6 dots) are stored in `cgram_lut[]` inside `lcdfns.c` and written to CGRAM at `lcd_init()`.

---

## Interrupt Configuration

| Interrupt | VIC Slot | Source | Trigger |
|---|---|---|---|
| EINT0 | Slot 0 | P0.16 | Level (falling) |
| EINT1 | Slot 1 | P0.14 | Level (falling) |

Both are configured as IRQ (not FIQ). `EXTMODE = 0x0` sets level-sensitive triggering; `EXTPOLAR = 0x00` sets active-low.

---

## Build

Open `digital dice.uvproj` in Keil µVision and build `Target 1`. Flash `digital dice.hex` to your LPC2148 board.

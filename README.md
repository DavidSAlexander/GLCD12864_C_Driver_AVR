# GLCD12864 Driver (AVR) — README

## Overview

This project provides a clean and modular driver for the **128x64 Graphic LCD (GLCD12864)** using AVR microcontrollers. The driver supports:

* Full display initialization
* Text rendering (page‑based)
* Bitmap rendering
* Cursor positioning
* Full screen clearing

The project also includes a demonstration application showing how to use the driver to display text and multiple bitmaps.

---

## Project Structure

```
GLCD12864/
├── Application/
│   └── main.c
├── HAL/
│   ├── Inc/
│   │   └── GLCD12864.h
│   └── Src/
│       └── GLCD12864.c
├── HAL/
│   └── Inc/
│       └── Images.h
```

---

## Features

* 128x64 GLCD control (KS0108‑type interface)
* Supports 8‑bit data bus
* Page‑based writing
* Draw full‑screen bitmaps
* Display ASCII strings
* Configurable pin‑mapping through `GLCD_Config_t`
* Clear screen / cursor control

---

## Hardware Configuration

You must configure:

* **RS, RW, EN** pins
* **CS1, CS2** chip select pins
* **RST** pin
* **Backlight pin** (optional)
* **8 data pins**

This is done through the `GLCD_Config_t` structure.

Example:

```c
GLCD_Config_t GLCD1 =
{
    .GLCD_RS_PIN.PORTx = PORTD_INDEX,
    .GLCD_RS_PIN.PINx  = PIN7_INDEX,
    .GLCD_RW_PIN.PORTx = PORTC_INDEX,
    .GLCD_RW_PIN.PINx  = PIN7_INDEX,
    .GLCD_EN_PIN.PORTx = PORTC_INDEX,
    .GLCD_EN_PIN.PINx  = PIN6_INDEX,

    .GLCD_CS1_PIN.PORTx = PORTA_INDEX,
    .GLCD_CS1_PIN.PINx  = PIN2_INDEX,
    .GLCD_CS2_PIN.PORTx = PORTB_INDEX,
    .GLCD_CS2_PIN.PINx  = PIN1_INDEX,
    .GLCD_RST_PIN.PORTx = PORTB_INDEX,
    .GLCD_RST_PIN.PINx  = PIN0_INDEX,

    .GLCD_DATA_PINS[0].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[0].PINx  = PIN5_INDEX,
    .GLCD_DATA_PINS[1].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[1].PINx  = PIN4_INDEX,
    .GLCD_DATA_PINS[2].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[2].PINx  = PIN3_INDEX,
    .GLCD_DATA_PINS[3].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[3].PINx  = PIN2_INDEX,
    .GLCD_DATA_PINS[4].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[4].PINx  = PIN1_INDEX,
    .GLCD_DATA_PINS[5].PORTx = PORTC_INDEX,
    .GLCD_DATA_PINS[5].PINx  = PIN0_INDEX,
    .GLCD_DATA_PINS[6].PORTx = PORTD_INDEX,
    .GLCD_DATA_PINS[6].PINx  = PIN6_INDEX,
    .GLCD_DATA_PINS[7].PORTx = PORTA_INDEX,
    .GLCD_DATA_PINS[7].PINx  = PIN3_INDEX
};
```

---

## Public API Reference

The following APIs are available in the GLCD12864 driver:

### **void GLCD_Init(GLCD_Config_t* GLCD)**

Initializes the GLCD module, sets cursor to home, enables chips, clears screen.

### **void GLCD_Clear(GLCD_Config_t* GLCD)**

Clears all display pages (128×64).

### **void GLCD_WriteString(GLCD_Config_t* GLCD, uint8_t Page_No, const char* Str)**

Writes a string starting from a specified **page number (0–7)**.

### **void GLCD_DrawBitmap(GLCD_Config_t* GLCD, const char *bitmap)**

Draws a **128×64 monochrome bitmap** stored in a byte array.

### **void GLCD_SetCursor(GLCD_Config_t* GLCD, uint8 x, uint8 y)**

Sets cursor column (**x**) and page (**y**).

---

## 📝 Example Usage (main.c)

```c
GLCD_Init(&GLCD1);
GLCD_Clear(&GLCD1);

GLCD_WriteString(&GLCD1, 3, "Welcome David");
_delay_ms(2000);

GLCD_Clear(&GLCD1);

while(1)
{
    GLCD_DrawBitmap(&GLCD1, img);
    _delay_ms(500);

    GLCD_DrawBitmap(&GLCD1, img1);
    _delay_ms(500);
}
```

---

## Bitmap Files

Bitmaps (Superman logo etc.) must follow the format:

* 1024 bytes (128×64 / 8)
* Stored row‑by‑row, page‑based
* Provided inside `Images.h`

Example:

```c
extern const char img[];
extern const char img2[];
```

---

## Requirements

* AVR MCU (ATmega2560, ATmega32, ATmega16, ATmega128, etc.)
* 8‑bit GLCD (KS0108 controller)
* AVR Toolchain / Microchip Studio
* DIO driver (included in MCAL layer)

---

## Notes

* Ensure proper delays for GLCD timing.
* CS1 and CS2 must correctly map left and right halves of the display.
* Backlight control is optional.

---


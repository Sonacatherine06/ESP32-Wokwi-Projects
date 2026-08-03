# RGB-LED-Control

An ESP32 project that drives a common-cathode RGB LED through three GPIO pins,
cycling the red, green, and blue channels on and off in sequence every second.
Built with the **ESP-IDF** SDK and **FreeRTOS** task primitives.

## Demo

![Wokwi simulation screenshot](Screenshot%202026-08-03%20134759.png)

## What It Does

The program configures three GPIO pins as outputs and sequentially enables each
color of an RGB LED for 1 second before turning everything off, producing a
repeating `RED → GREEN → BLUE → OFF` cycle:

| Step   | Red (GPIO 25) | Green (GPIO 26) | Blue (GPIO 27) | Serial Output |
|--------|---------------|-----------------|----------------|---------------|
| 1      | ON            | OFF             | OFF            | `RED ON`      |
| 2      | OFF           | ON              | OFF            | `GREEN ON`    |
| 3      | OFF           | OFF             | ON             | `BLUE ON`     |
| 4      | OFF           | OFF             | OFF            | `ALL OFF`     |

Each step is held for **1000 ms** (`vTaskDelay(pdMS_TO_TICKS(1000))`), and a status
message is printed to the serial console via `printf` so you can monitor the
active state in real time.

## Hardware

### Components

- ESP32 development board (e.g. ESP32 DevKit)
- Common-cathode RGB LED (common leg tied to GND)
- 3 × current-limiting resistors (220 Ω – 330 Ω, one per color channel)
- Breadboard and jumper wires

### Wiring

| RGB LED Pin | ESP32 GPIO | Label in Code |
|-------------|------------|---------------|
| Red         | GPIO 25    | `RED_PIN`     |
| Green       | GPIO 26    | `GREEN_PIN`   |
| Blue        | GPIO 27    | `BLUE_PIN`    |
| Common (C)  | GND        | —             |

> **Note:** Each color leg passes through a 220 Ω – 330 Ω resistor before reaching
> the ESP32 GPIO to limit current and protect the LED. The common cathode
> connects directly to one of the ESP32's GND pins.

## Software

### Prerequisites

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
  (v4.4 or later recommended)
- `idf.py` build tool

### Build & Flash

```bash
# From this project directory
idf.py set-target esp32
idf.py build
idf.py -p (PORT) flash
idf.py -p (PORT) monitor
```

Replace `(PORT)` with your serial port (e.g. `/dev/ttyUSB0` on Linux,
`COM3` on Windows, `/dev/cu.usbserial-*` on macOS).

### Serial Monitor Output

While running, the ESP32 prints the active state to the serial console:

```
RED ON
GREEN ON
BLUE ON
ALL OFF
RED ON
...
```

## How It Works

The application uses a single `app_main()` task — the default ESP-IDF entry
point — which runs an infinite FreeRTOS loop:

1. **Pin setup** — Each GPIO is reset with `gpio_reset_pin()` and configured as
   a push-pull output (`GPIO_MODE_OUTPUT`) via the `driver/gpio.h` driver.
2. **Sequencing** — Inside the `while (1)` loop the three channels are driven
   high/low to select one color at a time.
3. **Timing** — `vTaskDelay(pdMS_TO_TICKS(1000))` yields the CPU for 1000 ms
   between state changes, keeping the timing accurate and letting the FreeRTOS
   scheduler run other tasks if present.
4. **Logging** — `printf` writes the current state to `stdout`, which ESP-IDF
   routes to the serial console.

## Files

| File                            | Description                                   |
|---------------------------------|-----------------------------------------------|
| `rgb.ino`                       | ESP-IDF application source (entry point)      |
| `Screenshot 2026-08-03 134759.png` | Wokwi simulation capture of the circuit      |
| `README.md`                     | This file                                     |

## Troubleshooting

| Symptom                  | Likely Cause / Fix                                             |
|--------------------------|----------------------------------------------------------------|
| No serial output         | Wrong `PORT`; check `idf.py -p (PORT) monitor`                  |
| LED doesn't light        | Common cathode wired to `3V3` instead of `GND`; swap to GND     |
| Colors dim/wrong         | Resistor values too high or cathode/GPIO wiring reversed        |
| Build errors             | Run `idf.py fullclean` then rebuild; ensure ESP-IDF is sourced  |

## References

- [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)
- [FreeRTOS vTaskDelay](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/delay.html)
- [ESP32 Pinout Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/starters-guide-vscoding.html)

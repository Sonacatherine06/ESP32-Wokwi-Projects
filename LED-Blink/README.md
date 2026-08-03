# LED Blink

A simple ESP32 project that blinks an external LED connected to **GPIO 2** every **1000 ms** (1 second). Designed for [Wokwi](https://wokwi.com/) simulation but equally at home on real hardware.

---

## What It Does

The ESP32 toggles GPIO 2 **high** and **low** in an infinite loop, turning the LED on and off with a one-second delay between each transition:

```
LED ON  →  1000 ms delay  →  LED OFF  →  1000 ms delay  →  (repeat)
```

| Property        | Value        |
|-----------------|--------------|
| LED pin         | GPIO 2 (`GPIO_NUM_2`) |
| On duration     | 1000 ms      |
| Off duration    | 1000 ms      |
| Total period    | 2000 ms      |

---

## Hardware

| Component              | Quantity | Notes |
|------------------------|----------|-------|
| ESP32 DevKit-C v4      | 1        | The ESP32 board simulated in Wokwi |
| Red LED                | 1        | Through-hole through-hole, color is cosmetic |
| Breadboard             | 1        | Mini breadboard for prototyping |
| Jumper wires           | 2        | To connect the LED to the ESP32 |
| 220 Ω – 330 Ω resistor | 1        | Optional current-limiting resistor for the LED |

### Wiring

The LED is wired so that its **anode** (long leg, pin A) connects to **GPIO 2** on the ESP32, and its **cathode** (short leg, pin C) connects to **GND**:

```
ESP32 GPIO 2  ──→  LED (Anode / A)
ESP32 GND     ──→  LED (Cathode / C)
```

> **Tip:** On the ESP32 DevKit-C the on-board LED is *also* tied to GPIO 2, so the external LED will blink in sync with the built-in indicator.

---

## Code

Two source files are provided — they achieve the same behaviour with minor style differences:

### `code.ino`

Uses the ESP-IDF GPIO driver API with `printf` for serial status output. Note that `stdio.h` is **not** explicitly included (the ESP-IDF build system provides `printf` indirectly).

```cpp
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        printf("LED ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_PIN, 0);
        printf("LED OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### `sketch.ino`

Same logic, but explicitly includes the FreeRTOS headers and adds inline comments. No serial `printf` output.

```cpp
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_2

void app_main(void)
{
    // Configure GPIO as output
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_GPIO, 1);   // LED ON
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_GPIO, 0);   // LED OFF
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

| Difference            | `code.ino`                       | `sketch.ino`                          |
|-----------------------|----------------------------------|---------------------------------------|
| FreeRTOS headers      | Not included (relies on transitive includes) | Explicitly includes `FreeRTOS.h` and `task.h` |
| Serial output         | `printf("LED ON\n")` / `"LED OFF"` | None |
| Comments              | None                             | Inline comments on GPIO and each state |

---

## Simulation

This project is set up for **Wokwi**. The circuit and board layout live in [`diagram.json`](diagram.json) and the simulation can be launched in a browser:

> **Simulate this project on https://wokwi.com**
> ([Create a new ESP32 project →](https://wokwi.com/projects/new/esp32))

### Circuit Layout (`diagram.json`)

- **`board-esp32-devkit-c-v4`** — the ESP32 DevKit-C v4 microcontroller board.
- **`wokwi-breadboard-mini`** (`bb2`) — a mini breadboard for the LED.
- **`wokwi-led`** (`led1`) — a **red** LED.

Connections from the diagram:

| From               | To                | Purpose |
|--------------------|-------------------|---------|
| `esp:TX`           | `$serialMonitor:RX` | Serial monitor RX |
| `esp:RX`           | `$serialMonitor:TX` | Serial monitor TX |
| `led1:A` (anode)   | `bb2:13b.j`       | Breadboard row |
| `led1:C` (cathode) | `bb2:12b.j`       | Breadboard row |
| `led1:C` (cathode) | `esp:GND.2`       | Ground |
| `led1:A` (anode)   | `esp:2` (GPIO 2)  | LED drive signal |

---

## How to Run

### On Wokwi (simulation)

1. Open the project on [Wokwi](https://wokwi.com/).
2. Click **Start Simulation**.
3. Observe the red LED blinking on and off every second.
4. (If using `code.ino`) Open the **Serial Monitor** to see `LED ON` / `LED OFF` log messages.

### On real hardware (ESP-IDF)

1. Install the [ESP-IDF toolchain](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).
2. Copy `code.ino` (or `sketch.ino`) into your project's `main/` folder as `main.c` / `main.cpp`.
3. Build and flash:

   ```bash
   idf.py set-target esp32
   idf.py build
   idf.py -p (PORT) flash monitor
   ```

4. The LED connected to GPIO 2 should blink once per second.

---

## File Structure

```
LED-Blink/
├── README.md          # This file
├── code.ino           # Source with printf serial output
├── sketch.ino         # Source with FreeRTOS includes and comments
├── diagram.json       # Wokwi circuit diagram (board + LED + breadboard)
├── circuit.png        # Rendered circuit diagram image
└── wokwi-project.txt  # Wokwi project metadata / links
```

---

## Tips

- **GPIO 2 is special:** On most ESP32 DevKit boards GPIO 2 doubles as the on-board LED pin, so an external LED there lights in step with the built-in one.
- **Current limiting:** Always use a 220–330 Ω resistor in series with the LED when wiring to real hardware to avoid burning it out.
- **Baud rate:** When monitoring serial output (`code.ino`), connect the Wokwi serial monitor at the default ESP-IDF baud rate (typically 115200).

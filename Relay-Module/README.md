# Relay Module

A simple ESP32 project (ESP-IDF + FreeRTOS) that toggles a **5V relay** on and off every second using GPIO 23, printing the state to the serial monitor.

Designed for **[Wokwi](https://wokwi.org/)** — the online ESP32 simulator. The screenshot below shows the running simulation with the relay clicking and the serial monitor alternating between `Relay ON` and `Relay OFF`.

![Wokwi relay simulation](Screenshot%202026-08-03%20133325.png)

---

## Components

| Qty | Part                 | Notes                                   |
|-----|----------------------|-----------------------------------------|
| 1   | ESP32 DevKit         | ESP32-WROOM-32 development board        |
| 1   | 5V Relay Module      | Common 5V/1-channel relay breakout board|
| –   | Breadboard & wires   | For connecting the relay to the ESP32   |

> ⚠️ **Safety note** — A real relay switches high-voltage loads (120V/230V AC). Only build this with a sealed relay module and a properly rated load. This project uses the Wokwi simulator so no physical relay switching is required for testing.

---

## Wiring

Connect the relay module to the ESP32 as follows:

| Relay Module Pin | ESP32 Pin  |
|------------------|------------|
| `VCC`            | `5V` (or `3V3` on 3.3V relay modules) |
| `GND`            | `GND`      |
| `IN`             | `GPIO 23`  |

`GPIO 23` is also used as `RELAY_PIN` in the firmware.

---

## How It Works

The firmware (`relay.ino`) runs a single FreeRTOS task inside `app_main()`:

1. **Setup** — `GPIO 23` is reset and configured as a digital output.
2. **Loop** — The pin is driven **HIGH** (relay ON), the firmware prints `Relay ON`, then delays **1000 ms** via `vTaskDelay(pdMS_TO_TICKS(1000))`.
3. The pin is driven **LOW** (relay OFF), prints `Relay OFF`, and delays again.
4. The cycle repeats indefinitely.

Because `vTaskDelay` is a FreeRTOS call (not a blocking `sleep`), other tasks can still run concurrently — making this a good starting point for multitasking ESP-IDF projects.

---

## Code

```cpp
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RELAY_PIN GPIO_NUM_23

void app_main(void)
{
    gpio_reset_pin(RELAY_PIN);

    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(RELAY_PIN, 1);
        printf("Relay ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(RELAY_PIN, 0);
        printf("Relay OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

---

## Running

### In Wokwi

1. Open the [Wokwi ESP32 simulator](https://wokwi.org/projects/new/esp32).
2. Add an ESP32 DevKit and a relay module to the diagram.
3. Wire `IN` → `GPIO 23`, `VCC` → `5V`, `GND` → `GND`.
4. Paste the contents of `relay.ino` into the code editor.
5. Click **Start** — the relay toggles every second and the serial monitor logs each state.

### On a physical board

This project requires the **ESP-IDF** toolchain. Build and flash with:

```bash
# Initialize the build system (run once per shell)
. $HOME/esp/esp-idf/export.sh

# Configure the target
idf.py set-target esp32

# Build, flash, and monitor
idf.py -p (PORT) build flash monitor
```

Replace `(PORT)` with your ESP32's serial port (e.g. `/dev/ttyUSB0` on Linux, `COM3` on Windows).

---

## Expected Serial Output

```
Relay ON
Relay OFF
Relay ON
Relay OFF
...
```

Each line appears every **1000 ms**, matching the relay's audible click.

---

## File List

| File                                | Description                          |
|-------------------------------------|--------------------------------------|
| `relay.ino`                         | ESP-IDF firmware source              |
| `Screenshot 2026-08-03 133325.png`  | Wokwi simulation capture             |
| `README.md`                         | This file                            |

---

## License

This project is provided as-is for educational purposes.

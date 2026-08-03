# Active Buzzer

![ESP32](https://img.shields.io/badge/ESP32-ESP--IDF-20232A)
![Wokwi](https://img.shields.io/badge/Wokwi-Simulation-000000)

An ESP32 (ESP-IDF + FreeRTOS) project that drives an **active buzzer** on **GPIO 15**, toggling it on and off every second with serial monitor logging.

---

## Description

This project demonstrates basic GPIO output control on the ESP32 using the ESP-IDF SDK (not Arduino). The application configures GPIO 15 as an output and toggles a connected active buzzer every 1000 ms, producing a continuous beep-beep pattern. Each state change is logged to the serial monitor via `printf`.

---

## Hardware Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| ESP32 DevKit (ESP-IDF) | 1 | Any ESP32 board supported by ESP-IDF |
| Active Buzzer | 1 | Common 5V active buzzer module |
| Breadboard | 1 | For prototyping |
| Jumper wires | 5+ | Male-to-female |
| USB cable | 1 | For flashing and power |

---

## Pin Connections

| ESP32 Pin | Component | Notes |
|-----------|-----------|-------|
| **GPIO 15** | Buzzer (+) | Output pin — drives the buzzer signal |
| **GND** | Buzzer (−) | Common ground |

> **Note:** GPIO 15 has a pull-down resistor on many ESP32 dev kits, making it safe to use directly. On bare ESP32 modules, ensure a 10 kΩ pull-down is present if needed.

### ASCII Wiring Diagram

```
ESP32 GPIO15 ────────┬─────── Buzzer (+)
                     │
                [Active Buzzer]
                     │
ESP32 GND   ─────────┴─────── Buzzer (−)
```

---

## How It Works

The code is a standard ESP-IDF FreeRTOS application with the following flow:

1. **`app_main()`** — the ESP-IDF entry point (equivalent to Arduino's `setup()` + `loop()`) runs the main logic.
2. **Pin reset & configuration** — `gpio_reset_pin(BUZZER_PIN)` restores GPIO 15 to its default state; `gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT)` configures it as an output.
3. **Infinite toggle loop** — `gpio_set_level()` sets the pin HIGH (buzzer ON) and LOW (buzzer OFF), each with a 1-second delay via `vTaskDelay(pdMS_TO_TICKS(1000))`.
4. **Serial logging** — `printf("Buzzer ON\n")` and `printf("Buzzer OFF\n")` report each state change.

---

## Working Principle

| Step | Action | GPIO15 | Serial Output |
|------|--------|--------|---------------|
| 1 | Set HIGH | 1 (ON) | `Buzzer ON` |
| 2 | Delay 1000 ms | — | — |
| 3 | Set LOW | 0 (OFF) | `Buzzer OFF` |
| 4 | Delay 1000 ms | — | — |

---

## Expected Output (Serial Monitor)

```bash
$ idf.py -p (PORT) monitor
```

```
Buzzer ON
Buzzer OFF
Buzzer ON
Buzzer OFF
...
```

The serial monitor updates every ~2 seconds (1 s ON + 1 s OFF).

---

## File Structure

```
Active-Buzzer/
├── README.md             ← This file
├── buzzer.ino            ← ESP-IDF source code (C)
├── buzzer_circuit.png    ← Circuit diagram / simulation screenshot
└── buzzer.mp4            ← Simulation recording
```

---

## Installation & Running

### Prerequisites

- ESP-IDF v5.x (https://docs.espressif.com/projects/esp-idf/en/latest/)
- Python 3.x
- A toolchain compatible with ESP32

### Build & Flash

```bash
# Navigate to a directory containing a CMakeLists.txt or create an ESP-IDF project wrapper
# Copy the .ino file into an ESP-IDF project
./install.sh
./export.sh
idf.py set-target esp32
idf.py -p (PORT) flash monitor
```

### Wokwi Simulation

This project is designed to be simulated in [Wokwi](https://wokwi.org):

1. Open https://wokwi.com/projects/new/esp32
2. Replace `main.c` with the code from `buzzer.ino`
3. Add an **Active Buzzer** part from the Parts List, wiring its `+` pin to **GPIO 15** and `−` pin to **GND**
4. Click **Start Simulation** to see the buzzer toggle and observe the serial output

---

## Learning Outcomes

- ✅ ESP-IDF project structure (C language, `app_main`)
- ✅ GPIO configuration using `gpio_reset_pin()` and `gpio_set_direction()`
- ✅ Digital output control with `gpio_set_level()`
- ✅ FreeRTOS task delays with `vTaskDelay()` and `pdMS_TO_TICKS()`
- ✅ Serial output via `printf()` in ESP-IDF
- ✅ Active buzzer circuit fundamentals

---

## Future Improvements

- Add a configurable tone frequency using the LEDC PWM driver
- Implement non-blocking delays with FreeRTOS timers
- Add a web-based control endpoint via ESP-IDF HTTP server
- Log state changes to flash (NVS) for persistent counting

---

## License

This project is part of the **ESP32-Wokwi-Projects** repository, licensed under the MIT License.

# Potentiometer (Analog Read)

![ESP32](https://img.shields.io/badge/ESP32-ESP--IDF-20232A)
![Wokwi](https://img.shields.io/badge/Wokwi-Simulation-000000)

An ESP32 (ESP-IDF) project that reads the analog voltage of a **potentiometer** using the ESP-IDF **ADC oneshot** API and prints the raw value to the serial monitor every 500 ms.

---

## Description

This project demonstrates analog-to-digital conversion (ADC) on the ESP32 using the ESP-IDF SDK. The code reads the potentiometer's wiper voltage through **ADC1, channel 6** (GPIO 34) using the `esp_adc/adc_oneshot.h` driver and outputs the raw 12-bit ADC value (0–4095) to the serial monitor.

---

## Hardware Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| ESP32 DevKit (ESP-IDF) | 1 | Any ESP32 board supported by ESP-IDF |
| Potentiometer (10kΩ) | 1 | Linear, 3 pins |
| Breadboard | 1 | For prototyping |
| Jumper wires | 5+ | Male-to-female |
| USB cable | 1 | For flashing and power |

---

## Pin Connections

| ESP32 Pin | Component | Notes |
|-----------|-----------|-------|
| **GPIO 34** | Potentiometer (wiper) | ADC1_CH6 analog input |
| **3V3** | Potentiometer (VCC) | Power the potentiometer |
| **GND** | Potentiometer (GND) | Common ground |

> **Note:** GPIO 34 is an **input-only** pin on the ESP32 — it cannot be used as a digital output. It is mapped to ADC1 channel 6.

### ASCII Wiring Diagram

```
            3V3
             │
        ┌────┴────┐
        │   ⏚   │    (Potentiometer)
   𝜔   │  ⏚  │
   ◯   └──┬──┬──┘
        │  │  │
        │  │  GND
        │  │
        └──┴── GPIO34 (ADC1_CH6)
```

---

## How It Works

The code uses the ESP-IDF **ADC oneshot** API (the newer, simpler API compared to the legacy `adc_driver`):

1. **ADC unit initialization** — `adc_oneshot_unit_init_cfg_t` configures ADC_UNIT_1.
2. **Channel configuration** — `adc_oneshot_chan_cfg_t` sets the bit width (`ADC_BITWIDTH_DEFAULT` = 12-bit) and input attenuation (`ADC_ATTEN_DB_12` = 0–3.9 V range).
3. **Channel registration** — `adc_oneshot_config_channel()` binds `ADC_CHANNEL_6` (GPIO 34) to the handle.
4. **Read loop** — `adc_oneshot_read()` retrieves the raw ADC value, which is printed via `printf` every 500 ms using `vTaskDelay`.

---

## Working Principle

| Item | Value |
|------|-------|
| ADC Unit | ADC1 |
| ADC Channel | 6 |
| GPIO Pin | GPIO 34 |
| Bit Width | 12-bit (default) |
| Attenuation | ADC_ATTEN_DB_12 (0–3.9 V) |
| Read Interval | 500 ms |

---

## Expected Output (Serial Monitor)

```bash
$ idf.py -p (PORT) monitor
```

```
Potentiometer Value = 2048
Potentiometer Value = 2051
Potentiometer Value = 2055
...
```

Turning the potentiometer knob changes the value from **0** (fully CCW, 0 V) to **~4095** (fully CW, 3.9 V).

---

## File Structure

```
potentiometer/
├── README.md                        ← This file
├── pot.ino                          ← ESP-IDF source code (C)
└── Screenshot 2026-08-03 140230.png ← Wokwi simulation screenshot
```

---

## Installation & Running

### Prerequisites

- ESP-IDF v5.x (https://docs.espressif.com/projects/esp-idf/en/latest/)

### Build & Flash

```bash
idf.py set-target esp32
idf.py -p (PORT) flash monitor
```

### Wokwi Simulation

1. Open https://wokwi.com/projects/new/esp32
2. Replace `main.c` with the code from `pot.ino`
3. Add a **Potentiometer (10kΩ)** part from the Parts List
4. Wire:
   - `3V3` → Potentiometer VCC (left pin)
   - `GND` → Potentiometer GND (right pin)
   - `GPIO 34` → Potentiometer wiper (middle pin)
5. Click **Start Simulation** to see the potentiometer values update in the serial monitor

---

## Learning Outcomes

- ✅ ESP-IDF ADC oneshot API (`adc_oneshot.h`)
- ✅ ADC unit, channel, bit-width, and attenuation configuration
- ✅ Reading analog sensor values in ESP32 (C, not Arduino)
- ✅ FreeRTOS task delays (`vTaskDelay`, `pdMS_TO_TICKS`)
- ✅ Input-only GPIO (GPIO 34 cannot be a digital output)

---

## Future Improvements

- Map raw ADC value to voltage (V) for display
- Use the ADC value to control an LED's brightness via PWM
- Add a threshold trigger that activates an actuator when the pot exceeds a value
- Log the average value over a window to reduce ADC noise

---

## License

This project is part of the **ESP32-Wokwi-Projects** repository, licensed under the MIT License.

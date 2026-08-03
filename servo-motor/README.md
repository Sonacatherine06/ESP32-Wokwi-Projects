# Servo Motor Control (LEDC PWM)

![ESP32](https://img.shields.io/badge/ESP32-ESP--IDF-20232A)
![Wokwi](https://img.shields.io/badge/Wokwi-Simulation-000000)

An ESP32 (ESP-IDF) project that controls a **standard servo motor** on **GPIO 18** using the ESP-IDF **LEDC PWM** driver. The servo sweeps through **0° → 90° → 180°** with a 2-second pause at each position.

---

## Description

This project demonstrates pulse-width modulation (PWM) control of a hobby servo motor using the ESP-IDF SDK. Unlike Arduino's `Servo` library, ESP-IDF uses the **LEDC (LED Control)** peripheral to generate the 50 Hz PWM signal required by standard servos. The code cycles the servo through three positions: 0°, 90°, and 180°, repeating indefinitely.

---

## Hardware Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| ESP32 DevKit (ESP-IDF) | 1 | Any ESP32 board supported by ESP-IDF |
| Servo Motor (SG90) | 1 | Standard 5V hobby servo |
| Breadboard | 1 | For prototyping |
| Jumper wires | 5+ | Male-to-female |
| USB cable | 1 | For flashing and power |
| External power (optional) | — | 5V/1A+ for servo if not powered by ESP32 |

---

## Pin Connections

| ESP32 Pin | Component | Notes |
|-----------|-----------|-------|
| **GPIO 18** | Servo (Signal / Orange) | LEDC PWM output |
| **3V3** | Servo (Red / VCC) | 5V preferred; 3.3V may be weak for some servos |
| **GND** | Servo (Black / GND) | Common ground |

> **Note:** GPIO 18 is one of the ESP32's ADC2 pins but is fully usable as a digital/LEDC output. On ESP-IDF, it is configured via `LEDC_CHANNEL_0` on `LEDC_TIMER_0`.

### ASCII Wiring Diagram

```
ESP32 GPIO18 ─────── Servo Signal (Orange)
ESP32 3V3    ─────── Servo VCC (Red)
ESP32 GND    ─────── Servo GND (Black)
```

---

## PWM Configuration

| Parameter | Value | Description |
|-----------|-------|-------------|
| PWM Frequency | 50 Hz | Standard servo PWM frequency |
| Timer | LEDC_TIMER_0 | LEDC hardware timer 0 |
| Speed Mode | LEDC_LOW_SPEED_MODE | Low-speed (timer) mode |
| Duty Resolution | LEDC_TIMER_14_BIT | 14-bit resolution (0–16383) |
| Channel | LEDC_CHANNEL_0 | LEDC channel 0 |
| Clock Source | LEDC_AUTO_CLK | Auto-selected clock |

### Duty Cycle → Angle Mapping

| Angle | Duty Value | Description |
|-------|------------|-------------|
| 0° | 410 | ≈ 1 ms pulse |
| 90° | 1230 | ≈ 1.5 ms pulse |
| 180° | 2050 | ≈ 2 ms pulse |

---

## How It Works

The code uses the ESP-IDF **LEDC driver** (`driver/ledc.h`):

1. **Timer configuration** — `ledc_timer_config_t` sets up a 50 Hz PWM timer with 14-bit duty resolution.
2. **Channel configuration** — `ledc_channel_config_t` binds the timer to GPIO 18 (LEDC_CHANNEL_0, LOW_SPEED_MODE).
3. **Duty cycle update** — `ledc_set_duty()` sets the duty value (410, 1230, or 2050), and `ledc_update_duty()` applies it.
4. **Position cycling** — the infinite `while(1)` loop cycles through 0° → 90° → 180° → 0°, pausing 2 seconds at each position with `vTaskDelay(pdMS_TO_TICKS(2000))`.

---

## Working Principle

| Step | Angle | Duty Value | Duration | Serial Output |
|------|-------|------------|----------|---------------|
| 1 | 0° | 410 | 2000 ms | `Servo : 0 Degree` |
| 2 | 90° | 1230 | 2000 ms | `Servo : 90 Degree` |
| 3 | 180° | 2050 | 2000 ms | `Servo : 180 Degree` |

The servo sweeps 0 → 180°, then returns to 0°, repeating indefinitely.

---

## Expected Output (Serial Monitor)

```bash
$ idf.py -p (PORT) monitor
```

```
Servo : 0 Degree
Servo : 90 Degree
Servo : 180 Degree
Servo : 0 Degree
Servo : 90 Degree
Servo : 180 Degree
...
```

Each position holds for ~2 seconds before moving to the next.

---

## File Structure

```
servo-motor/
├── README.md              ← This file
├── wokwi/
│   ├── code.ino           ← ESP-IDF source code (C)
│   ├── esp1.png           ← Simulation screenshot
│   └── Recording.mp4      ← Simulation recording
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
2. Replace `main.c` with the code from `wokwi/code.ino`
3. Add a **Servo (SG90)** part from the Parts List
4. Wire:
   - `GPIO 18` → Servo signal (orange)
   - `3V3` or `5V` → Servo VCC (red)
   - `GND` → Servo GND (black)
5. Click **Start Simulation** to see the servo sweep

---

## Learning Outcomes

- ✅ ESP-IDF LEDC PWM driver (`driver/ledc.h`)
- ✅ Configuring LEDC timers and channels in C
- ✅ Generating servo-compatible PWM signals (50 Hz, 1–2 ms pulse)
- ✅ Mapping duty cycle values to servo angles
- ✅ FreeRTOS task delays with `vTaskDelay`

---

## Future Improvements

- Add a second servo on a different LEDC channel
- Control the servo position from a web server (ESP-IDF HTTP server)
- Use a potentiometer (analog input) to set the servo angle dynamically
- Implement non-blocking delays with FreeRTOS timers

---

## License

This project is part of the **ESP32-Wokwi-Projects** repository, licensed under the MIT License.

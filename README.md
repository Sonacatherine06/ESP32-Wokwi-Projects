# ESP32-Wokwi-Projects

A collection of **ESP32** embedded projects built with the **ESP-IDF** framework and **FreeRTOS**, each simulated on the [Wokwi](https://wokwi.com) online emulator. Every project controls a different peripheral, demonstrating GPIO, ADC, and PWM (LEDC) usage on the ESP32.

[![Wokwi](https://img.shields.io/badge/Wokwi-Simulator-blue?logo=wokwi)](https://wokwi.com/projects/new/esp32)
[![ESP-IDF](https://img.shields?label=ESP-IDF&color=000000&logo=espressif)](https://docs.espressif.com/projects/esp-idf/en/latest/)
[![License](https://img.shields.io/badge/license-MIT-green)]

---

## Projects

| # | Project | Peripheral | GPIO / Pin | Description |
|---|---------|-----------|------------|-------------|
| 1 | [LED-Blink](./LED-Blink) | LED | GPIO2 | Blinks an LED on and off every 1 second. |
| 2 | [Active-Buzzer](./Active-Buzzer) | Active Buzzer | GPIO15 | Toggles a buzzer on and off every 1 second. |
| 3 | [potentiometer](./potentiometer) | Potentiometer (ADC) | GPIO34 (ADC1_CH6) | Reads and prints the analog value of a potentiometer. |
| 4 | [Push Button + LED](./Push%20Button%20+%20LED) | Push Button + LED | GPIO2 (LED), GPIO4 (Button) | Turns an LED on when a push button is pressed. |
| 5 | [Relay-Module](./Relay-Module) | Relay | GPIO23 | Toggles a relay on and off every 1 second. |
| 6 | [RGB-LED-Control](./RGB-LED-Control) | RGB LED | GPIO25 / GPIO26 / GPIO27 | Cycles an RGB LED through red, green, and blue. |
| 7 | [servo-motor](./servo-motor) | Servo Motor | GPIO18 (LEDC PWM) | Sweeps a servo motor to 0°, 90°, and 180°. |
| 8 | [LDR](./LDR) | Light Dependent Resistor | — | Light sensor project — code pending. |

---

## Quick Start

Each subproject is an independent ESP-IDF application. To build and flash:

```bash
# 1. Install the ESP-IDF toolchain
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
. ./export.sh

# 2. Build & flash a project (example: LED-Blink)
cd ../ESP32-Wokwi-Projects/LED-Blink
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash
```

> All examples target the **ESP32** (ESP-IDF). The default entry point for every application is `app_main()`.

## Wokwi Simulation

Every project can be run directly in the browser on
[wokwi.com](https://wokwi.com/projects/new/esp32). Projects that include a
`diagram.json` (e.g. **LED-Blink**) define their own virtual circuit and
peripherals, so no hardware is required for simulation.

```
https://wokwi.com/projects/new/esp32
```

---

## Project Details

### 1. LED-Blink — GPIO2

Blinks an on-board/external LED connected to **GPIO2** with a 1-second interval
using `vTaskDelay`.

```c
#define LED_PIN GPIO_NUM_2

void app_main(void) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED_PIN, 1);   // ON
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED_PIN, 0);   // OFF
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

> Files: [`code.ino`](./LED-Blink/code.ino), [`sketch.ino`](./LED-Blink/sketch.ino), [`diagram.json`](./LED-Blink/diagram.json)

---

### 2. Active-Buzzer — GPIO15

Drives an active buzzer on **GPIO15**, turning it on and off every second.

```c
#define BUZZER_PIN GPIO_NUM_15

void app_main(void) {
    gpio_reset_pin(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(BUZZER_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(BUZZER_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

> Files: [`buzzer.ino`](./Active-Buzzer/buzzer.ino)

---

### 3. potentiometer — GPIO34 (ADC1_CH6)

Reads the analog voltage of a potentiometer using the ESP-IDF
**ADC oneshot** API and prints the raw value over the serial monitor.

```c
#define POT_CHANNEL ADC_CHANNEL_6   // GPIO34

void app_main(void) {
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten    = ADC_ATTEN_DB_12,
    };
    adc_oneshot_config_channel(adc_handle, POT_CHANNEL, &config);

    while (1) {
        adc_oneshot_read(adc_handle, POT_CHANNEL, &adc_raw);
        printf("Potentiometer Value = %d\n", adc_raw);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

> Files: [`pot.ino`](./potentiometer/pot.ino)

---

### 4. Push Button + LED — GPIO2 (LED) / GPIO4 (Button)

Uses a push button wired to **GPIO4** (with an internal pull-up) to control
an LED on **GPIO2**. Pressing the button turns the LED on.

```c
#define LED_PIN    GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4

void app_main(void) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0) {   // button pressed
            gpio_set_level(LED_PIN, 1);
        } else {
            gpio_set_level(LED_PIN, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

> Files: [`led+pushbutton.ino`](./Push%20Button%20+%20LED/led+pushbutton.ino)

---

### 5. Relay-Module — GPIO23

Toggles a relay module on **GPIO23** on and off every second.

```c
#define RELAY_PIN GPIO_NUM_23

void app_main(void) {
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(RELAY_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(RELAY_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

> Files: [`relay.ino`](./Relay-Module/relay.ino)

---

### 6. RGB-LED-Control — GPIO25 / GPIO26 / GPIO27

Cycles a common-cathode RGB LED through red, green, and blue channels
connected to **GPIO25**, **GPIO26**, and **GPIO27** respectively.

```c
#define RED_PIN   GPIO_NUM_25
#define GREEN_PIN GPIO_NUM_26
#define BLUE_PIN  GPIO_NUM_27

void app_main(void) {
    gpio_reset_pin(RED_PIN);
    gpio_reset_pin(GREEN_PIN);
    gpio_reset_pin(BLUE_PIN);
    gpio_set_direction(RED_PIN,   GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_PIN,  GPIO_MODE_OUTPUT);

    while (1) {
        set_color(1, 0, 0); // RED
        set_color(0, 1, 0); // GREEN
        set_color(0, 0, 1); // BLUE
        set_color(0, 0, 0); // OFF
    }
}
```

> Files: [`rgb.ino`](./RGB-LED-Control/rgb.ino)

---

### 7. servo-motor — GPIO18 (LEDC PWM)

Controls a standard servo motor on **GPIO18** using the ESP-IDF **LEDC**
timer/channel API. The servo sweeps from 0° → 90° → 180° with a 2-second
pause at each position.

```c
#define SERVO_PIN GPIO_NUM_18

void app_main(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_14_BIT,
        .freq_hz          = 50,
        .clk_cfg          = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num  = SERVO_PIN,
        .speed_mode= LEDC_LOW_SPEED_MODE,
        .channel   = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty      = 0,
        .hpoint    = 0,
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 410);  // 0°
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1230); // 90°
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 2050); // 180°
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```

> Files: [`wokwi/code.ino`](./servo-motor/wokwi/code.ino)

---

### 8. LDR — Light Dependent Resistor

A placeholder project for a light-sensing circuit using an LDR. The code has
not been implemented yet.

> Files: [`README.md`](./LDR/README.md)

---

## Contributing

Feel free to open an issue or submit a pull request to add new projects, fix
bugs, or improve documentation.

## License

This repository is licensed under the MIT License.

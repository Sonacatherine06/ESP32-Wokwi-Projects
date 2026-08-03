# Push Button + LED

## Overview

A simple ESP32 project that reads the state of a momentary push button and drives an
LED accordingly. The button uses the ESP32's internal pull-up resistor, so pressing the
button pulls the input to ground (logic `0`), turning the LED on. The current state is
reported over the serial console via `printf`.

## Hardware

| Component   | ESP32 Pin | Notes                                    |
|-------------|-----------|------------------------------------------|
| LED         | GPIO 2    | Output. Connects to a current-limiting resistor (220–330 Ω) and then to GND. |
| Push Button | GPIO 4    | Input with `GPIO_PULLUP_ONLY`. One side to GPIO 4, the other to GND. |

### Wiring

```
                 +3.3V
                   |
                   |              Button
GPIO 4 ----+-------+-------[ ]-------
           |                 |
           |            (pull-up = internal)
           |                 |
          GND --------------+

GPIO 2 ----[220–330 Ω]----|>|---- GND   (LED + current-limiting resistor)
```

> The ESP32's internal pull-up (`GPIO_PULLUP_ONLY`) holds GPIO 4 at `1` when the button
> is unpressed. Pressing the button connects the pin to GND, pulling it to `0`.

## How It Works

1. **LED (GPIO 2)** is configured as a digital output.
2. **Button (GPIO 4)** is configured as a digital input with an internal pull-up resistor.
3. The `app_main()` task polls the button level every 100 ms.
4. **Button pressed (level 0):** the LED is turned **ON** (`gpio_set_level(LED_PIN, 1)`).
5. **Button released (level 1):** the LED is turned **OFF** (`gpio_set_level(LED_PIN, 0)`).
6. Each transition is logged to the serial console:
   - `Button Pressed - LED ON`
   - `Button Released - LED OFF`

```cpp
#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4

gpio_reset_pin(LED_PIN);
gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

gpio_reset_pin(BUTTON_PIN);
gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

while (1) {
    int buttonState = gpio_get_level(BUTTON_PIN);
    if (buttonState == 0) {
        gpio_set_level(LED_PIN, 1);
        printf("Button Pressed - LED ON\n");
    } else {
        gpio_set_level(LED_PIN, 0);
        printf("Button Released - LED OFF\n");
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
```

## Serial Monitor Output

Configure your serial monitor to **115200 baud** (default for ESP-IDF). You should see:

```
Button Pressed - LED ON
Button Released - LED OFF
```

These messages repeat as the button is pressed and released.

## Files

| File                      | Description                              |
|---------------------------|------------------------------------------|
| `led+pushbutton.ino`      | ESP-IDF source code for the project.      |
| `README.md`               | This documentation.                       |
| `Push Button + LED.mp4`   | Demo video of the project in Wokwi.       |

## Running in Wokwi

1. Open the [Wokwi ESP32 Simulator](https://wokwi.app/).
2. Create a new ESP32 project and add the components above to the diagram:

   ```json
   {
     "components": [
       { "type": "esp32-devkit", "id": "esp32" },
       { "type": "button", "id": "btn1", "top": 50, "left": 150, "attrs": { "bounce": "0" } },
       { "type": "led", "id": "led1", "top": 100, "left": 150, "attrs": { "color": "green" } },
       { "type": "resistor", "id": "r1", "top": 100, "left": 200, "attrs": { "value": "220" } }
     ]
   }
   ```

3. Wire the components:

   ```
   esp32.pinGPIO2  -> led1.anode   /  led1.cathode -> r1 -> esp32.GND
   esp32.pinGPIO4  -> btn1.1       /  btn1.2      -> esp32.GND
   ```

4. Upload `led+pushbutton.ino` as the sketch and click **Start** (🔘).
5. Open the **Serial Monitor** to see the button/LED log messages.

## Demo

A demonstration of the circuit running in Wokwi is included in
`Push Button + LED.mp4`.

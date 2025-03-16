# Example for `bh1750` Driver

## What it Does

This example demonstrates how to configure and interact with a BH1750 ambient light sensor using the ESP-IDF framework. The BH1750 provides lux (light intensity) readings, which are read and displayed every 2 seconds.

The I2C bus and BH1750 sensor parameters can be configured via **menuconfig**.

## Configuration

To configure the example:

1. Run `idf.py menuconfig`.
2. Navigate to **Example Configuration**:
   - Set the I2C bus number.
   - Set the GPIO numbers for SDA and SCL.
   - Choose the I2C address (`0x23` or `0x5C`).

### Default Configuration

| Setting               | Description                              | Default  |
|-----------------------|------------------------------------------|----------|
| **I2C Bus**          | I2C port number                          | `0`      |
| **SDA GPIO**         | GPIO number for SDA line                 | `21`     |
| **SCL GPIO**         | GPIO number for SCL line                 | `22`     |
| **Sensor Address**   | I2C address (`0x23` or `0x5C`)           | `0x23`   |

## Wiring

Connect the BH1750 sensor as follows:

| BH1750 Pin | ESP32 Connection         |
|------------|--------------------------|
| **VCC**    | 3.3V                     |
| **GND**    | Ground                   |
| **SDA**    | Configured SDA GPIO (e.g., `21`) |
| **SCL**    | Configured SCL GPIO (e.g., `22`) |

## Example Code

Here’s how to read data from the BH1750 sensor in your project:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bh1750.h"

static const char *TAG = "example_main";

void app_main(void)
{
    bh1750_dev_t sensor;
    ESP_ERROR_CHECK(bh1750_init_desc(&sensor, BH1750_ADDR_LO, I2C_NUM_0));
    ESP_ERROR_CHECK(bh1750_power_on(&sensor));
    ESP_ERROR_CHECK(bh1750_setup(&sensor, BH1750_MODE_CONTINUOUS, BH1750_RES_HIGH));

    while (1)
    {
        uint16_t lux = 0;
        esp_err_t result = bh1750_read(&sensor, &lux);
        if (result == ESP_OK)
        {
            ESP_LOGI(TAG, "Light Intensity: %d lux", lux);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read sensor data: %s", esp_err_to_name(result));
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}
```

## Building and Running

Build the Project:
```sh
idf.py build
```
Flash the ESP32:
```sh
idf.py flash
```
Monitor Logs:
```sh
idf.py monitor
```

## Example Output

```
I (12345) example_main: Light Intensity: 430 lux
I (14345) example_main: Light Intensity: 425 lux
```

## Notes

- Ensure the BH1750 sensor is connected properly, and the I2C pins match the configuration in menuconfig.
- The I2C address depends on the sensor's ADDR pin.
- The sensor must be powered on before taking readings.

StudioPieters® | Innovation and Learning Labs | https://www.studiopieters.nl

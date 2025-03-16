/**
   Copyright 2025 Achim Pieters | StudioPieters®

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   for more information visit https://www.studiopieters.nl
 **/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "bh1750.h"

#define SDA_GPIO GPIO_NUM_21   // Update based on your board
#define SCL_GPIO GPIO_NUM_22   // Update based on your board
#define I2C_PORT I2C_NUM_0
#define I2C_FREQ_HZ 400000

// Use Kconfig options for the I2C address
#ifdef CONFIG_EXAMPLE_I2C_ADDRESS_LO
    #define BH1750_I2C_ADDR BH1750_ADDR_LO
#elif CONFIG_EXAMPLE_I2C_ADDRESS_HI
    #define BH1750_I2C_ADDR BH1750_ADDR_HI
#else
    #define BH1750_I2C_ADDR BH1750_ADDR_LO  // Default
#endif

static const char *TAG = "BH1750_MAIN";

void app_main(void)
{
    i2c_dev_t sensor;

    ESP_LOGI(TAG, "Initializing BH1750 sensor...");

    // Configure the I2C bus first
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    // Initialize BH1750 descriptor using Kconfig address
    esp_err_t err = bh1750_init_desc(&sensor, BH1750_I2C_ADDR, I2C_PORT, SDA_GPIO, SCL_GPIO);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize sensor descriptor: %s", esp_err_to_name(err));
        return;
    }

    err = bh1750_power_on(&sensor);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to power on sensor: %s", esp_err_to_name(err));
        return;
    }

    err = bh1750_setup(&sensor, BH1750_MODE_CONTINUOUS, BH1750_RES_HIGH);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to setup sensor: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Sensor initialized successfully at address 0x%02X.", BH1750_I2C_ADDR);

    while (1)
    {
        uint16_t lux = 0;
        err = bh1750_read(&sensor, &lux);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "Light Intensity: %d lux", lux);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read sensor data: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

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
#include <esp_log.h>
#include <driver/i2c.h>
#include "bh1750.h"

static const char *TAG = "BH1750";

#define I2C_FREQ_HZ 400000

#define OPCODE_POWER_DOWN 0x00
#define OPCODE_POWER_ON   0x01
#define OPCODE_CONTINUOUS_HIGH 0x10
#define OPCODE_ONE_TIME_HIGH   0x20

static esp_err_t send_command(i2c_dev_t *dev, uint8_t cmd)
{
    return i2c_master_write_to_device(dev->port, dev->addr, &cmd, 1, pdMS_TO_TICKS(100));
}

esp_err_t bh1750_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    if (!dev) return ESP_ERR_INVALID_ARG;

    dev->port = port;
    dev->addr = addr;

    ESP_LOGI(TAG, "BH1750 initialized at I2C addr 0x%02X on port %d", addr, port);
    return ESP_OK;
}

esp_err_t bh1750_free_desc(i2c_dev_t *dev)
{
    if (!dev) return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "BH1750 descriptor freed.");
    return ESP_OK;
}

esp_err_t bh1750_power_down(i2c_dev_t *dev)
{
    return send_command(dev, OPCODE_POWER_DOWN);
}

esp_err_t bh1750_power_on(i2c_dev_t *dev)
{
    return send_command(dev, OPCODE_POWER_ON);
}

esp_err_t bh1750_setup(i2c_dev_t *dev, bh1750_mode_t mode, bh1750_resolution_t resolution)
{
    uint8_t opcode = (mode == BH1750_MODE_CONTINUOUS) ? OPCODE_CONTINUOUS_HIGH : OPCODE_ONE_TIME_HIGH;
    return send_command(dev, opcode);
}

esp_err_t bh1750_read(i2c_dev_t *dev, uint16_t *level)
{
    if (!dev || !level) return ESP_ERR_INVALID_ARG;

    uint8_t buf[2] = {0};

    esp_err_t err = i2c_master_read_from_device(dev->port, dev->addr, buf, 2, pdMS_TO_TICKS(100));
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read from sensor: %s", esp_err_to_name(err));
        return err;
    }

    *level = (buf[0] << 8) | buf[1];
    *level = (*level * 10) / 12; // Convert to LUX
    return ESP_OK;
}

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
 
 #ifndef __BH1750_H__
 #define __BH1750_H__

 #include <stdint.h>
 #include <driver/i2c.h>
 #include <esp_err.h>

 #ifdef __cplusplus
 extern "C" {
 #endif

 #define BH1750_ADDR_LO 0x23 //!< I2C address when ADDR pin floating/low
 #define BH1750_ADDR_HI 0x5C //!< I2C address when ADDR pin high

 typedef enum
 {
     BH1750_MODE_ONE_TIME = 0, //!< One time measurement
     BH1750_MODE_CONTINUOUS    //!< Continuous measurement
 } bh1750_mode_t;

 typedef enum
 {
     BH1750_RES_LOW = 0,  //!< 4 lx resolution, measurement time is usually 16 ms
     BH1750_RES_HIGH,     //!< 1 lx resolution, measurement time is usually 120 ms
     BH1750_RES_HIGH2     //!< 0.5 lx resolution, measurement time is usually 120 ms
 } bh1750_resolution_t;

 typedef struct {
     i2c_port_t port;
     uint8_t addr;
     i2c_config_t i2c_conf;
 } i2c_dev_t;

 esp_err_t bh1750_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);
 esp_err_t bh1750_free_desc(i2c_dev_t *dev);
 esp_err_t bh1750_power_down(i2c_dev_t *dev);
 esp_err_t bh1750_power_on(i2c_dev_t *dev);
 esp_err_t bh1750_setup(i2c_dev_t *dev, bh1750_mode_t mode, bh1750_resolution_t resolution);
 esp_err_t bh1750_read(i2c_dev_t *dev, uint16_t *level);

 #ifdef __cplusplus
 }
 #endif

 #endif /* __BH1750_H__ */

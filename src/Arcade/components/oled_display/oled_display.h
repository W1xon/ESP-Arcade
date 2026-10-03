#pragma once

#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_ssd1306.h"
#include "driver/i2c_master.h"


#define OLED_SDA_GPIO GPIO_NUM_22
#define OLED_SCL_GPIO GPIO_NUM_23

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_I2C_ADDRESS 0x3C

void InitOled(void);
void DrawBitmapToOled(const uint8_t *bitmap);
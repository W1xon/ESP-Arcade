#pragma once

#include "driver/i2c_master.h"


#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void InitOled(void);
void DrawBitmapToOled(const uint8_t *bitmap);
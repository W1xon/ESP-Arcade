#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "esp_random.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_HEIGHT / 8)

void GameStep();
void RandomizeBuffer();
void ClearGameBuffer();
const uint8_t* GetCurrentFrame();

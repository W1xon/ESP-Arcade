#pragma once

#define WIDTH 128
#define HEIGHT 64

#define BUFFER_SIZE (WIDTH * HEIGHT / 8)
#include <stdint.h>

void Update();
const uint8_t* GetCurrentFrame();

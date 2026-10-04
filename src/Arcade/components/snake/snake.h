#pragma once
#include <stdint.h>

#define WIDTH 128
#define HEIGHT 64

#define BUFFER_SIZE (WIDTH * HEIGHT / 8)

void InitSnake();
void Update();
const uint8_t* SnakeGetCurrentFrame();

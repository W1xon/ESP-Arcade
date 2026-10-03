#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

void ArcadeInit();

 void IRAM_ATTR ArcadeButtonISRHandler(void *arg);
QueueHandle_t GetArcadeFrameQueue();
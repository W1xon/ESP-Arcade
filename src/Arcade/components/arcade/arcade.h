#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "joystick.h"


void ArcadeInit();

 void IRAM_ATTR ArcadeButtonISRHandler(void *arg);
QueueHandle_t GetArcadeFrameQueue();
joystickPosition_t GetJoystickPosition();
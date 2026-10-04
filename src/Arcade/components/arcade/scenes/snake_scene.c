#include "snake_scene.h"
#include <stdint.h>
#include "snake.h"
#include "arcade.h"

void StartSnakeGame() {
    InitSnake();
}
void UpdateSnakeGame() {
    Update();
    const uint8_t *frame = SnakeGetCurrentFrame();
    QueueHandle_t frameQueue = GetArcadeFrameQueue();
    xQueueSend(frameQueue, &frame, portMAX_DELAY);
}

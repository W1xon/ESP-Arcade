#include "gol_scene.h"
#include "game_of_life.h"
#include "arcade.h"

void StartGameOfLife() {
    RandomizeBuffer();
}
void UpdateGameOfLife() {
    GameStep();

    const uint8_t *frame = GolGetCurrentFrame();
    QueueHandle_t frameQueue = GetArcadeFrameQueue();
    xQueueSend(frameQueue, &frame, portMAX_DELAY);
}
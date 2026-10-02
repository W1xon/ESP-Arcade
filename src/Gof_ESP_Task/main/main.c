#include "oled_display.h"
#include "game_of_life.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static QueueHandle_t frameQueue = NULL;

void GameLogiclTask(void *args) {
    RandomizeBuffer();
    while (1) {
        GameStep();

        const uint8_t *frame = GetCurrentFrame();

        xQueueSend(frameQueue, &frame, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void DisplayTask(void *args) {
    InitOled();
    const uint8_t *drawFrame = NULL;
    while (1) {
        if (xQueueReceive(frameQueue, &drawFrame, portMAX_DELAY) == pdTRUE) {
            if (drawFrame != NULL) {
                DrawBitmapToOled(drawFrame);
            }
        }
    }
}
void app_main(void) {

    frameQueue = xQueueCreate(2, sizeof(uint8_t *));
    if (frameQueue != NULL) {

        xTaskCreate(GameLogiclTask, "GameLogiclTask", 2048, NULL, 2, NULL);

        xTaskCreate(DisplayTask, "DisplayTask", 2048, NULL, 1, NULL);
    }

    vTaskDelete(NULL);
}
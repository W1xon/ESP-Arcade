#include "oled_display.h"
#include "game_of_life.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    InitOled();
    RandomizeBuffer();
    DrawBitmapToOled(GetCurrentFrame());
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1) {
        GameStep();
        DrawBitmapToOled(GetCurrentFrame());
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
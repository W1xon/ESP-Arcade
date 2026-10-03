#include "oled_display.h"
#include "game_of_life.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/gpio_filter.h"

#define BUTTON_GPIO GPIO_NUM_16


static QueueHandle_t frameQueue = NULL;
static TaskHandle_t xGameLogicTaskHandle = NULL;

void GameLogicTask(void *args) {
    RandomizeBuffer();
    while (1) {

        if (ulTaskNotifyTake(pdTRUE, 0) > 0) {

            ClearGameBuffer();
            RandomizeBuffer();
        }
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
static void IRAM_ATTR button_isr_handler(void *arg) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xGameLogicTaskHandle, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}
void app_main(void) {

    gpio_config_t buttonConfig = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    gpio_config(&buttonConfig);

    gpio_pin_glitch_filter_config_t filterConfig = {
        .clk_src = GLITCH_FILTER_CLK_SRC_DEFAULT,
        .gpio_num = BUTTON_GPIO,
    };
    gpio_glitch_filter_handle_t filterHandle = NULL;
    gpio_new_pin_glitch_filter(&filterConfig, &filterHandle);
    gpio_glitch_filter_enable(filterHandle);

    frameQueue = xQueueCreate(2, sizeof(uint8_t *));
    if (frameQueue != NULL) {

        xTaskCreate(GameLogicTask, "GameLogicTask", 2048, NULL, 2, &xGameLogicTaskHandle);


        xTaskCreate(DisplayTask, "DisplayTask", 2048, NULL, 1, NULL);
    }

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, (void *)BUTTON_GPIO);

    vTaskDelete(NULL);
}
#include "arcade.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/gpio_filter.h"
#include "oled_display.h"
#include "game_of_life.h"

#include "scenes/menu_scene.h"
#include "scenes/gol_scene.h"
#include "scenes/snake_scene.h"
#include "joystick.h"

typedef  void(*StateFunction)(void);
typedef enum {
    STATE_MAIN_MENU,
    STATE_GAME_OF_LIFE,
    STATE_SNAKE,
    STATE_MAX
} AppState;

static AppState currentState = STATE_MAIN_MENU;


static StateFunction startStateFunctions[] = {
    [STATE_MAIN_MENU] = StartMainMenu,
    [STATE_GAME_OF_LIFE] = StartGameOfLife,
    [STATE_SNAKE] = StartSnakeGame,
};

static StateFunction updateStateFunctions[] = {
    [STATE_MAIN_MENU] = UpdateMainMenu,
    [STATE_GAME_OF_LIFE] = UpdateGameOfLife,
    [STATE_SNAKE] = UpdateSnakeGame,
};

static QueueHandle_t frameQueue = NULL;
static TaskHandle_t xGameLogicTaskHandle = NULL;

static QueueHandle_t joystickQueue = NULL;

QueueHandle_t GetArcadeFrameQueue() {
    return frameQueue;
}

void ArcadeButtonISRHandler(void *arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xGameLogicTaskHandle, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void ArcadeLogicTask(void *args) {
    bool sceneChanged = false;
    startStateFunctions[currentState]();
    while(1) {

        if (ulTaskNotifyTake(pdTRUE, 0) > 0) {
            printf("Button pressed, changing scene\n");
            currentState = (AppState)((currentState + 1) % STATE_MAX);
            sceneChanged = true;
        }
        if (sceneChanged) {
            startStateFunctions[currentState]();
        }
        else {
            updateStateFunctions[currentState]();
        }

        sceneChanged = false;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void ArcadeDisplayTask(void *args) {
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

void JoystickHandleTask(void *args) {

    JoystickInit();
    joystickPosition_t joystickPos;
    while (1) {

        joystickPos = JoystickRead();

        xQueueOverwrite(joystickQueue, &joystickPos);
        printf("Joystick: x=%d, y=%d\n", joystickPos.x, joystickPos.y);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
void ArcadeInit() {
    frameQueue = xQueueCreate(2, sizeof(uint8_t *));
    joystickQueue = xQueueCreate(1, sizeof(joystickPosition_t));

    xTaskCreate(ArcadeLogicTask, "ArcadeLogicTask", 2048, NULL, 2, &xGameLogicTaskHandle);
    xTaskCreate(ArcadeDisplayTask, "ArcadeDisplayTask", 2048, NULL, 1, NULL);
    xTaskCreate(JoystickHandleTask, "JoystickHandleTask", 2048, NULL, 4, NULL);
}

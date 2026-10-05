#include "menu_scene.h"

#include <string.h>

#include "arcade.h"

#define MENU_WIDTH 128
#define MENU_HEIGHT 64
#define BUFFER_SIZE (MENU_WIDTH * MENU_HEIGHT / 8)

static uint8_t menuBuffer[BUFFER_SIZE];

static void ClearBuffer(void)
{
    memset(menuBuffer, 0, BUFFER_SIZE);
}

static void DrawFilledRectangle(int x, int y, int width, int height)
{
    for (int currentY = y; currentY < y + height; currentY++) {
        for (int currentX = x; currentX < x + width; currentX++) {
            if (currentX < 0 || currentX >= MENU_WIDTH ||
                currentY < 0 || currentY >= MENU_HEIGHT) {
                continue;
                }

            int byteIndex = currentX + (currentY / 8) * MENU_WIDTH;
            uint8_t bitMask = 1 << (currentY % 8);

            menuBuffer[byteIndex] |= bitMask;
        }
    }
}

void StartMainMenu(void)
{
    ClearBuffer();
    DrawFilledRectangle(48, 16, 32, 32);

    const uint8_t *frame = menuBuffer;

    QueueHandle_t frameQueue = GetArcadeFrameQueue();
    xQueueSend(frameQueue, &frame, pdMS_TO_TICKS(100));
}

void UpdateMainMenu(void)
{
}
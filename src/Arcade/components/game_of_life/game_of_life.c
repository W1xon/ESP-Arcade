#include "game_of_life.h"

static uint8_t gameBuffer[BUFFER_SIZE];
static uint8_t nextGameBuffer[BUFFER_SIZE];

static bool IsInBounds(int x, int y) {
    return (x >= 0 && x < OLED_WIDTH && y >= 0 && y < OLED_HEIGHT);
}


static bool IsCellAlive(int x, int y) {
    if (!IsInBounds(x, y)) {
        return false;
    }
    int byteIndex = x + (y / 8) * OLED_WIDTH;
    uint8_t bitMask = 1 << (y % 8);
    return (gameBuffer[byteIndex] & bitMask) != 0;
}

static int GetLifeNeighbours(int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            int newX = x + dx;
            int newY = y + dy;
            if (!IsInBounds(newX, newY)) {
                continue;
            }
            if (IsCellAlive(newX, newY)) {
                count++;
            }
        }
    }
    return count;
}


static void ChangeCellState(uint8_t *buffer, int x, int y, bool state) {
    if (!IsInBounds(x, y)) {
        return;
    }
    int byteIndex = x + (y / 8) * OLED_WIDTH;
    uint8_t bitMask = 1 << (y % 8);
    if (state) {
        buffer[byteIndex] |= bitMask;
    }
    else {
        buffer[byteIndex] &= ~bitMask;
    }
}

static void ClearBuffer(uint8_t *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
}
static bool IsCellAliveNextGen(int x, int y) {
    int lifeNeighbours = GetLifeNeighbours(x, y);
    if (IsCellAlive(x, y)) {
        return (lifeNeighbours == 2 || lifeNeighbours == 3);
    }
    return (lifeNeighbours == 3);
}

void GameStep() {
    ClearBuffer(nextGameBuffer);
    for (int y = 0; y < OLED_HEIGHT; y++) {
        for (int x = 0; x < OLED_WIDTH; x++) {
            bool nextState = IsCellAliveNextGen(x, y);
            ChangeCellState(nextGameBuffer, x, y, nextState);
        }
    }
    memcpy(gameBuffer, nextGameBuffer, BUFFER_SIZE);
}

void RandomizeBuffer() {
    ClearGameBuffer();
    uint32_t *ptr = (uint32_t*)gameBuffer;
    for (int i = 0; i < BUFFER_SIZE / 4; i++) {
        ptr[i] = esp_random();
    }
}
void ClearGameBuffer() {
    memset(gameBuffer, 0, BUFFER_SIZE);
}

const uint8_t* GetCurrentFrame() {
    return gameBuffer;
}

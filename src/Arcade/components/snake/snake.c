#include "arcade.h"
#include  "snake.h"

#include <string.h>

#include "joystick.h"
#include "esp_random.h"

#define JOY_CENTER 1500
#define JOY_THRESHOLD 800
#define MAX_SNAKE_LENGTH 50

typedef struct {
    int8_t x;
    int8_t y;
} Point_t;

static Point_t currentDirection = {.x = 1, .y = 0};
static Point_t foodPosition = {.x = 0, .y = 0};
static Point_t snake[MAX_SNAKE_LENGTH];
static uint8_t snakeLength = 5;
static uint8_t gameBuffer[BUFFER_SIZE];

static void SetPixel(int x, int y, int8_t value) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }
    int byteIndex = x + (y / 8) * WIDTH;
    uint8_t bitMask = 1 << (y % 8);
    if (value) {
        gameBuffer[byteIndex] |= bitMask;
    } else {
        gameBuffer[byteIndex] &= ~bitMask;
    }
}
static void CreateFood() {
    int foodX = esp_random() % WIDTH;
    int foodY = esp_random() % HEIGHT;
    foodPosition.x = foodX;
    foodPosition.y = foodY;
    SetPixel(foodX, foodY, 2);
}


static void UpdateDirection() {
    joystickPosition_t joystickPos = GetJoystickPosition();

    if (joystickPos.x < JOY_CENTER - JOY_THRESHOLD) {
        if (currentDirection.x != 1) currentDirection = (Point_t){.x = 1, .y = 0};
    }
    else if (joystickPos.x > JOY_CENTER + JOY_THRESHOLD) {
        if (currentDirection.x != -1) currentDirection = (Point_t){.x = -1, .y = 0};
    }
    else if (joystickPos.y < JOY_CENTER - JOY_THRESHOLD) {
        if (currentDirection.y != 1) currentDirection = (Point_t){.x = 0, .y = -1};
    }
    else if (joystickPos.y > JOY_CENTER + JOY_THRESHOLD) {
        if (currentDirection.y != -1) currentDirection = (Point_t){.x = 0, .y = 1};
    }
}


static void Move() {
    Point_t oldTail = snake[snakeLength - 1];

    for (uint8_t i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    snake[0].x = (snake[0].x + currentDirection.x + WIDTH) % WIDTH;
    snake[0].y = (snake[0].y + currentDirection.y + HEIGHT) % HEIGHT;

    if (snake[0].x == foodPosition.x && snake[0].y == foodPosition.y) {
        if (snakeLength < MAX_SNAKE_LENGTH) {
            snake[snakeLength] = oldTail;
            snakeLength++;
        }
        CreateFood();
    } else {
        SetPixel(oldTail.x, oldTail.y, 0);
    }

    SetPixel(snake[0].x, snake[0].y, 1);
}


void SnakeClearGameBuffer() {
    memset(gameBuffer, 0, BUFFER_SIZE);
}
void InitSnake() {
    SnakeClearGameBuffer();
    for (uint8_t i = 0; i < snakeLength; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }
    CreateFood();
}

void Update() {
    UpdateDirection();
    Move();

    if (snake[0].x == foodPosition.x && snake[0].y == foodPosition.y) {
        if (snakeLength < MAX_SNAKE_LENGTH) {
            snakeLength++;
            snake[snakeLength - 1] = snake[snakeLength - 2];
        }
        CreateFood();
    }
}


const uint8_t* SnakeGetCurrentFrame() {
    return gameBuffer;
}
#include "arcade.h"
#include "driver/gpio.h"
#include "driver/gpio_filter.h"

#define BUTTON_GPIO GPIO_NUM_17

static void ConfigureButtonGPIO() {
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
}


void app_main(void) {

    ArcadeInit();

    ConfigureButtonGPIO();
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, ArcadeButtonISRHandler, (void *)BUTTON_GPIO);

    vTaskDelete(NULL);
}

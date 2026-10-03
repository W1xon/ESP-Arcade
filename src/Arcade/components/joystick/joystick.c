#include "joystick.h"
#include "esp_adc/adc_oneshot.h"

// Настройки АЦП под XIAO ESP32C6 (D0 и D1)
#define JOYSTICK_ADC_UNIT        ADC_UNIT_1
#define JOYSTICK_ADC_CH_X        ADC_CHANNEL_0 // Пин D0
#define JOYSTICK_ADC_CH_Y        ADC_CHANNEL_1 // Пин D1

static adc_oneshot_unit_handle_t adc_handle = NULL;

void JoystickInit(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = JOYSTICK_ADC_UNIT,
        .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
    };

    if (adc_oneshot_new_unit(&init_config, &adc_handle) != ESP_OK) {
        return;
    }

    adc_oneshot_chan_cfg_t adc_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    adc_oneshot_config_channel(adc_handle, JOYSTICK_ADC_CH_X, &adc_config);
    adc_oneshot_config_channel(adc_handle, JOYSTICK_ADC_CH_Y, &adc_config);
}

joystickPosition_t JoystickRead(void)
{
    joystickPosition_t data = { .x = 2048, .y = 2048 }; // Дефолт в центр

    if (adc_handle == NULL) {
        return data;
    }

    int raw_x = 0;
    int raw_y = 0;

    if (adc_oneshot_read(adc_handle, JOYSTICK_ADC_CH_X, &raw_x) == ESP_OK) {
        data.x = (int16_t)raw_x;
    }

    if (adc_oneshot_read(adc_handle, JOYSTICK_ADC_CH_Y, &raw_y) == ESP_OK) {
        data.y = (int16_t)raw_y;
    }

    return data;
}

#include  "oled_display.h"

static esp_lcd_panel_handle_t panelOled;

void InitOled(void) {

    // Создаем конфигурацию шины
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = -1,
        .sda_io_num = OLED_SDA_GPIO,
        .scl_io_num = OLED_SCL_GPIO,
        .flags.enable_internal_pullup = true

    };

    // Создаем объект шины I2C
    i2c_master_bus_handle_t i2cBus;
    i2c_new_master_bus(&bus_config, &i2cBus);

    //Создаем конфиг IO
    esp_lcd_panel_io_i2c_config_t ioConfig = {
        .dev_addr = 0x3C,
        .scl_speed_hz = 400000,
        .control_phase_bytes =  1,
        .dc_bit_offset = 6,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };

    // Создаем объект IO
    esp_lcd_panel_io_handle_t ioOled;
    esp_lcd_new_panel_io_i2c(i2cBus, &ioConfig, &ioOled);

    // Создаем конфиг ssd1306
    esp_lcd_panel_ssd1306_config_t ssd1306Config = {
        .height = 64,
    };

    // Настрйоки панели
    esp_lcd_panel_dev_config_t panelConfig = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
        .vendor_config = &ssd1306Config,
    };

    // Создаем объект панели
    esp_lcd_new_panel_ssd1306(ioOled, &panelConfig, &panelOled);

    // Запуск
    esp_lcd_panel_reset(panelOled);
    esp_lcd_panel_init(panelOled);
    esp_lcd_panel_disp_on_off(panelOled, true);
}

void DrawBitmapToOled(const uint8_t *bitmap) {
    esp_lcd_panel_draw_bitmap(panelOled, 0, 0, OLED_WIDTH, OLED_HEIGHT, bitmap);
}

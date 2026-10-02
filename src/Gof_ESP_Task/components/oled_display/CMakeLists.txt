idf_component_register(
        SRCS "oled_display.c"
        INCLUDE_DIRS "."
        REQUIRES esp_lcd esp_driver_i2c driver
)
idf_component_register(SRCS "arcade.c"
        "scenes/menu_scene.c"
        "scenes/gol_scene.c"
        "scenes/snake_scene.c"
        INCLUDE_DIRS "." "scenes"
        REQUIRES oled_display game_of_life joystick driver log snake)

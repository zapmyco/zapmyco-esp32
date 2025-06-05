set(requires
    driver
    esp_driver_i2s
    esp_timer
    )

idf_component_register(SRCS
                       main.cc
                       bsp_board.cc
                       commands/command_manager.cc
                       commands/light_on_command.cc
                       commands/light_off_command.cc
                       commands/bye_bye_command.cc
                       INCLUDE_DIRS
                       "."
                       )


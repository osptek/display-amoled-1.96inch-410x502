file(GLOB_RECURSE SOURCES
    ./*.c
    ./app_init/*.c
)

idf_component_register(
    SRCS
        ${SOURCES}
    INCLUDE_DIRS
        "."
        "./app_init"
    REQUIRES
        driver
        esp_lcd
        esp_timer
        esp_lcd_qspi_amoled
        esp_lcd_touch
        esp_lcd_touch_cst820
)

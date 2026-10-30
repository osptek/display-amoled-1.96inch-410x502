#pragma once

#include "esp_err.h"
#include "lvgl.h"

esp_err_t app_lcd_init(void);
esp_err_t app_touch_init(void);
esp_err_t app_lvgl_init(void);

extern lv_display_t *disp;

bool app_lvgl_lock(int timeout_ms);
void app_lvgl_unlock(void);

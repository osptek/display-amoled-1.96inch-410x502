#include "esp_log.h"
#include "app_init.h"

static const char *TAG = "app_init";

esp_err_t app_init(void)
{
    ESP_ERROR_CHECK(app_lcd_init());

    esp_err_t touch_ret = app_touch_init();
    if (touch_ret != ESP_OK) {
        ESP_LOGW(TAG, "Touch unavailable (%s), continue with display only", esp_err_to_name(touch_ret));
    }

    ESP_ERROR_CHECK(app_lvgl_init());
    return ESP_OK;
}

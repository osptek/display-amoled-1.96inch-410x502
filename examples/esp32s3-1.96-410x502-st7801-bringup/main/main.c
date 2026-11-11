#include "esp_log.h"
#include "lv_demos.h"
#include "app_init.h"

static const char *TAG = "app_main";

void app_main(void)
{
    ESP_ERROR_CHECK(app_init());

    if (app_lvgl_lock(-1)) {
        ESP_LOGI(TAG, "Start LVGL widgets demo");
        lv_demo_widgets();
        app_lvgl_unlock();
    }
}

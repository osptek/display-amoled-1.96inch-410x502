#include <assert.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_qspi_amoled.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_cst820.h"
#include "driver/spi_master.h"
#include "driver/i2c_master.h"
#include "esp_lv_adapter.h"
#include "lvgl.h"

#include "config.h"
#include "lcd_init.h"

static const char *TAG = "LCD Init";

#define LCD_HOST                SPI2_HOST
#define TOUCH_I2C_NUM           I2C_NUM_0
#define TOUCH_I2C_CLK_HZ        (400 * 1000)
#define LCD_PIXEL_CLOCK_HZ      (80 * 1000 * 1000)

#define LCD_H_RES               410
#define LCD_V_RES               502
#define LCD_X_GAP               0
#define LCD_Y_GAP               0

lv_display_t *disp = NULL;

static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t panel_io = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/*
 * Truly 1.96" 410x502 AMOLED, ST7801N controller (QSPI).
 * Init sequence per vendor "R196GRK-0xx-LCM-InitialCode" (docs/).
 * MADCTL(36h) and COLMOD(3Ah) are issued by the driver itself; CASET(2Ah)/
 * RASET(2Bh) are issued per-frame by the driver, so they are omitted here.
 */
static const qspi_amoled_lcd_init_cmd_t lcd_init_cmds[] = {
    {0x11, (uint8_t []){0x00}, 0, 120},   // Sleep out
    {0x35, (uint8_t []){0x00}, 1, 0},     // TE line on
    {0x3A, (uint8_t []){0x55}, 1, 0},     // Interface pixel format: 16bit/pixel
    {0x53, (uint8_t []){0x20}, 1, 0},     // Write CTRL display: brightness ctrl on
    {0x51, (uint8_t []){0xFF}, 1, 0},     // Display brightness: max
    {0x29, (uint8_t []){0x00}, 0, 60},    // Display on
};

#if !BOARD_LCD_USE_TE
static void st7801_area_rounder_cb(lv_area_t *area, void *user_data)
{
    (void)user_data;
    area->x1 = (area->x1 >> 1) << 1;
    area->y1 = (area->y1 >> 1) << 1;
    area->x2 = ((area->x2 >> 1) << 1) + 1;
    area->y2 = ((area->y2 >> 1) << 1) + 1;
}
#endif

static void setup_default_screen_bg(lv_display_t *display)
{
    lv_obj_t *default_scr = lv_display_get_screen_active(display);
    if (default_scr) {
        lv_obj_remove_style_all(default_scr);
        lv_obj_set_style_bg_color(default_scr, lv_color_black(), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(default_scr, LV_OPA_COVER, LV_STATE_DEFAULT);
        lv_obj_set_size(default_scr, LV_PCT(100), LV_PCT(100));
    }
}

esp_err_t app_lcd_init(void)
{
    ESP_LOGI(TAG, "Initialize ST7801 QSPI (%dx%d), BOARD_VERSION=%d, TE=%d",
             LCD_H_RES, LCD_V_RES, BOARD_VERSION, BOARD_LCD_USE_TE);

    const spi_bus_config_t bus_config = QSPI_AMOLED_PANEL_BUS_QSPI_CONFIG(
        PIN_LCD_PCLK_NUM,
        PIN_LCD_DATA0_NUM,
        PIN_LCD_DATA1_NUM,
        PIN_LCD_DATA2_NUM,
        PIN_LCD_DATA3_NUM,
        LCD_H_RES * LCD_V_RES * sizeof(uint16_t));
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = PIN_LCD_CS_NUM,
        .dc_gpio_num = -1,
        .spi_mode = 0,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        /* DMA max is 32KB. Depth 2 keeps one PSRAM->SRAM copy in flight with one
         * SRAM->panel DMA. Direct PSRAM DMA underruns at 80MHz; depth 10 needs
         * ~320KB of internal bounce buffers and fails to allocate. */
        .trans_queue_depth = 2,
        .lcd_cmd_bits = 32,
        .lcd_param_bits = 8,
        .flags = {
            .quad_mode = true,
        },
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &panel_io));

    qspi_amoled_vendor_config_t vendor_config = {
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
        .flags = {
            .use_qspi_interface = 1,
        },
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_LCD_RST_NUM,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
        .vendor_config = &vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_qspi_amoled(panel_io, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, LCD_X_GAP, LCD_Y_GAP));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(panel_qspi_amoled_set_brightness(panel_handle, 255));

    return ESP_OK;
}

esp_err_t app_touch_init(void)
{
    ESP_LOGI(TAG, "Initialize CST820 (SCL=%d SDA=%d)", PIN_TP_SCL_NUM, PIN_TP_SDA_NUM);

    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = TOUCH_I2C_NUM,
        .scl_io_num = PIN_TP_SCL_NUM,
        .sda_io_num = PIN_TP_SDA_NUM,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t i2c_bus = NULL;
    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &i2c_bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_new_master_bus failed: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST820_CONFIG();
    tp_io_config.scl_speed_hz = TOUCH_I2C_CLK_HZ;
    ret = esp_lcd_new_panel_io_i2c(i2c_bus, &tp_io_config, &tp_io_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_panel_io_i2c failed: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = PIN_TP_RST_NUM,
        .int_gpio_num = PIN_TP_INT_NUM,
        .levels = {.reset = 0, .interrupt = 0},
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    ret = esp_lcd_touch_new_i2c_cst820(tp_io_handle, &tp_cfg, &touch_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "CST820 init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "CST820 touch ready");
    return ESP_OK;
}

esp_err_t app_lvgl_init(void)
{
    ESP_LOGI(TAG, "Initialize esp_lvgl_adapter (TE sync=%d)", BOARD_LCD_USE_TE);

    esp_lv_adapter_config_t adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    adapter_cfg.stack_in_psram = true;
    ESP_ERROR_CHECK(esp_lv_adapter_init(&adapter_cfg));

#if BOARD_LCD_USE_TE
    esp_lv_adapter_display_config_t disp_cfg =
        ESP_LV_ADAPTER_DISPLAY_SPI_WITH_PSRAM_TE_DEFAULT_CONFIG(
            panel_handle,
            panel_io,
            LCD_H_RES,
            LCD_V_RES,
            ESP_LV_ADAPTER_ROTATE_0,
            PIN_LCD_TE_NUM,
            LCD_PIXEL_CLOCK_HZ,
            4,
            16);
#else
    esp_lv_adapter_display_config_t disp_cfg =
        ESP_LV_ADAPTER_DISPLAY_SPI_WITH_PSRAM_DEFAULT_CONFIG(
            panel_handle,
            panel_io,
            LCD_H_RES,
            LCD_V_RES,
            ESP_LV_ADAPTER_ROTATE_0);
#endif

    disp = esp_lv_adapter_register_display(&disp_cfg);
    assert(disp != NULL);
    setup_default_screen_bg(disp);

#if !BOARD_LCD_USE_TE
    ESP_ERROR_CHECK(esp_lv_adapter_set_area_rounder_cb(disp, st7801_area_rounder_cb, NULL));
#endif

    if (touch_handle) {
        esp_lv_adapter_touch_config_t touch_cfg = ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(disp, touch_handle);
        lv_indev_t *indev = esp_lv_adapter_register_touch(&touch_cfg);
        assert(indev != NULL);
        ESP_LOGI(TAG, "LVGL touch input registered");
    }

    ESP_ERROR_CHECK(esp_lv_adapter_start());
    ESP_LOGI(TAG, "esp_lvgl_adapter started");
    return ESP_OK;
}

bool app_lvgl_lock(int timeout_ms)
{
    TickType_t ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return esp_lv_adapter_lock(ticks) == ESP_OK;
}

void app_lvgl_unlock(void)
{
    esp_lv_adapter_unlock();
}

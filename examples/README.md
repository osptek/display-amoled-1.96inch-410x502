# 示例工程 / Sample Projects

本目录收录本规格屏幕的示例工程，每个子文件夹是一个独立工程，打开对应文件夹按其中的 README 编译即可。
这些工程是参考代码，引脚、背光、触摸等配置请按你的硬件修改；选用前请对照仓库首页各型号的驱动 IC 和接口确认。

This folder holds sample projects for this display spec. Each sub-folder is a standalone project — open it and follow its own README to build.
They are reference code; adjust pins, backlight, touch and other settings to your hardware. Before choosing one, check the driver IC and interface against the models listed on the repository home page.

| 例程 / Project | 平台 / Platform | 驱动 IC / Driver IC | 接口 / Interface | 说明 / Description |
|---|---|---|---|---|
| [`esp32s3-1.96-410x502-st7801-bringup`](./esp32s3-1.96-410x502-st7801-bringup/) | ESP32-S3 · ESP-IDF 5 | ST7801 | QSPI | 点亮测试，含 QSPI AMOLED 驱动组件 / Bring-up test with QSPI AMOLED driver component |
| [`esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl8`](./esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl8/) | ESP32-S3 · ESP-IDF 5 | CO5300 | QSPI | LVGL 8，含 CST820 触摸 / LVGL 8 with CST820 touch |
| [`esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl8-te`](./esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl8-te/) | ESP32-S3 · ESP-IDF 5 | CO5300 | QSPI | LVGL 8、TE 防撕裂，含 CST820 触摸 / LVGL 8, TE tear-free, with CST820 touch |
| [`esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl9`](./esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl9/) | ESP32-S3 · ESP-IDF 5 | CO5300 | QSPI | LVGL 9，含 CST820 触摸 / LVGL 9 with CST820 touch |
| [`esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl9-te`](./esp32s3-idf5-co5300-qspi-lvgl-adapter-lvgl9-te/) | ESP32-S3 · ESP-IDF 5 | CO5300 | QSPI | LVGL 9、TE 防撕裂，含 CST820 触摸 / LVGL 9, TE tear-free, with CST820 touch |
| [`esp32s3-idf5-co5300-qspi-lvgl8-te-rotate90`](./esp32s3-idf5-co5300-qspi-lvgl8-te-rotate90/) | ESP32-S3 · ESP-IDF 5 | CO5300 | QSPI | LVGL 8、TE 防撕裂、软件旋转 90°，含 BSP 与 LVGL 移植层 / LVGL 8, TE tear-free, SW rotate 90°, with BSP and LVGL port |

如有问题请联系技术支持。 / Contact technical support if you have questions.

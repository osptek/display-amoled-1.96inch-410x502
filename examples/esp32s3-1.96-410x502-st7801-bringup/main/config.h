#pragma once

#include "driver/gpio.h"

#define BOARD_VERSION_BLACK     0
#define BOARD_VERSION_GREEN     1

#define BOARD_VERSION           BOARD_VERSION_GREEN

#if BOARD_VERSION == BOARD_VERSION_BLACK

#define BOARD_LCD_USE_TE        0

/* 黑色测试板 — LCD QSPI */
#define PIN_LCD_CS_NUM          (GPIO_NUM_14)
#define PIN_LCD_PCLK_NUM        (GPIO_NUM_9)
#define PIN_LCD_DATA0_NUM       (GPIO_NUM_10)
#define PIN_LCD_DATA1_NUM       (GPIO_NUM_11)
#define PIN_LCD_DATA2_NUM       (GPIO_NUM_12)
#define PIN_LCD_DATA3_NUM       (GPIO_NUM_13)
#define PIN_LCD_RST_NUM         (GPIO_NUM_15)

/* 黑色测试板 — Touch I2C (CST820) */
#define PIN_TP_SCL_NUM          (GPIO_NUM_42)
#define PIN_TP_SDA_NUM          (GPIO_NUM_41)
#define PIN_TP_RST_NUM          (GPIO_NUM_40)
#define PIN_TP_INT_NUM          (GPIO_NUM_39)

#elif BOARD_VERSION == BOARD_VERSION_GREEN

#define BOARD_LCD_USE_TE        1

/* 绿色测试板 — LCD QSPI */
#define PIN_LCD_CS_NUM          (GPIO_NUM_40)
#define PIN_LCD_PCLK_NUM        (GPIO_NUM_21)
#define PIN_LCD_DATA0_NUM       (GPIO_NUM_47)
#define PIN_LCD_DATA1_NUM       (GPIO_NUM_45)
#define PIN_LCD_DATA2_NUM       (GPIO_NUM_38)
#define PIN_LCD_DATA3_NUM       (GPIO_NUM_39)
#define PIN_LCD_RST_NUM         (GPIO_NUM_41)
#define PIN_LCD_TE_NUM          (GPIO_NUM_42)

/* 绿色测试板 — Touch I2C (CST820) */
#define PIN_TP_SCL_NUM          (GPIO_NUM_2)
#define PIN_TP_SDA_NUM          (GPIO_NUM_1)
#define PIN_TP_RST_NUM          (GPIO_NUM_13)
#define PIN_TP_INT_NUM          (GPIO_NUM_14)

#else
#error "Unknown BOARD_VERSION"
#endif

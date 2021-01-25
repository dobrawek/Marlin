/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * STM32F407VET6 with RAMPS-like shield
 * 'Black' STM32F407VET6 board - https://www.stm32duino.com/viewtopic.php?t=485
 * Shield - https://github.com/jmz52/Hardware
 */

#if NOT_TARGET(STM32F4, STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "Black STM32F4VET6 supports up to 2 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "QIDI X-MAX"
#endif

#define DEFAULT_MACHINE_NAME "STM32F407ZET6"

// Use one of these or SDCard-based Emulation will be used
#if NO_EEPROM_SELECTED
  //#define SRAM_EEPROM_EMULATION                 // Use BackSRAM-based EEPROM emulation
  #define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT


// Avoid conflict with fans and TIMER_TONE
#define TEMP_TIMER 3
#define STEP_TIMER 5


//#define I2C_EEPROM
//#define SRAM_EEPROM_EMULATION
//#define MARLIN_EEPROM_SIZE                0x2000  // 8KB


//
// Limit Switches
//
#define X_STOP_PIN                          PG10
#define Y_STOP_PIN                          PA12
#define Z_STOP_PIN                          PA14

//
// Steppers
//
#define X_ENABLE_PIN                        PC13
#define X_STEP_PIN                          PE5
#define X_DIR_PIN                           PE6

#define Y_ENABLE_PIN                        PE4
#define Y_STEP_PIN                          PE2
#define Y_DIR_PIN                           PE3

#define Z_ENABLE_PIN                        PE1
#define Z_STEP_PIN                          PB9
#define Z_DIR_PIN                           PE0

#define E0_ENABLE_PIN                       PB8
#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB5

#define E1_ENABLE_PIN                       PG8
#define E1_STEP_PIN                         PC7
#define E1_DIR_PIN                          PC6

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA1   // TH1
#define TEMP_BED_PIN                        PA0   // TB1

//
// Heaters
//
#define HEATER_0_PIN                        PG12  // HEATER1
#define HEATER_BED_PIN                      PG11  // HOT BED

//
// Fans
//
#define CONTROLLER_FAN_PIN                  PD6   // BOARD FAN
#define FAN_PIN                             PG13  // FAN
#define FAN2_PIN                            PG14

//
// Misc
//
#define BEEPER_PIN                          PB0
//#define LED_PIN                           -1
//#define POWER_LOSS_PIN                    -1
#define FIL_RUNOUT_PIN                      PA15

// SPI Flash
#define HAS_SPI_FLASH                          1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE                0x200000  // 2MB
#endif

// SPI 2
#define W25QXX_CS_PIN                       PB12
#define W25QXX_MOSI_PIN                     PB15
#define W25QXX_MISO_PIN                     PB14
#define W25QXX_SCK_PIN                      PB13

//
// TronXY TFT Support
//

#if HAS_FSMC_TFT

  // Shared FSMC

  #define TOUCH_CS_PIN                      PB7   // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define TFT_RESET_PIN                     PF11
  #define TFT_BACKLIGHT_PIN                 PD13

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
  #define FSMC_CS_PIN                       PD7
  #define FSMC_RS_PIN                       PD11

  #define TFT_CS_PIN                          FSMC_CS_PIN  // @dobrawek
  #define TFT_RS_PIN                          FSMC_RS_PIN  // @dobrawek


  #define FSMC_DMA_DEV                      DMA2
  #define FSMC_DMA_CHANNEL               DMA_CH5

#endif

#if ENABLED(TFT_LVGL_UI)
  // LVGL
  #define HAS_SPI_FLASH_FONT                   1
  #define HAS_GCODE_PREVIEW                    1
  #define HAS_GCODE_DEFAULT_VIEW_IN_FLASH      0
  #define HAS_LANG_SELECT_SCREEN               1
  #define HAS_BAK_VIEW_IN_FLASH                0
  #define HAS_LOGO_IN_FLASH                    0
#elif ENABLED(TFT_COLOR_UI)
  // Color UI
  #define TFT_DRIVER                     ILI9488
  #define TFT_BUFFER_SIZE                  14400
#endif

// XPT2046 Touch Screen calibration
#if ANY(TFT_LVGL_UI, TFT_COLOR_UI, TFT_CLASSIC_UI)
  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -17181
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y            11434
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   501
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                    -9
  #endif
#endif

// SPI1(PA7)=LCD & SPI3(PB5)=STUFF, are not available
// Needs to use SPI2
#define SPI_DEVICE                             2
#define SD_SCK_PIN                          PB13
#define SD_MISO_PIN                         PB14
#define SD_MOSI_PIN                         PB15
#define SD_SS_PIN                           PB12


// //
// // Servos
// //
// #define SERVO0_PIN                          PC6
// #define SERVO1_PIN                          PC7

// //
// // Limit Switches
// //
// #define X_MIN_PIN                           PC13
// #define X_MAX_PIN                           PA15
// #define Y_MIN_PIN                           PA5
// #define Y_MAX_PIN                           PD12
// #define Z_MIN_PIN                           PD14
// #define Z_MAX_PIN                           PD15

// //
// // Steppers
// //
// #define X_STEP_PIN                          PC4
// #define X_DIR_PIN                           PA4
// #define X_ENABLE_PIN                        PE7

// #define Y_STEP_PIN                          PE5
// #define Y_DIR_PIN                           PE2
// #define Y_ENABLE_PIN                        PE6

// #define Z_STEP_PIN                          PD5
// #define Z_DIR_PIN                           PD3
// #define Z_ENABLE_PIN                        PD6

// #define E0_STEP_PIN                         PD7
// #define E0_DIR_PIN                          PD0
// #define E0_ENABLE_PIN                       PB9

// #define E1_STEP_PIN                         PE0
// #define E1_DIR_PIN                          PE1
// #define E1_ENABLE_PIN                       PB8

// //
// // Temperature Sensors
// //
// #define TEMP_0_PIN                          PC0   // T0
// #define TEMP_1_PIN                          PC1   // T1
// #define TEMP_BED_PIN                        PC2   // TB

// #ifndef TEMP_CHAMBER_PIN
//   #define TEMP_CHAMBER_PIN                  PC3   // TC
// #endif

// //
// // Heaters / Fans
// //
// #define HEATER_0_PIN                        PA2   // Heater0
// #define HEATER_1_PIN                        PA3   // Heater1
// #define HEATER_BED_PIN                      PA1   // Hotbed

// #define FAN_PIN                             PE9   // Fan0
// #define FAN1_PIN                            PE11  // Fan1
// #define FAN2_PIN                            PE13  // Fan2
// #define FAN3_PIN                            PE14  // Fan3

// //
// // Misc. Functions
// //
// #define LED_PIN                             PA6
// //#define LED_PIN                           PA7
// #define KILL_PIN                            PB1

// //
// // LCD / Controller
// //
// //#define SD_DETECT_PIN                     PC5
// //#define SD_DETECT_PIN                     PA8   // SDIO SD_DETECT_PIN, external SDIO card reader only

// #define BEEPER_PIN                          PD10
// #define LCD_PINS_RS                         PE15
// #define LCD_PINS_ENABLE                     PD8
// #define LCD_PINS_D4                         PE10
// #define LCD_PINS_D5                         PE12
// #define LCD_PINS_D6                         PD1
// #define LCD_PINS_D7                         PE8
// #define BTN_ENC                             PD9
// #define BTN_EN1                             PD4
// #define BTN_EN2                             PD13

// #define DOGLCD_CS                    LCD_PINS_D5
// #define DOGLCD_A0                    LCD_PINS_D6

// #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
//   #define BTN_ENC_EN                 LCD_PINS_D7  // Detect the presence of the encoder
// #endif

// //
// // Onboard SD support
// //
// #define SDIO_D0_PIN                         PC8
// #define SDIO_D1_PIN                         PC9
// #define SDIO_D2_PIN                         PC10
// #define SDIO_D3_PIN                         PC11
// #define SDIO_CK_PIN                         PC12
// #define SDIO_CMD_PIN                        PD2

// #ifndef SDCARD_CONNECTION
//   #define SDCARD_CONNECTION              ONBOARD
// #endif

// #if SD_CONNECTION_IS(ONBOARD)
//   #define SDIO_SUPPORT                            // Use SDIO for onboard SD

//   #ifndef SDIO_SUPPORT
//     #define SOFTWARE_SPI                          // Use soft SPI for onboard SD
//     #define SDSS                     SDIO_D3_PIN
//     #define SD_SCK_PIN               SDIO_CK_PIN
//     #define SD_MISO_PIN              SDIO_D0_PIN
//     #define SD_MOSI_PIN             SDIO_CMD_PIN
//   #endif
// #endif

/** 
 * LCD driver for controller st7735.h / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        st7735.h
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#include <avr/pgmspace.h>

#ifndef __ST7735_H__
#define __ST7735_H__

  #ifndef PORT
    #define PORT PORTB
  #endif
  #ifndef DDR
    #define DDR  DDRB
  #endif
  #ifndef ST7735_DC_LD
    #define ST7735_DC_LD  PB1
  #endif
  #ifndef ST7735_BL
    #define ST7735_BL     PB2
  #endif
  #ifndef ST7735_CS_SD
    #define ST7735_CS_SD  PB3
  #endif
  #ifndef ST7735_CS_LD
    #define ST7735_CS_LD  PB4
  #endif
  #ifndef ST7735_MOSI
    #define ST7735_MOSI   PB5
  #endif
  #ifndef ST7735_MISO
    #define ST7735_MISO   PB6
  #endif
  #ifndef ST7735_SCK
    #define ST7735_SCK    PB7
  #endif

  #ifndef HW_RESET_DDR
    #define HW_RESET_DDR  DDR
  #endif
  #ifndef HW_RESET_PORT
    #define HW_RESET_PORT PORT
  #endif
  #ifndef HW_RESET_PIN
    #define HW_RESET_PIN  PB0
  #endif

  #define DELAY   0x80
  
  #define NOP     0x00
  #define SWRESET 0x01
  #define RDDID   0x04
  #define RDDST   0x09

  #define SLPIN   0x10
  #define SLPOUT  0x11
  #define PTLON   0x12
  #define NORON   0x13

  #define INVOFF  0x20
  #define INVON   0x21
  #define DISPOFF 0x28
  #define DISPON  0x29
  #define RAMRD   0x2E
  #define CASET   0x2A
  #define RASET   0x2B
  #define RAMWR   0x2C

  #define PTLAR   0x30
  #define MADCTL  0x36
  #define COLMOD  0x3A

  #define FRMCTR1 0xB1
  #define FRMCTR2 0xB2
  #define FRMCTR3 0xB3
  #define INVCTR  0xB4
  #define DISSET5 0xB6

  #define PWCTR1  0xC0
  #define PWCTR2  0xC1
  #define PWCTR3  0xC2
  #define PWCTR4  0xC3
  #define PWCTR5  0xC4
  #define VMCTR1  0xC5

  #define RDID1   0xDA
  #define RDID2   0xDB
  #define RDID3   0xDC
  #define RDID4   0xDD

  #define GMCTRP1 0xE0
  #define GMCTRN1 0xE1

  #define PWCTR6  0xFC

  // MV = 0 in MADCTL
  // max columns
  #define MAX_X 162
  // max rows
  #define MAX_Y 132
  // columns max counter
  #define SIZE_X MAX_X - 1
  // rows max counter
  #define SIZE_Y MAX_Y - 1
  // whole pixels
  #define CACHE_SIZE_MEM (MAX_X * MAX_Y)
  // number of columns for chars
  #define CHARS_COLS_LEN 5
  // number of rows for chars
  #define CHARS_ROWS_LEN 8

  /** @const Command list ST7735B */
  extern const uint8_t INIT_ST7735B[];

  /** @const Characters */
  extern const uint8_t CHARACTERS[][CHARS_COLS_LEN];

  /** @enum Font sizes */
  typedef enum {
    // normal font size: 1x high & 1x wide
    X1 = 0x00,
    // bigger font size: 2x higher & 1x wide
    X2 = 0x01,
    // the biggest font size: font 2x higher & 2x wider
    // 0x0A is set cause offset 5 position to right only for
    //      this case and no offset for previous cases X1, X2
    //      when draw the characters of string in DrawString()
    X3 = 0x0A
  } ESizes;

  /**
   * @description     Hardware Reset
   *
   * @param void
   * @return void
   */
  void HardwareReset(void);

  /**
   * @description     Initialise SPI communication
   *
   * @param void
   * @return void
   */
  void SpiInit(void);

  /**
   * @description     Initialise st7735 driver
   *
   * @param void
   * @return void
   */
  void St7735Init(void);

  /**
   * @description     Send list commands
   *
   * @param uint8_t*  array of commands
   * @return void
   */
  void St7735Commands(const uint8_t *commands);

  /**
   * @description     Command send
   *
   * @param uint8_t   command
   * @return void
   */
  uint8_t CommandSend(uint8_t);

  /**
   * @description     8 bits data send
   *
   * @param uint8_t   data
   * @return void
   */
  uint8_t Data8BitsSend(uint8_t);

  /**
   * @description     16 bits data send
   *
   * @param uint16_t  data
   * @return void
   */
  uint8_t Data16BitsSend(uint16_t);

  /**
   * @description     Set window
   *
   * @param uint8_t   x - start position
   * @param uint8_t   x - end position
   * @param uint8_t   y - start position
   * @param uint8_t   y - end position
   * @return uint8_t
   */
  uint8_t SetWindow(uint8_t, uint8_t, uint8_t, uint8_t);

  /**
   * @description     Set pixel position x, y
   *
   * @param uint8_t   x - position
   * @param uint8_t   y - position
   * @return void
   */
  char SetPosition(uint8_t, uint8_t);

  /**
   * @description     Set partial area / window
   *
   * @param uint8_t   x - start row 
   * @param uint8_t   x - end row
   * @return uint8_t
   */
  uint8_t SetPartialArea(uint8_t, uint8_t);

  /**
   * @description     Write color pixels
   *
   * @param uint16_t  color
   * @param uint16_t  number of cycles
   * @return void
   */
  void SendColor565(uint16_t, uint16_t);

  /**
   * @description     Draw pixel
   *
   * @param uint8_t   x position
   * @param uint8_t   y position
   * @param uint16_t  color
   * @return void
   */
  void DrawPixel(uint8_t, uint8_t, uint16_t);

  /**
   * @description     Draw character
   *
   * @param char      character
   * @param uint16_t  color
   * @param Esizes    see enum sizes in st7735.h
   * @return void
   */
  char DrawChar(char, uint16_t, ESizes);

  /**
   * @description     Draw string
   *
   * @param char*     string
   * @param uint16_t  color
   * @param Esizes    see enum sizes in st7735.h
   * @return void
   */
  void DrawString(char*, uint16_t, ESizes);

  /**
   * @description     Draw line
   *
   * @param uint8_t   x - start position
   * @param uint8_t   x - end position
   * @param uint8_t   y - start position
   * @param uint8_t   y - end position
   * @param uint16_t  color
   * @return void
   */
  char DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @description     Fast draw line horizontal
   *
   * @param uint8_t   x - start position
   * @param uint8_t   x - end position
   * @param uint8_t   y - position
   * @param uint16_t  color
   * @return void
   */
  void DrawLineHorizontal(uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @description     Fast draw line vertical
   *
   * @param uint8_t   x - position
   * @param uint8_t   y - start position
   * @param uint8_t   y - end position
   * @param uint16_t  color
   * @return void
   */
  void DrawLineVertical(uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @description     Clear screen
   *
   * @param uint16_t color
   * @return void
   */
  void ClearScreen(uint16_t);

  /**
   * @description     Update screen
   *
   * @param void
   * @return void
   */
  void UpdateScreen(void);

  /**
   * @description     Delay
   *l
   * @param uint8_t   delay in milliseconds
   * @return void
   */
  void DelayMs(uint8_t);

#endif

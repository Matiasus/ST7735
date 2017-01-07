/*** 
 * LCD driver for controller st7735 / 1.8 tft display /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author  Marian Hrinko
 * @datum   24.12.2016
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
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
  #define MAX_X 132
  // max rows
  #define MAX_Y 162
  // columns max counter
  #define SIZE_X MAX_X - 1
  // rows max counter
  #define SIZE_Y MAX_Y - 1
  // whole pixels
  #define CACHE_SIZE_MEM (MAX_X * MAX_Y)

  /** @const Command list ST7735B */
  extern const uint8_t INIT_ST7735B[];

  /** @const Characters */
  extern const uint8_t CHARACTERS[][5];

  /** @enum Command or data */
  typedef enum {
    DATA = 1,     // active high
    COMMAND = 0,  // active low
  } enumCmdOrData;

  /**
   * @description Hardware Reset
   *
   * @param void
   * @return void
   */
  void HardwareReset(void);

  /**
   * @description Initialise SPI communication
   *
   * @param void
   * @return void
   */
  void SpiInit(void);

  /**
   * @description End SPI communication
   *
   * @param void
   * @return void
   */
  void SpiEnd(void);

  /**
   * @description Initialise st7735 driver
   *
   * @param void
   * @return void
   */
  void St7735Init(void);

  /**
   * @description Send list commands
   *
   * @param uint8_t *
   * @return void
   */
  void St7735Commands(const uint8_t *commands);

  /**
   * @description Command/data send
   *
   * @param EnumCmdOrData /see st7735.h/
   * @param uint8_t
   * @return uint8_t
   */
  uint8_t CmdOrDataSend(enumCmdOrData, uint8_t);

  /**
   * @description Set window
   *
   * @param uint8_t x - start position
   * @param uint8_t x - end position
   * @param uint8_t y - start position
   * @param uint8_t y - end position
   * @return uint8_t
   */
  uint8_t SetWindow(uint8_t, uint8_t, uint8_t, uint8_t);

  /**
   * @description Send word - 2 bytes (16 bites)
   *
   * @param uint16_t
   * @param uint16_t 
   * @return void
   */
  void SendColor565(uint16_t, uint16_t);

  /**
   * @description Clear screen
   *
   * @param uint16_t color
   * @return void
   */
  void ClearScreen(uint16_t);

  /**
   * @description Update screen
   *
   * @param void
   * @return void
   */
  void UpdateScreen(void);

  /**
   * @description Draw pixel
   *
   * @param uint8_t
   * @param uint8_t
   * @param uint16_t
   * @return void
   */
  void DrawPixel(uint8_t, uint8_t, uint16_t);

  /**
   * @description Draw character
   *
   * @param char
   * @param uint16_t color
   * @return void
   */
  char DrawChar(char, uint16_t);

  /**
   * @description Draw string
   *
   * @param char*
   * @param uint16_t color
   * @return void
   */
  void DrawString(char*, uint16_t);

  /**
   * @description Set pixel position x, y
   *
   * @param uint8_t x - position
   * @param uint8_t y - position
   * @return void
   */
  char SetPosition(uint8_t, uint8_t);

  /**
   * @description Draw line
   *
   * @param uint8_t x - start position
   * @param uint8_t x - end position
   * @param uint8_t y - start position
   * @param uint8_t y - end position
   * @param uint16_t  color
   * @return void
   */
  char DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @description Delay
   *l
   * @param uint8_t delay in milliseconds
   * @return void
   */
  void DelayMs(uint8_t);

#endif

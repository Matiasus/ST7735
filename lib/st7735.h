/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        ST773 1.8" LCD Driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       13.10.2020
 * @file        st7735.h
 * @tested      AVR Atmega16
 *
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 */

#include <avr/pgmspace.h>

#ifndef __ST7735_H__
#define __ST7735_H__

  #ifndef HW_RESET_DDR
    #define HW_RESET_DDR  DDRB
  #endif
  #ifndef HW_RESET_PORT
    #define HW_RESET_PORT PORTB
  #endif
  #ifndef HW_RESET_PIN
    #define HW_RESET_PIN  0
  #endif

  #ifndef PORT
    #define PORT PORTB
  #endif
  #ifndef DDR
    #define DDR  DDRB
  #endif
  #ifndef ST7735_DC_LD
    #define ST7735_DC_LD  1
  #endif
  #ifndef ST7735_BL
    #define ST7735_BL     2
  #endif
  #ifndef ST7735_CS_SD
    #define ST7735_CS_SD  3
  #endif
  #ifndef ST7735_CS_LD
    #define ST7735_CS_LD  4
  #endif
  #ifndef ST7735_MOSI         // SDA
    #define ST7735_MOSI   5
  #endif
  #ifndef ST7735_MISO
    #define ST7735_MISO   6
  #endif
  #ifndef ST7735_SCK         // SCL
    #define ST7735_SCK    7
  #endif

  #define DELAY           0x80
  
  #define NOP             0x00
  #define SWRESET         0x01
  #define RDDID           0x04
  #define RDDST           0x09

  #define SLPIN           0x10
  #define SLPOUT          0x11
  #define PTLON           0x12
  #define NORON           0x13

  #define INVOFF          0x20
  #define INVON           0x21
  #define DISPOFF         0x28
  #define DISPON          0x29
  #define RAMRD           0x2E
  #define CASET           0x2A
  #define RASET           0x2B
  #define RAMWR           0x2C

  #define PTLAR           0x30
  #define MADCTL          0x36
  #define COLMOD          0x3A

  #define FRMCTR1         0xB1
  #define FRMCTR2         0xB2
  #define FRMCTR3         0xB3
  #define INVCTR          0xB4
  #define DISSET5         0xB6

  #define PWCTR1          0xC0
  #define PWCTR2          0xC1
  #define PWCTR3          0xC2
  #define PWCTR4          0xC3
  #define PWCTR5          0xC4
  #define VMCTR1          0xC5

  #define RDID1           0xDA
  #define RDID2           0xDB
  #define RDID3           0xDC
  #define RDID4           0xDD

  #define GMCTRP1         0xE0
  #define GMCTRN1         0xE1

  #define PWCTR6          0xFC

  // Colors
  #define BLACK           0x0000
  #define WHITE           0xFFFF
  #define RED             0xF000

  #define ST7735_SUCCESS  0
  #define ST7735_ERROR    1

  #define MAX_X           161               // max columns / MV = 0 in MADCTL
  #define MAX_Y           130               // max rows / MV = 0 in MADCTL
  #define SIZE_X          MAX_X - 1         // columns max counter
  #define SIZE_Y          MAX_Y - 1         // rows max counter
  #define CACHE_SIZE_MEM  (MAX_X * MAX_Y)   // whole pixels
  #define CHARS_COLS_LEN  5                 // number of columns for chars
  #define CHARS_ROWS_LEN  8                 // number of rows for chars

  /** @const Command list ST7735B */
  extern const uint8_t INIT_ST7735B[];
  /** @var array Chache memory char index row */
  unsigned short int cacheMemIndexRow;
  /** @var array Chache memory char index column */
  unsigned short int cacheMemIndexCol;

  /** @enum Font sizes */
  typedef enum {
    // 1x high & 1x wide size
    X1 = 0x00,
    // 2x high & 1x wide size
    X2 = 0x80,
    // 2x high & 2x wider size
    // 0x0A is set because need to offset 5 position to right
    //      when draw the characters of string 
    X3 = 0x81
  } ESizes;

  /**
   * @desc    Hardware Reset
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_HWReset(void);

  /**
   * @desc    Init SPI communication
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_SpiInit(void);

  /**
   * @desc    Init st7735 driver
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_Init(void);

  /**
   * @desc    Send list commands
   *
   * @param   const uint8_t*
   *
   * @return  void
   */
  void ST7735_Commands(const uint8_t *commands);

  /**
   * @desc    Command send
   *
   * @param   uint8_t
   *
   * @return void
   */
  uint8_t ST7735_CommandSend(uint8_t);

  /**
   * @desc    8bits data send
   *
   * @param   uint8_t
   *
   * @return  void
   */
  uint8_t ST7735_Data8BitsSend(uint8_t);

  /**
   * @desc    16bits data send
   *
   * @param   uint8_t
   *
   * @return  void
   */
  uint8_t ST7735_Data16BitsSend(uint16_t);

  /**
   * @desc    Set window
   *
   * @param   uint8_t x - start position
   * @param   uint8_t x - end position
   * @param   uint8_t y - start position
   * @param   uint8_t y - end position
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetWindow(uint8_t, uint8_t, uint8_t, uint8_t);

  /**
   * @desc    Check text position x, y
   *
   * @param   unsigned char
   * @param   unsigned char
   * @param   unsigned char
   *
   * @return  char
   */
  char ST7735_CheckPosition(unsigned char, unsigned char, unsigned char, ESizes);

  /**
   * @desc    Set pixel position x, y
   *
   * @param   uint8_t x - position
   * @param   uint8_t y - position
   *
   * @return  char
   */
  char ST7735_SetPosition(uint8_t, uint8_t);

  /**
   * @desc    Set partial area / window
   *
   * @param   uint8_t x - start row 
   * @param   uint8_t x - end row
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetPartialArea(uint8_t, uint8_t);

  /**
   * @desc    Write color pixels
   *
   * @param   uint16_t color
   * @param   uint16_t number of cycles
   *
   * @return  void
   */
  void ST7735_SendColor565(uint16_t, uint16_t);

  /**
   * @desc    Draw pixel
   *
   * @param   uint8_t x position
   * @param   uint8_t y position
   * @param   uint16_t color
   * 
   * @return  void
   */
  void ST7735_DrawPixel(uint8_t, uint8_t, uint16_t);

  /**
   * @description     Draw character
   *
   * @param char      character
   * @param uint16_t  color
   * @param Esizes    see enum sizes in st7735.h
   * @return void
   */
  char ST7735_DrawChar(char, uint16_t, ESizes);

  /**
   * @desc    Draw string
   *
   * @param   char *
   * @param   uint16_t
   * @param   Esizes

   * @return void
   */
  void ST7735_DrawString(char*, uint16_t, ESizes);

  /**
   * @desc    Draw line
   *
   * @param   uint8_t x - start position
   * @param   uint8_t x - end position
   * @param   uint8_t y - start position
   * @param   uint8_t y - end position
   * @param   uint16_t color
   *
   * @return void
   */
  char ST7735_DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @desc    Fast draw line horizontal
   *
   * @param   uint8_t x - start position
   * @param   uint8_t x - end position
   * @param   uint8_t y - position
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_DrawLineHorizontal(uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @desc    Fast draw line vertical
   *
   * @param   uint8_t x - position
   * @param   uint8_t y - start position
   * @param   uint8_t y - end position
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_DrawLineVertical(uint8_t, uint8_t, uint8_t, uint16_t);

  /**
   * @desc    Draw rectangle
   *
   * @param   uint8_t x - start position
   * @param   uint8_t x - end position
   * @param   uint8_t y - start position
   * @param   uint8_t y - end position
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_DrawRectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);


  /**
   * @desc    Clear screen
   *
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_ClearScreen(uint16_t);

  /**
   * @description     Update screen
   *
   * @param void
   *
   * @return void
   */
  void ST7735_UpdateScreen(void);

  /**
   * @desc    Delay
   *
   * @param   uint8_t ms
   *
   * @return  void
   */
  void ST7735_DelayMs(uint8_t);

#endif

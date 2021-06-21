/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        ST773 1.8" LCD Driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       13.10.2020
 * @update      21.06.2021
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

  // Success
  #define ST7735_SUCCESS        0
  // Error
  #define ST7735_ERROR          1

  // PORT/PIN definition
  // -----------------------------------
  #define HW_RESET_DDR          DDRB
  #define HW_RESET_PORT         PORTB
  #define HW_RESET_PIN          0

  #define PORT                  PORTB
  #define DDR                   DDRB 
  #define ST7735_DC_LD          1
  #define ST7735_CS             4
  #define ST7735_MOSI           5 // SDA
  #define ST7735_MISO           6
  #define ST7735_SCK            7 // SCL

  // 1st LCD Display
  // -----------------------------------  
  #define LCD1_CS               3
  #define LCD1_BL               2

  // Command definition
  // -----------------------------------
  #define DELAY                 0x80
  
  #define NOP                   0x00
  #define SWRESET               0x01
  #define RDDID                 0x04
  #define RDDST                 0x09

  #define SLPIN                 0x10
  #define SLPOUT                0x11
  #define PTLON                 0x12
  #define NORON                 0x13

  #define INVOFF                0x20
  #define INVON                 0x21
  #define DISPOFF               0x28
  #define DISPON                0x29
  #define RAMRD                 0x2E
  #define CASET                 0x2A
  #define RASET                 0x2B
  #define RAMWR                 0x2C

  #define PTLAR                 0x30
  #define MADCTL                0x36
  #define COLMOD                0x3A

  #define FRMCTR1               0xB1
  #define FRMCTR2               0xB2
  #define FRMCTR3               0xB3
  #define INVCTR                0xB4
  #define DISSET5               0xB6

  #define PWCTR1                0xC0
  #define PWCTR2                0xC1
  #define PWCTR3                0xC2
  #define PWCTR4                0xC3
  #define PWCTR5                0xC4
  #define VMCTR1                0xC5

  #define RDID1                 0xDA
  #define RDID2                 0xDB
  #define RDID3                 0xDC
  #define RDID4                 0xDD

  #define GMCTRP1               0xE0
  #define GMCTRN1               0xE1

  #define PWCTR6                0xFC

  // Colors
  // -----------------------------------
  #define BLACK                 0x0000
  #define WHITE                 0xFFFF
  #define RED                   0xF000

  // AREA definition
  // -----------------------------------
  #define MAX_X                 161               // max columns / MV = 0 in MADCTL
  #define MAX_Y                 130               // max rows / MV = 0 in MADCTL
  #define SIZE_X                MAX_X - 1         // columns max counter
  #define SIZE_Y                MAX_Y - 1         // rows max counter
  #define CACHE_SIZE_MEM        (MAX_X * MAX_Y)   // whole pixels
  #define CHARS_COLS_LEN        5                 // number of columns for chars
  #define CHARS_ROWS_LEN        8                 // number of rows for chars

  // set bit
  #define SET_BIT(port, bit)    ((port) |= (1 << (bit)))
  // clear bit
  #define CLR_BIT(port, bit)    ((port) &= ~(1 << (bit)))
  // bit is set
  #define BIT_IS_SET(port, bit) (((port) & (1 << bit)) ? 1 : 0)

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

  /** @struct Chip Select */
  struct signal {
    // ddr
    char ddr;
    // port
    char port;   
    // pin
    char pin;
  };

  /** @struct lcd */
  struct st7735 {
    // Chip Select
    struct signal * cs;
    // Back Light
    struct signal * bl;
  };

  /**
   * @desc    Hardware Reset
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_HWReset (void);

  /**
   * @desc    Init SPI
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_SPI_Init (void);

  /**
   * @desc    Init CS
   *
   * @param   struct st7735 *
   *
   * @return  void
   */
  void ST7735_CS_Init (struct st7735 *);

  /**
   * @desc    Init BackLight
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_BL_Init (struct st7735 *);

  /**
   * @desc    Init st7735 driver
   *
   * @param   struct st7735
   *
   * @return  void
   */
  void ST7735_Init (struct st7735 *);

  /**
   * @desc    Send list commands
   *
   * @param   struct st7735 *
   * @param   const uint8_t *
   *
   * @return  void
   */
  void ST7735_Commands (struct st7735 *, const uint8_t *commands);

  /**
   * @desc    Command send
   *
   * @param   struct st7735 *
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_CommandSend (struct st7735 *, uint8_t);

  /**
   * @desc    8bits data send
   *
   * @param   struct st7735 *
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_Data8BitsSend (struct st7735 *, uint8_t);

  /**
   * @desc    16bits data send
   *
   * @param   struct st7735 *
   * @param   uint16_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_Data16BitsSend (struct st7735 *, uint16_t);

  /**
   * @desc    Set window
   *
   * @param   struct st7735 *
   * @param   uint8_t
   * @param   uint8_t
   * @param   uint8_t
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetWindow (struct st7735 *, uint8_t, uint8_t, uint8_t, uint8_t);

  /**
   * @desc    Check text position
   *
   * @param   unsigned char
   * @param   unsigned char
   * @param   unsigned char
   * @param   Esizes
   *
   * @return  uint8_t
   */
  uint8_t ST7735_CheckPosition (uint8_t, uint8_t, uint8_t, ESizes);

  /**
   * @desc    Set pixel position x, y
   *
   * @param   uint8_t
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetPosition (uint8_t, uint8_t);

  /**
   * @desc    Set partial area / window
   *
   * @param   uint8_t
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetPartialArea (uint8_t, uint8_t);

  /**
   * @desc    Write color pixels
   *
   * @param   struct st7735 *
   * @param   uint16_t
   * @param   uint16_t
   *
   * @return  void
   */
  void ST7735_SendColor565 (struct st7735 *, uint16_t, uint16_t);

  /**
   * @desc    Draw pixel
   *
   * @param   struct st7735 *
   * @param   uint8_t
   * @param   uint8_t
   * @param   uint16_t
   * 
   * @return  void
   */
  void ST7735_DrawPixel (struct st7735 *, uint8_t, uint8_t, uint16_t);

  /**
   * @desc    Clear screen
   *
   * @param   struct st7735 *
   * @param   uint16_t
   *
   * @return  void
   */
  void ST7735_ClearScreen (struct st7735 *, uint16_t);

  /**
   * @desc    Update screen
   *
   * @param   struct st7735 *
   *
   * @return  void
   */
  void ST7735_UpdateScreen (struct st7735 *);

  /**
   * @desc    Delay
   *
   * @param   uint8_t
   *
   * @return  void
   */
  void ST7735_DelayMs (uint8_t);

#endif

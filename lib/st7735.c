/** 
 * LCD driver for controller st7735.c / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        st7735.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#ifndef F_CPU
  #define F_CPU 16000000
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "spi.h"
#include "st7735.h"

/** @array Init command */
const uint8_t INIT_ST7735B[] PROGMEM = {
    // 11 commands in list:
    11,
    // Software reset
    //  no arguments
    //  delay
    SWRESET, 
        DELAY,  
          200,  // 200 ms delay
    // Out of sleep mode, 
    //  no arguments, 
    //  delay
    SLPOUT,
        DELAY,  
          200,  // 200 ms delay
    // Set color mode, 
    //  1 argument
    //  delay
    COLMOD, 
      1+DELAY,  
         0x05,  // 16-bit color
           10,  // 10 ms
    // Frame rate control, 
    //  3 arguments
    //  delay
    FRMCTR1,
      3+DELAY,  
         0x00,  // fastest refresh
         0x06,  // 6 lines front porch
         0x03,  // 3 lines back porch
           10,  // 10 ms delay
    // Inversion mode off
     INVOFF,
        DELAY,  
           10,
    // Memory access ctrl (directions), 
    //  1 argument
    //  no delay
     MADCTL, 
            1,
         // D7  D6  D5  D4  D3  D2  D1  D0
         // MY  MX  MV  ML RGB  MH   -   -
         // ------------------------------
         // ------------------------------
         // MV  MX  MY -> {MV (row / column exchange) MX (column address order), MY (row address order)}
         // ------------------------------
         //  0   0   0 -> begin left-up corner, end right-down corner 
         //               left-right (normal view) 
         //  0   0   1 -> begin left-down corner, end right-up corner 
         //               left-right (Y-mirror)
         //  0   1   0 -> begin right-up corner, end left-down corner 
         //               right-left (X-mirror)
         //  0   1   1 -> begin right-down corner, end left-up corner
         //               right-left (X-mirror, Y-mirror)
         //  1   0   0 -> begin left-up corner, end right-down corner
         //               up-down (X-Y exchange)  
         //  1   0   1 -> begin left-down corner, end right-up corner
         //               down-up (X-Y exchange, Y-mirror)
         //  1   1   0 -> begin right-up corner, end left-down corner 
         //               up-down (X-Y exchange, X-mirror)  
         //  1   1   1 -> begin right-down corner, end left-up corner
         //               down-up (X-Y exchange, X-mirror, Y-mirror)
         // ------------------------------
         //  ML: vertical refresh order 
         //      0 -> refresh top to bottom 
         //      1 -> refresh bottom to top
         // ------------------------------
         // RGB: filter panel
         //      0 -> RGB 
         //      1 -> BGR        
         // ------------------------------ 
         //  MH: horizontal refresh order 
         //      0 -> refresh left to right 
         //      1 -> refresh right to left
         // 0xA0 = 1010 0000  
         0xA0,
    // Display settings #5, 
    //  2 arguments 
    //  no delay
    DISSET5, 
            2,  
         0x15,  // 1 clk cycle nonoverlap, 2 cycle gate
                // rise, 3 cycle osc equalize
         0x02,  // Fix on VTL
    // Display inversion control, 
    //  1 argument
    //  no delay
     INVCTR,
            1,  
          0x0,  //     Line inversion
    // Magical unicorn dust, 
    //  16 arguments
    //  no delay
    GMCTRP1,
           16,
         0x09, 
         0x16, 
         0x09, 
         0x20,
         0x21, 
         0x1B,
         0x13,
         0x19,
         0x17,
         0x15,
         0x1E,
         0x2B,
         0x04,
         0x05,
         0x02,
         0x0E,
    // Sparkles and rainbows 
    //  16 arguments 
    //  delay
    GMCTRN1,
     16+DELAY,  
         0x0B,
         0x14,
         0x08,
         0x1E, 
         0x22,
         0x1D,
         0x18,
         0x1E,
         0x1B,
         0x1A,
         0x24,  
         0x2B,
         0x06,
         0x06,
         0x02,
         0x0F,
           10,  // 10 ms delay
    // Normal display on
    //  no arguments
    //  delay
      NORON, 
        DELAY, 
           10,  // 10 ms delay
/*
    // Main screen turn on
    //  no arguments
    //  delay
     DISPON,
        DELAY,  
          200   // 200 ms delay
*/
};
/** @array Charset */
const uint8_t CHARACTERS[][5] PROGMEM = {
  { 0x00, 0x00, 0x00, 0x00, 0x00 }, // 20 space
  { 0x00, 0x00, 0x5f, 0x00, 0x00 }, // 21 !
  { 0x00, 0x07, 0x00, 0x07, 0x00 }, // 22 "
  { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // 23 #
  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // 24 $
  { 0x23, 0x13, 0x08, 0x64, 0x62 }, // 25 %
  { 0x36, 0x49, 0x55, 0x22, 0x50 }, // 26 &
  { 0x00, 0x05, 0x03, 0x00, 0x00 }, // 27 '
  { 0x00, 0x1c, 0x22, 0x41, 0x00 }, // 28 (
  { 0x00, 0x41, 0x22, 0x1c, 0x00 }, // 29 )
  { 0x14, 0x08, 0x3e, 0x08, 0x14 }, // 2a *
  { 0x08, 0x08, 0x3e, 0x08, 0x08 }, // 2b +
  { 0x00, 0x50, 0x30, 0x00, 0x00 }, // 2c ,
  { 0x08, 0x08, 0x08, 0x08, 0x08 }, // 2d -
  { 0x00, 0x60, 0x60, 0x00, 0x00 }, // 2e .
  { 0x20, 0x10, 0x08, 0x04, 0x02 }, // 2f /
  { 0x3e, 0x51, 0x49, 0x45, 0x3e }, // 30 0
  { 0x00, 0x42, 0x7f, 0x40, 0x00 }, // 31 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 32 2
  { 0x21, 0x41, 0x45, 0x4b, 0x31 }, // 33 3
  { 0x18, 0x14, 0x12, 0x7f, 0x10 }, // 34 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 35 5
  { 0x3c, 0x4a, 0x49, 0x49, 0x30 }, // 36 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 37 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 38 8
  { 0x06, 0x49, 0x49, 0x29, 0x1e }, // 39 9
  { 0x00, 0x36, 0x36, 0x00, 0x00 }, // 3a :
  { 0x00, 0x56, 0x36, 0x00, 0x00 }, // 3b ;
  { 0x08, 0x14, 0x22, 0x41, 0x00 }, // 3c <
  { 0x14, 0x14, 0x14, 0x14, 0x14 }, // 3d =
  { 0x00, 0x41, 0x22, 0x14, 0x08 }, // 3e >
  { 0x02, 0x01, 0x51, 0x09, 0x06 }, // 3f ?
  { 0x32, 0x49, 0x79, 0x41, 0x3e }, // 40 @
  { 0x7e, 0x11, 0x11, 0x11, 0x7e }, // 41 A
  { 0x7f, 0x49, 0x49, 0x49, 0x36 }, // 42 B
  { 0x3e, 0x41, 0x41, 0x41, 0x22 }, // 43 C
  { 0x7f, 0x41, 0x41, 0x22, 0x1c }, // 44 D
  { 0x7f, 0x49, 0x49, 0x49, 0x41 }, // 45 E
  { 0x7f, 0x09, 0x09, 0x09, 0x01 }, // 46 F
  { 0x3e, 0x41, 0x49, 0x49, 0x7a }, // 47 G
  { 0x7f, 0x08, 0x08, 0x08, 0x7f }, // 48 H
  { 0x00, 0x41, 0x7f, 0x41, 0x00 }, // 49 I
  { 0x20, 0x40, 0x41, 0x3f, 0x01 }, // 4a J
  { 0x7f, 0x08, 0x14, 0x22, 0x41 }, // 4b K
  { 0x7f, 0x40, 0x40, 0x40, 0x40 }, // 4c L
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f }, // 4d M
  { 0x7f, 0x04, 0x08, 0x10, 0x7f }, // 4e N
  { 0x3e, 0x41, 0x41, 0x41, 0x3e }, // 4f O
  { 0x7f, 0x09, 0x09, 0x09, 0x06 }, // 50 P
  { 0x3e, 0x41, 0x51, 0x21, 0x5e }, // 51 Q
  { 0x7f, 0x09, 0x19, 0x29, 0x46 }, // 52 R
  { 0x46, 0x49, 0x49, 0x49, 0x31 }, // 53 S
  { 0x01, 0x01, 0x7f, 0x01, 0x01 }, // 54 T
  { 0x3f, 0x40, 0x40, 0x40, 0x3f }, // 55 U
  { 0x1f, 0x20, 0x40, 0x20, 0x1f }, // 56 V
  { 0x3f, 0x40, 0x38, 0x40, 0x3f }, // 57 W
  { 0x63, 0x14, 0x08, 0x14, 0x63 }, // 58 X
  { 0x07, 0x08, 0x70, 0x08, 0x07 }, // 59 Y
  { 0x61, 0x51, 0x49, 0x45, 0x43 }, // 5a Z
  { 0x00, 0x7f, 0x41, 0x41, 0x00 }, // 5b [
  { 0x02, 0x04, 0x08, 0x10, 0x20 }, // 5c backslash
  { 0x00, 0x41, 0x41, 0x7f, 0x00 }, // 5d ]
  { 0x04, 0x02, 0x01, 0x02, 0x04 }, // 5e ^
  { 0x40, 0x40, 0x40, 0x40, 0x40 }, // 5f _
  { 0x00, 0x01, 0x02, 0x04, 0x00 }, // 60 `
  { 0x20, 0x54, 0x54, 0x54, 0x78 }, // 61 a
  { 0x7f, 0x48, 0x44, 0x44, 0x38 }, // 62 b
  { 0x38, 0x44, 0x44, 0x44, 0x20 }, // 63 c
  { 0x38, 0x44, 0x44, 0x48, 0x7f }, // 64 d
  { 0x38, 0x54, 0x54, 0x54, 0x18 }, // 65 e
  { 0x08, 0x7e, 0x09, 0x01, 0x02 }, // 66 f
  { 0x0c, 0x52, 0x52, 0x52, 0x3e }, // 67 g
  { 0x7f, 0x08, 0x04, 0x04, 0x78 }, // 68 h
  { 0x00, 0x44, 0x7d, 0x40, 0x00 }, // 69 i
  { 0x20, 0x40, 0x44, 0x3d, 0x00 }, // 6a j
  { 0x7f, 0x10, 0x28, 0x44, 0x00 }, // 6b k
  { 0x00, 0x41, 0x7f, 0x40, 0x00 }, // 6c l
  { 0x7c, 0x04, 0x18, 0x04, 0x78 }, // 6d m
  { 0x7c, 0x08, 0x04, 0x04, 0x78 }, // 6e n
  { 0x38, 0x44, 0x44, 0x44, 0x38 }, // 6f o
  { 0x7c, 0x14, 0x14, 0x14, 0x08 }, // 70 p
  { 0x08, 0x14, 0x14, 0x14, 0x7c }, // 71 q
  { 0x7c, 0x08, 0x04, 0x04, 0x08 }, // 72 r
  { 0x48, 0x54, 0x54, 0x54, 0x20 }, // 73 s
  { 0x04, 0x3f, 0x44, 0x40, 0x20 }, // 74 t
  { 0x3c, 0x40, 0x40, 0x20, 0x7c }, // 75 u
  { 0x1c, 0x20, 0x40, 0x20, 0x1c }, // 76 v
  { 0x3c, 0x40, 0x30, 0x40, 0x3c }, // 77 w
  { 0x44, 0x28, 0x10, 0x28, 0x44 }, // 78 x
  { 0x0c, 0x50, 0x50, 0x50, 0x3c }, // 79 y
  { 0x44, 0x64, 0x54, 0x4c, 0x44 }, // 7a z
  { 0x00, 0x08, 0x36, 0x41, 0x00 }, // 7b {
  { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // 7c |
  { 0x00, 0x41, 0x36, 0x08, 0x00 }, // 7d }
  { 0x10, 0x08, 0x08, 0x10, 0x08 }, // 7e ~
  { 0x00, 0x00, 0x00, 0x00, 0x00 }  // 7f
};

/** @var array Chache memory char index row */
int cacheMemIndexRow = 0;
/** @var array Chache memory char index column */
int cacheMemIndexCol = 0;

/**
 * @description Hardware Reset Impulse - minimal time required 120 ms
 *
 * @param void
 * @return void
 */
void HardwareReset(void)
{
  // Actiavte pull-up register logical high on pin RST
  HW_RESET_PORT |= (1 << HW_RESET_PIN);
  // DDR as output
  HW_RESET_DDR  |= (1 << HW_RESET_PIN); 
  // delay 200 ms
  _delay_ms(200);
  // Reset Low 
  HW_RESET_PORT &= ~(1 << HW_RESET_PIN);
  // delay 200 ms
  _delay_ms(200);
  // Reset High
  HW_RESET_PORT |=  (1 << HW_RESET_PIN);
}

/**
 * @description Initialise SPI communication
 *
 * @param void
 * @return void
 */
void SpiInit(void)
{
  // Output: SCK, MOSI, CS_LD, DC_LD 
  DDR  |= (1 << ST7735_SCK)   | 
          (1 << ST7735_MOSI)  |
          (1 << ST7735_CS_LD) |
          (1 << ST7735_DC_LD);
  // SPE  - SPI Enale
  // MSTR - Master device
  SPCR |= (1 << SPE) | 
          (1 << MSTR);
  // SPI2X - Prescaler fclk/2 = 8MHz
  SPSR |= (1 << SPI2X);
}

/**
 * @description Initialise St7735 communication
 *
 * @param void
 * @return void
 */
void St7735Init(void)
{
  // set DDR BackLigt
  DDR  |= (1 << ST7735_BL);
  // set high level on Backlight
  PORT |= (1 << ST7735_BL);
  // hardware reset
  HardwareReset();
  // init spi
  SpiInit();
  // load list of commands
  St7735Commands(INIT_ST7735B);
}

/**
 * @description Send commands
 *
 * @param const uint8_t *
 * @return void
 */
void St7735Commands(const uint8_t *commands)
{
  uint8_t milliseconds;
  uint8_t numOfCommands;
  uint8_t numOfArguments;

  // number of commands
  numOfCommands = pgm_read_byte(commands++);
  
  // loop through whole command list
  while (numOfCommands--) {
    // send command
    CommandSend(pgm_read_byte(commands++));
    // read number of arguments
    numOfArguments = pgm_read_byte(commands++);
    // check if delay set
    milliseconds = numOfArguments & DELAY;
    // remove delay flag
    numOfArguments &= ~DELAY;
    // loop through number of arguments
    while (numOfArguments--) {
      // send arguments
      Data8BitsSend(pgm_read_byte(commands++));
    }
    // check if delay set
    if (milliseconds) {
      // value in milliseconds
      milliseconds = pgm_read_byte(commands++);
      // delay
      DelayMs(milliseconds);
    }
  }
}

/**
 * @description Command send
 *
 * @param uint8_t command
 * @return void
 */
uint8_t CommandSend(uint8_t data)
{
  // chip enable - active low
  PORT &= ~(1 << ST7735_CS_LD);
  // command (active low)
  PORT &= ~(1 << ST7735_DC_LD);
  // transmitting data
  SPDR = data;
  // wait till data transmit
  while (!(SPSR & (1 << SPIF)));
  // chip disable - idle high
  PORT |= (1 << ST7735_CS_LD);
  // return received data
  return SPDR;
}

/**
 * @description 8 bits data send
 *
 * @param uint8_t 
 * @return void
 */
uint8_t Data8BitsSend(uint8_t data)
{
  // chip enable - active low
  PORT &= ~(1 << ST7735_CS_LD);
  // data (active high)
  PORT |= (1 << ST7735_DC_LD);
  // transmitting data
  SPDR = data;
  // wait till data transmit
  while (!(SPSR & (1 << SPIF)));
  // chip disable - idle high
  PORT |= (1 << ST7735_CS_LD);
  // return received data
  return SPDR;
}

/**
 * @description 16 bits data send
 *
 * @param uint16_t 
 * @return void
 */
uint8_t Data16BitsSend(uint16_t data)
{
  // chip enable - active low
  PORT &= ~(1 << ST7735_CS_LD);
  // data (active high)
  PORT |= (1 << ST7735_DC_LD);
  // transmitting data high byte
  SPDR = (uint8_t) (data >> 8);
  // wait till high byte transmit
  while (!(SPSR & (1 << SPIF)));
  // transmitting data low byte
  SPDR = (uint8_t) (data);
  // wait till low byte transmit
  while (!(SPSR & (1 << SPIF)));
  // chip disable - idle high
  PORT |= (1 << ST7735_CS_LD);
  // return received data
  return SPDR;
}

/**
 * Write color pixels
 *
 * @param uint16_t color
 * @param uint16_t counter
 * @return void
 */
void SendColor565(uint16_t color, uint16_t count)
{
  // access to RAM
  CommandSend(RAMWR);
  // counter
  while (count--) {
    // write color
    Data16BitsSend(color);
  }
}

/**
 * @description Set Partial Area / Window
 *
 * @param uint8_t start row
 * @param uint8_t end row
 * @return void
 */
uint8_t SetPartialArea(uint8_t sRow, uint8_t eRow)
{
  // check if coordinates is out of range
  if ((sRow > SIZE_Y) ||
      (eRow > SIZE_Y)) { 
    // out of range
    return 0;
  }  
  // column address set
  CommandSend(PTLAR);
  // start start Row
  Data8BitsSend(0x00);
  // start start Row
  Data8BitsSend(sRow);
  // row end Row
  Data8BitsSend(0x00);
  // end end Row
  Data8BitsSend(eRow);
  // partial area on
  CommandSend(PTLON);
  // success
  return 1;
}

/**
 * @description Set Window
 *
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @return void
 */
uint8_t SetWindow(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1)
{
  // check if coordinates is out of range
  if ((x0 > x1)     ||
      (x1 > SIZE_X) ||
      (y0 > y1)     ||
      (y1 > SIZE_Y)) { 
    // out of range
    return 0;
  }  
  // column address set
  CommandSend(CASET);
  // start x position
  Data8BitsSend(0x00);
  // start x position
  Data8BitsSend(x0);
  // start x position
  Data8BitsSend(0x00);
  // end x position
  Data8BitsSend(x1);
  // row address set
  CommandSend(RASET);
  // start x position
  Data8BitsSend(0x00);
  // start y position
  Data8BitsSend(y0);
  // start x position
  Data8BitsSend(0x00);
  // end y position
  Data8BitsSend(y1);
  // success
  return 1;
}

/**
 * @description Set text position x, y
 *
 * @param uint8_t x - position
 * @param uint8_t y - position
 * @return void
 */
char SetPosition(uint8_t x, uint8_t y)
{
  // check if coordinates is out of range
  if ((x > MAX_X - (CHARS_COLS_LEN + 1)) &&
      (y > MAX_Y - (CHARS_ROWS_LEN))) {
    // out of range
    return 0;
  }
  // check if x coordinates is out of range
  // and y is not out of range go to next line
  if ((x > MAX_X - (CHARS_COLS_LEN + 1)) &&
      (y < MAX_Y - (CHARS_ROWS_LEN))) {
    // change position y
    cacheMemIndexRow = y + CHARS_ROWS_LEN;
    // change position x
    cacheMemIndexCol = x;
  } else {
    // set position y 
    cacheMemIndexRow = y;
    // set position x
    cacheMemIndexCol = x;
  }
  // success
  return 1;
}

/**
 * @description Draw pixel
 *
 * @param uint8_t   x position / 0 <= cols <= MAX_X-1
 * @param uint8_t   y position / 0 <= rows <= MAX_Y-1
 * @param uint16_t  color
 * @return Null
 */
void DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
  // set window
  SetWindow(x, x, y, y);
  // draw pixel by 565 mode
  SendColor565(color, 1);
}

/**
 * @description     Draw character 2x larger
 *
 * @param char      character
 * @param uint16_t  color
 * @param Esizes    see enum sizes in st7735.h
 * @return void
 */
char DrawChar(char character, uint16_t color, ESizes size)
{
  // variables
  uint8_t letter, idxCol, idxRow;
  // check if character is out of range
  if ((character < 0x20) &&
      (character > 0x7f)) { 
    // out of range
    return 0;
  }
  // last column of character array - 5 columns 
  idxCol = CHARS_COLS_LEN;
  // last row of character array - 8 rows / bits
  idxRow = CHARS_ROWS_LEN;

  // ----------------------------------------
  // SIZE X1 - normal font: 1x high, 1x wide
  // ----------------------------------------
  if (size == X1) {  
    // loop through 5 bytes
    while (idxCol--) {
      // read from ROM memory 
      letter = pgm_read_byte(&CHARACTERS[character - 32][idxCol]);
      // loop through 8 bits
      while (idxRow--) {
        // check if bit set
        if ((letter & 0x80) == 0x80) {
          // draw pixel 
          DrawPixel(cacheMemIndexCol + idxCol, cacheMemIndexRow + idxRow, color);
        }
        // byte move to left / next bit
        letter = letter << 1;
      }
      // fill index row again
      idxRow = CHARS_ROWS_LEN;
    }
  // -----------------------------------------
  // SIZE X2 - bigger font 2x higher, 1x wide
  // -----------------------------------------
  } else if (size == X2) {
    // loop through 5 bytes
    while (idxCol--) {
      // read from ROM memory 
      letter = pgm_read_byte(&CHARACTERS[character - 32][idxCol]);
      // loop through 8 bits
      while (idxRow--) {
        // check if bit set
        if ((letter & 0x80) == 0x80) {
          // draw first up pixel; 
          // note: (idxRow << 1) - 2x multiplied 
          DrawPixel(cacheMemIndexCol + idxCol, cacheMemIndexRow + (idxRow << 1), color);
          // draw second down pixel
          DrawPixel(cacheMemIndexCol + idxCol, cacheMemIndexRow + (idxRow << 1) + 1, color);
        }
        // byte move to left / next bit
        letter = letter << 1;
      }
      // fill index row again
      idxRow = CHARS_ROWS_LEN;
    }
  // ------------------------------------------------
  // SIZE X3 - the biggest font: 2x higher, 2x wider
  // ------------------------------------------------
  } else if (size == X3) {
    // loop through 5 bytes
    while (idxCol--) {
      // read from ROM memory 
      letter = pgm_read_byte(&CHARACTERS[character - 32][idxCol]);
      // loop through 8 bits
      while (idxRow--) {
        // check if bit set
        if ((letter & 0x80) == 0x80) {
          // draw first left up pixel; 
          // note: (idxRow << 1) - 2x multiplied 
          DrawPixel(cacheMemIndexCol + (idxCol << 1), cacheMemIndexRow + (idxRow << 1), color);
          // draw second left down pixel
          DrawPixel(cacheMemIndexCol + (idxCol << 1), cacheMemIndexRow + (idxRow << 1) + 1, color);
          // draw third right up pixel
          DrawPixel(cacheMemIndexCol + (idxCol << 1) + 1, cacheMemIndexRow + (idxRow << 1), color);
          // draw fourth right down pixel
          DrawPixel(cacheMemIndexCol + (idxCol << 1) + 1, cacheMemIndexRow + (idxRow << 1) + 1, color);
        }
        // byte move to left / next bit
        letter = letter << 1;
      }
      // fill index row again
      idxRow = CHARS_ROWS_LEN;
    }
  }
  // return exit
  return 0;
}

/**
 * @description     Draw string
 *
 * @param char*     string 
 * @param uint16_t  color
 * @param Esizes    see enum sizes in st7735.h
 * @return void
 */
void DrawString(char *str, uint16_t color, ESizes size)
{
  // variables
  uint8_t i = 0;
  // loop through character of string
  while (str[i] != '\0') {
    //read characters and increment index
    DrawChar(str[i++], color, size);
    // update position
    SetPosition(cacheMemIndexCol + (CHARS_COLS_LEN + 1) + (size >> 1), cacheMemIndexRow);
  }
}

/**
 * @description Draw line by Bresenham algoritm
 * @surce       https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *  
 * @param uint8_t   x start position / 0 <= cols <= MAX_X-1
 * @param uint8_t   x end position   / 0 <= cols <= MAX_X-1
 * @param uint8_t   y start position / 0 <= rows <= MAX_Y-1 
 * @param uint8_t   y end position   / 0 <= rows <= MAX_Y-1
 * @param uint16_t  color
 * @return void
 */
char DrawLine(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color)
{
  // determinant
  int16_t D;
  // deltas
  int16_t delta_x, delta_y;
  // steps
  int16_t trace_x = 1, trace_y = 1;

  // delta x
  delta_x = x2 - x1;
  // delta y
  delta_y = y2 - y1;

  // check if x2 > x1
  if (delta_x < 0) {
    // negate delta x
    delta_x = -delta_x;
    // negate step x
    trace_x = -trace_x;
  }

  // check if y2 > y1
  if (delta_y < 0) {
    // negate detla y
    delta_y = -delta_y;
    // negate step y
    trace_y = -trace_y;
  }

  // Bresenham condition for m < 1 (dy < dx)
  if (delta_y < delta_x) {
    // calculate determinant
    D = (delta_y << 1) - delta_x;
    // draw first pixel
    DrawPixel(x1, y1, color);
    // check if x1 equal x2
    while (x1 != x2) {
      // update x1
      x1 += trace_x;
      // check if determinant is positive
      if (D >= 0) {
        // update y1
        y1 += trace_y;
        // update determinant
        D -= (delta_x << 1);    
      }
      // update deteminant
      D += (delta_y << 1);
      // draw next pixel
      DrawPixel(x1, y1, color);
    }
  // for m > 1 (dy > dx)    
  } else {
    // calculate determinant
    D = delta_y - (delta_x << 1);
    // draw first pixel
    DrawPixel(x1, y1, color);
    // check if y2 equal y1
    while (y1 != y2) {
      // update y1
      y1 += trace_y;
      // check if determinant is positive
      if (D <= 0) {
        // update y1
        x1 += trace_x;
        // update determinant
        D += (delta_y << 1);    
      }
      // update deteminant
      D -= (delta_x << 1);
      // draw next pixel
      DrawPixel(x1, y1, color);
    }
  }
  // success return
  return 1;
}

/**
 * @description Fast draw line horizontal
 *
 * @param uint8_t xs - start position
 * @param uint8_t xe - end position
 * @param uint8_t y - position
 * @param uint16_t  color
 * @return void
 */
void DrawLineHorizontal(uint8_t xs, uint8_t xe, uint8_t y, uint16_t color)
{
  uint8_t temp;
  // check if start is > as end  
  if (xs > xe) {
    // temporary safe
    temp = xs;
    // start change for end
    xe = xs;
    // end change for start
    xs = temp;
  }
  // set window
  SetWindow(xs, xe, y, y);
  // draw pixel by 565 mode
  SendColor565(color, xe - xs);
}

/**
 * @description Fast draw line vertical
 *
 * @param uint8_t x - position
 * @param uint8_t ys - start position
 * @param uint8_t ye - end position
 * @param uint16_t  color
 * @return void
 */
void DrawLineVertical(uint8_t x, uint8_t ys, uint8_t ye, uint16_t color)
{
  uint8_t temp;
  // check if start is > as end
  if (ys > ye) {
    // temporary safe
    temp = ys;
    // start change for end
    ye = ys;
    // end change for start
    ys = temp;
  }
  // set window
  SetWindow(x, x, ys, ye);
  // draw pixel by 565 mode
  SendColor565(color, ye - ys);
}

/**
 * @description Clear screen
 *
 * @param uint16_t color
 * @return void
 */
void ClearScreen(uint16_t color)
{
  // set whole window
  SetWindow(0, SIZE_X, 0, SIZE_Y);
  // draw individual pixels 
  // CACHE_SIZE_MEM = SIZE_X * SIZE_Y
  SendColor565(color, CACHE_SIZE_MEM);
}

/**
 * @description Update screen
 *
 * @param void
 * @return void
 */
void UpdateScreen(void)
{
  // display on
  CommandSend(DISPON);
}

/**
 * @description Delay
 *
 * @param uint8_t time in milliseconds
 * @return void
 */
void DelayMs(uint8_t time)
{
  // loop through real time
  while (time--) {
    // 1 s delay
    _delay_ms(1);
  }
}

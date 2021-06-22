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
 * @file        st7735.c
 * @tested      AVR Atmega16
 *
 * @depend      font.h
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "font.h"
#include "st7735.h"

/** @array Init command */
const uint8_t INIT_ST7735B[] PROGMEM = {
  // number of initializers
  5,
  // ---------------------------------------
  // Software reset - no arguments,  delay
  0, 150, SWRESET,
  // Out of sleep mode, no arguments, delay
  0, 200, SLPOUT,  
  // Set color mode, 1 argument delay
  1,  10, COLMOD, 0x05,
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
  1,   0, MADCTL, 0xA0,
  // Main screen turn on
  0, 200, DISPON 
  // ---------------------------------------
};

/** @var array Chache memory char index row */
unsigned short int cacheMemIndexRow = 0;
/** @var array Chache memory char index column */
unsigned short int cacheMemIndexCol = 0;

/**
 * @desc    Hardware Reset
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_Reset (void)
{
  // Actiavte pull-up resistor logical high on pin RST
  SET_BIT (HW_RESET_PORT, HW_RESET_PIN);
  // DDR as output
  SET_BIT (HW_RESET_DDR, HW_RESET_PIN);
  // delay 200 ms
  _delay_ms(200);
  // Reset Low 
  CLR_BIT (HW_RESET_PORT, HW_RESET_PIN);
  // delay 200 ms
  _delay_ms(200);
  // Reset High
  SET_BIT (HW_RESET_PORT, HW_RESET_PIN);
}

/**
 * @desc    Init SPI
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_SPI_Init (void)
{
  // Output: SCK, MOSI, DC_LD 
  SET_BIT (DDR, ST7735_SCK);
  SET_BIT (DDR, ST7735_MOSI);
  SET_BIT (DDR, ST7735_DC_LD);
  // SPE  - SPI Enale
  // MSTR - Master device
  SET_BIT (SPCR, SPE);
  SET_BIT (SPCR, MSTR);
  // SPI2X - Prescaler fclk/2 = 8MHz
  SET_BIT (SPSR, SPI2X);
}

/**
 * @desc    Init CS
 *
 * @param   struct st7735 *
 *
 * @return  void
 */
void ST7735_CS_Init (struct st7735 * lcd)
{
  // set CS as output
  SET_BIT (lcd->cs->ddr, lcd->cs->pin);
  // set CS high level / inactive /
  SET_BIT (lcd->cs->port, lcd->cs->pin);
}

/**
 * @desc    Init BackLight
 *
 * @param   struct st7735 *
 *
 * @return  void
 */
void ST7735_BL_Init (struct st7735 * lcd)
{
  // set DDR BackLigt
  SET_BIT (lcd->bl->ddr, lcd->bl->pin);
  // set BL high level / Backlight ON /
  SET_BIT (lcd->bl->port, lcd->bl->pin);
}

/**
 * @desc    Init st7735 driver
 *
 * @param   struct st7735 *
 *
 * @return  void
 */
void ST7735_Init (struct st7735 * lcd)
{
  // init SPI
  ST7735_SPI_Init ();
  // init CS
  ST7735_CS_Init (lcd);
  // hardware reset
  ST7735_Reset ();
  // init BL
  ST7735_BL_Init (lcd);
  // load list of commands
  ST7735_Commands (lcd, INIT_ST7735B);
}

/**
 * @desc    Send list commands
 *
 * @param   struct st7735 * lcd
 * @param   const uint8_t*
 *
 * @return  void
 */
void ST7735_Commands (struct st7735 * lcd, const uint8_t *initializers)
{
  uint8_t args;
  uint8_t cmnd;
  uint8_t time;
  uint8_t loop = pgm_read_byte (initializers++);

  // loop through whole initializer list
  while (loop--) {

    // 1st arg - number of command arguments
    args = pgm_read_byte (initializers++);
    // 2nd arg - delay time
    time = pgm_read_byte (initializers++);
    // 3th arg - command
    cmnd = pgm_read_byte (initializers++);

    // send command
    ST7735_CommandSend (lcd, cmnd);
    // send arguments
    while (args--) {
      // send argument
      ST7735_Data8BitsSend (lcd, pgm_read_byte (initializers++));
    }

    // delay
    ST7735_DelayMs (time);
  }
}

/**
 * @desc    Command send
 *
 * @param   struct st7735 *
 * @param   uint8_t
 *
 * @return void
 */
uint8_t ST7735_CommandSend (struct st7735 * lcd, uint8_t data)
{
  // chip enable - active low
  CLR_BIT (lcd->cs->port, lcd->cs->pin);
  // command (active low)
  CLR_BIT (PORT, ST7735_DC_LD);
  // transmitting data
  SPDR = data;
  // wait till data transmit
  while ( BIT_IS_SET (SPSR, SPIF) );
  // chip disable - idle high
  SET_BIT (lcd->cs->port, lcd->cs->pin);
  // return received data
  return SPDR;
}

/**
 * @desc    8bits data send
 *
 * @param   struct st7735 *
 * @param   uint8_t
 *
 * @return  void
 */
uint8_t ST7735_Data8BitsSend (struct st7735 * lcd, uint8_t data)
{
  // chip enable - active low
  CLR_BIT (lcd->cs->port, lcd->cs->pin);
  // data (active high)
  SET_BIT (PORT, ST7735_DC_LD);
  // transmitting data
  SPDR = data;
  // wait till data transmit
  while (!(SPSR & (1 << SPIF)));
  // chip disable - idle high
  SET_BIT (lcd->cs->port, lcd->cs->pin);
  // return received data
  return SPDR;
}

/**
 * @desc    16bits data send
 *
 * @param   struct st7735 * lcd
 * @param   uint16_t
 *
 * @return  void
 */
uint8_t ST7735_Data16BitsSend (struct st7735 * lcd, uint16_t data)
{
  // chip enable - active low
  CLR_BIT (lcd->cs->port, lcd->cs->pin);
  // data (active high)
  SET_BIT (PORT, ST7735_DC_LD);
  // transmitting data high byte
  SPDR = (uint8_t) (data >> 8);
  // wait till high byte transmit
  while (!(SPSR & (1 << SPIF)));
  // transmitting data low byte
  SPDR = (uint8_t) (data);
  // wait till low byte transmit
  while (!(SPSR & (1 << SPIF)));
  // chip disable - idle high
  SET_BIT (lcd->cs->port, lcd->cs->pin);
  // return received data
  return SPDR;
}

/**
 * @desc    Set window
 *
 * @param   struct st7735 * lcd
 * @param   uint8_t x - start position
 * @param   uint8_t x - end position
 * @param   uint8_t y - start position
 * @param   uint8_t y - end position
 *
 * @return  uint8_t
 */
uint8_t ST7735_SetWindow (struct st7735 * lcd, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1)
{
  // check if coordinates is out of range
  if ((x0 > x1)     ||
      (x1 > SIZE_X) ||
      (y0 > y1)     ||
      (y1 > SIZE_Y)) { 
    // out of range
    return ST7735_ERROR;
  }  
  // column address set
  ST7735_CommandSend (lcd, CASET);
  // send start x position
  ST7735_Data16BitsSend (lcd, 0x0000 | x0);
  // send end x position
  ST7735_Data16BitsSend (lcd, 0x0000 | x1);

  // row address set
  ST7735_CommandSend (lcd, RASET);
  // send start y position
  ST7735_Data16BitsSend (lcd, 0x0000 | y0);
  // send end y position
  ST7735_Data16BitsSend (lcd, 0x0000 | y1);

  // success
  return ST7735_SUCCESS;
}

/**
 * @desc    Write color pixels
 *
 * @param   struct st7735 * lcd
 * @param   uint16_t color
 * @param   uint16_t counter
 *
 * @return  void
 */
void ST7735_SendColor565 (struct st7735 * lcd, uint16_t color, uint16_t count)
{
  // access to RAM
  ST7735_CommandSend (lcd, RAMWR);
  // counter
  while (count--) {
    // write color
    ST7735_Data16BitsSend (lcd, color);
  }
}

/**
 * @desc    Draw pixel
 *
 * @param   struct st7735 * lcd
 * @param   uint8_t   x position / 0 <= cols <= MAX_X-1
 * @param   uint8_t   y position / 0 <= rows <= MAX_Y-1
 * @param   uint16_t  color
 *
 * @return  void
 */
void ST7735_DrawPixel (struct st7735 * lcd, uint8_t x, uint8_t y, uint16_t color)
{
  // set window
  ST7735_SetWindow (lcd, x, x, y, y);
  // draw pixel by 565 mode
  ST7735_SendColor565 (lcd, color, 1);
}

/**
 * @desc    Clear screen
 *
 * @param   struct st7735 * lcd
 * @param   uint16_t color
 *
 * @return  void
 */
void ST7735_ClearScreen (struct st7735 * lcd, uint16_t color)
{
  // set whole window
  ST7735_SetWindow (lcd, 0, SIZE_X, 0, SIZE_Y);
  // draw individual pixels
  ST7735_SendColor565 (lcd, color, CACHE_SIZE_MEM);
}

/**
 * @desc    Update screen
 *
 * @param   struct st7735 * lcd
 *
 * @return  void
 */
void ST7735_UpdateScreen (struct st7735 * lcd)
{
  // display on
  ST7735_CommandSend (lcd, DISPON);
}

/**
 * @desc    Delay
 *
 * @param   uint8_t time in milliseconds
 *
 * @return  void
 */
void ST7735_DelayMs (uint8_t time)
{
  // loop through real time
  while (time--) {
    // 1 s delay
    _delay_ms(1);
  }
}

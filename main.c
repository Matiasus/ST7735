/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        Main example st7735 LCD driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       13.10.2020
 * @update      21.06.2021
 * @file        main.c
 * @version     2.0
 * @tested      AVR Atmega328
 *
 * @depend      st7735.h
 * --------------------------------------------------------------------------------------------+
 * @descr       Version 1.0 -> applicable for 1 display
 *              Version 2.0 -> applicable for more than 1 display
 */
#include "lib/st7735.h"

/**
 * @desc    Main function
 *
 * @param   Void
 *
 * @return  Void
 */
int main (void)
{
  // start
  uint8_t start = 30;
  // end
  uint8_t end = MAX_X - start;

  // LCD 1 - init struct
  // ----------------------------------------------------------
  // Chip Select
  struct signal cs = { .ddr = &DDRB, .port = &PORTB, .pin = 2 };
  // Back Light
  struct signal bl = { .ddr = &DDRB, .port = &PORTB, .pin = 1 };
  // Data / Command
  struct signal dc = { .ddr = &DDRB, .port = &PORTB, .pin = 0 };
  // Reset
  struct signal rs = { .ddr = &DDRD, .port = &PORTD, .pin = 7 };
  // LCD struct
  struct st7735 lcd1 = { .cs = &cs, .bl = &bl, .dc = &dc, .rs = &rs };

  // LCD 2 - init struct
  // ----------------------------------------------------------
  // Chip Select
  struct signal cs2 = { .ddr = &DDRD, .port = &PORTD, .pin = 0 };
  // Back Light
  struct signal bl2 = { .ddr = &DDRD, .port = &PORTD, .pin = 1 };
  // Data / Command
  struct signal dc2 = { .ddr = &DDRB, .port = &PORTB, .pin = 0 };
  // Reset
  struct signal rs2 = { .ddr = &DDRD, .port = &PORTD, .pin = 3 };
  // LCD struct
  struct st7735 lcd2 = { .cs = &cs2, .bl = &bl2, .dc = &dc2, .rs = &rs2 };

  // LCD 1
  // ----------------------------------------------------------
  // init lcd 1
  ST7735_Init (&lcd1);
  // clear screen
  ST7735_ClearScreen (&lcd1, BLACK);
  // set position X, Y
  ST7735_SetPosition (start + 5, 10);  
  // draw string
  ST7735_DrawString (&lcd1, "Loading DATA ...", WHITE, X2);

  // draw Loading
  for (uint8_t i = start; i < end; i++) {
    // draw rectangle
    ST7735_DrawRectangle (&lcd1, start, i, 30, 40, RED);
  }

  // LCD 2
  // ----------------------------------------------------------
  // init lcd 2
  ST7735_Init (&lcd2);
  // clear screen
  ST7735_ClearScreen (&lcd2, BLACK);
  // set position X, Y
  ST7735_SetPosition (17, 10);  
  // draw string
  ST7735_DrawString (&lcd2, "ST7735 LCD 2", WHITE, X3);
  // draw fast horizontal line
  ST7735_DrawLineHorizontal (&lcd2, 5, MAX_X - 5, 27, WHITE);

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}

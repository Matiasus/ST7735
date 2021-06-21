/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        Main example st7735 LCD driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       13.10.2020
 * @update      21.06.2021
 * @file        main.c
 * @tested      AVR Atmega16
 *
 * @depend      st7735.h
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
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

  // LCD 1 - init
  // ----------------------------------------------------------
  // Chip Select
  struct signal cs = { .ddr = DDRB, .port = PORTB, .pin = LCD1_CS };
  // Back Light
  struct signal bl = { .ddr = DDRB, .port = PORTB, .pin = LCD1_BL };
  // LCD struct
  struct st7735 lcd_01 = { &cs, &bl };

  // init lcd 1
  ST7735_Init (&lcd_01);
   
/* 
  // init
  lcd.Init (lcd.cs);

  // clear screen
  lcd.ClearScreen (RED, lcd.cs);
  // update screen
  lcd.UpdateScreen (lcd.cs);
  // init lcd
  ST7735_Init();

  // clear screen
  ST7735_ClearScreen(BLACK);
  // set position
  ST7735_SetPosition(2, 2);
  // Draw string
  ST7735_DrawString("Dated 08/14/06. One amplifier that the 2N5416 PNP transistors were NPN", WHITE, X1);
  // update screen
  ST7735_UpdateScreen();

*/

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}





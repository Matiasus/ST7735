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
  // LCD 1 - init structs
  // ----------------------------------------------------------
  // Chip Select
  struct signal cs = { .ddr = DDRB, .port = PORTB, .pin = 3 };
  // Back Light
  struct signal bl = { .ddr = DDRB, .port = PORTB, .pin = 2 };
  // LCD struct
  struct st7735 lcd_01 = { &cs, &bl };

  // init lcd 1
  ST7735_Init (&lcd_01);
  // clear screen
  ST7735_ClearScreen (&lcd_01, BLACK);

  // draw character
  ST7735_DrawChar (&lcd_01, 'B', RED, X2);

  // set position X, Y
  ST7735_SetPosition (1, 2);  
  // draw string
  ST7735_DrawString (&lcd_01, "Dated 08/14/06. One amplifier that the 2N5416 PNP transistors that he purchased were NPN. I have tested them and verified this. Two of the transistors are shown in the linked photo. They are identified by the letters ON surrounded by a circle, signifying they are products of ON Semiconductor.You can check for a transistor type as follows:One amplifier that the 2N5416 PNP transistors that he purchased were NPN. One amplifier that the 2N5416 PNP transistors that he purchased were NPN", WHITE, X2);

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}





/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        Main example st7735 LCD driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       13.10.2020
 * @file        main.c
 * @tested      AVR Atmega16
 *
 * @depend      st7735.h
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 */
#include <stdio.h>
#include "lib/st7735.h"

/**
 * @desc    Main
 *
 * @param   Void
 *
 * @return  Void
 */
int main(void)
{
  // init lcd
  St7735Init();
  // clear screen
  ClearScreen(0x0000);
  // set position
  SetPosition(2, 2);
  // Draw string
  DrawString("Dated 08/14/06. One amplifier that the 2N5416 PNP transistors were NPN", 0xffff, X1);
  // update screen
  UpdateScreen();
  
  // return
  return 0;
}





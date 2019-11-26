/** 
 * Example of TFT controller st7735 / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "lib/st7735.h"

/**
 * @description Main function
 *
 * @param Void
 * @return Null
 */
int main(void)
{ 
  // init st7735 driver
  St7735Init();
  // clear screen
  ClearScreen(0xffff);

  // draw line
  DrawLine(23, 137, 20, 20, 0x04af);
  // draw line
  DrawLine(23, 137, 21, 21, 0x04af);
  // set text position
  SetPosition(41, 10);
  // draw text
  DrawString("ST7735 DRIVER", 0x007b, X1);
  // set text position
  SetPosition(53, 26);
  // draw text
  DrawString("ATMEGA 16", 0x0000);
  // display on
  UpdateScreen();

  // return Null
  return 0;
}


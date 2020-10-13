/** 
 * Example of TFT controller st7735 / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       16.12.2017
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
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

  DrawString("Dated 08/14/06. One amplifier that the 2N5416 PNP transistors that he purchased were NPN. I have tested them and verified this. Two of the transistors are shown in the linked photo. They are identified by the letters ON surrounded by a circle, signifying they are products of ON Semiconductor.You can check for a transistor type as follows:One amplifier that the 2N5416 PNP transistors that he purchased were NPN. One amplifier that the 2N5416 PNP transistors that he purchased were NPN", 0xffff, X1);

  // zobrazenie */
  UpdateScreen();
  // ukoncenie */
  return 0;
}





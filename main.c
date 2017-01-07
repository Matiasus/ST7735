/*** 
 * Example drive LCD controller st7735 / 1.8 tft display /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author  Marian Hrinko
 * @datum   24.12.2016
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "lib/st7735.h"

void flash(uint16_t);
void TFTClear(void);


/**
 * @description Main function
 *
 * @param Void
 * @return Null
 */
int main(void)
{ 
  DDRA |= (1 << PA0);

  // init st7735 driver
  St7735Init();
  flash(1000);

  // clear screen
  ClearScreen(0xffff);

  // draw line
  DrawLine(10, 117, 20, 20, 0x04af);
  DrawLine(10, 117, 21, 21, 0x04af);
  DrawLine(10, 117, 22, 22, 0x04af);

  SetPosition(25, 10);
  DrawString("ST7735 DRIVER", 0x007b);

  SetPosition(37, 26);
  DrawString("ATMEGA 16", 0x0000);
  
  // return Null
  return 0;
}

/**
 * @description Flash led
 *
 * @param uint8_t
 * @return void
 */
void flash(uint16_t time)
{
  DelayMs(time);
  PORTA &= ~(1 << PA0);
  DelayMs(time);
  PORTA |=  (1 << PA0);
}

# LCD 162x132

## ST7735 Description
Detailed information are described in [Datasheet ST7735](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf).

## Library
C library is aimed for driving [ST7735 1.8 TFT LCD display](img/st7735.jpg) through SPI's Atmega16 / Atmega8.

### Tested
Library was tested and proved on a **_ST7735 1.8″ TFT Dispay_** with **_Atmega16_**.

## Usage
Prior defined for MCU Atmega16 / Atmega8. Need to be carefull with SPI ports definition.
- [Atmega16](https://www.gme.cz/data/attachments/dsh.958-112.1.pdf)
  - SS - PB4
  - MOSI - PB5
  - MISO - PB6
  - SCK - PB7
- [Atmega8](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_datasheet.pdf)
  - SS - PB2
  - MOSI - PB3
  - MISO - PB4
  - SCK - PB5
  
## Demonstration
<img src="img/st7735.jpg" />

## Links
- [Datasheet ST7735](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf)
- [Adafuit TFT](https://github.com/adafruit/Adafruit-ST7735-Library)
- [AVR TFT](http://w8bh.net/avr/AvrTFT.pdf)

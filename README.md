# C Library for ST7735 1.8 TFT LCD display

## ST7735 Description
Detailed information are described in [Datasheet ST7735](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf).

## Library
C library is aimed for driving [ST7735 1.8 TFT LCD display](#demonstration) 162x132 through SPI's Atmega16 / Atmega8.

## Hardware connection
| PORT LCD | PORT ATMEGA16A | Description |
| :--- | :--- |  :--- |
| RST | PORTB 0 | Chip Reset Signal |
| CS | PORTB 4 | SPI Chip Select |
| D/C | PORTB 1 | SPI data / command; 1=Data, 0=Command |
| DIN | PORTB 5 | SPI Serial data in / MOSI |
| CLK | PORTB 7 | SPI Serial clock signal |
| UCC | +5V | Supply Voltage +5V |
| BL | PORTB 2 | Backlight control (with resistor 40-150 Ohm) |
| GND | GND | Ground |

### Usage
Prior defined for MCU Atmega16 / Atmega8. Need to be carefull with SPI ports definition.

| PORT  | [Atmega16](http://ww1.microchip.com/downloads/en/devicedoc/doc2466.pdf) | [Atmega8](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_datasheet.pdf) |
| :---: | :---: | :---: |
| SS | PB4 | PB2 |
| MOSI | PB5 | PB3 |
| MISO | PB6 | PB4 |
| SCK | PB7 | PB5 |

### Tested
Library was tested and proved on a **_ST7735 1.8″ TFT Dispay_** with **_Atmega16_**.
  
## Demonstration
<img src="img/st7735_v2.jpg" />

## Links
- [Datasheet ST7735](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf)
- [Adafuit TFT](https://github.com/adafruit/Adafruit-ST7735-Library)
- [AVR TFT](http://w8bh.net/avr/AvrTFT.pdf)

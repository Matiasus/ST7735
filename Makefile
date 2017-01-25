#
# @description makefile for compiling, linking and flashing code to microcontroller
# @inspiration https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
#              https://wiki.hacdc.org/index.php/AVR_Makefile
#
# @author      Marian Hrinko
# @datum       21.01.2017
#

# BASIC CONFIGURATION, SETTINGS
# ------------------------------------------------------------------

#
# Final file
TARGET        = main
#
# Library directory
LIBDIR        = lib
#
# Type of microcontroller
MMCU          = atmega16
#
# Frequency
FCPU          = 16000000
#
# Optimization
OPTIMIZE      = Os
#
# Type of compiler
CC            = avr-gcc
#
# Compiler flags
CFLAGS        = -g -Wall -mmcu=$(MMCU) -DF_CPU=$(FCPU) -$(OPTIMIZE)
#
# Includes
INCLUDES      = -I.
#
# Libraries
LIBS          = -L$(LIBDIR)
#
# Object copy
OBJCOPY       = avr-objcopy
#
# Objcopy, create hex file flags 
# -R .eeprom -O ihex or -j .data -j .text -O ihex
OBJFLAGS      = -j .data -j .text -O ihex
#
# Size of file
AVRSIZE       = avr-size
#
# Size flags
SFLAGS        = --mcu=$(MMCU) --format=avr
#
# Target and dependencies .c
SOURCES      := $(wildcard *.c $(LIBDIR)/*.c)
#
# Target and dependencies .o
# @notes				
# Suffix Replacement within a macro: $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
# For example $(SOURCES:.c=.o)
OBJECTS	      = $(SOURCES:.c=.o)

# AVRDUDE CONFIGURATION, SETTINGS
# -------------------------------------------------------------------

#
# AVRDUDE COMMAND
AVRDUDE       = avrdude
#
# AVRDUDE PORT
AVRDUDE_PORT  = /dev/ttyUSB0
#
# AVRDUDE PROGRAMMER
AVRDUDE_PROG  = usbasp
#
# AVRDUDE BAUD RATE
AVRDUDE_BAUD  = 19200
#
# AVRDUDE FLAGS
AVRDUDE_FLAGS = -p $(MMCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROG) -b $(AVRDUDE_BAUD) -u -U

# 
# Create file to programmer
main: $(TARGET).hex
	
# 
# Create hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) $(OBJFLAGS) $(TARGET).elf $(TARGET).hex
	$(AVRSIZE) $(TARGET).elf

# 
# Create .elf file
$(TARGET).elf:$(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET).elf

#
# Create object files
%.o: %.c
	 $(CC) $(CFLAGS) -c $< -o $@

# 
# Program avr - send file to programmer
program: 
	$(AVRDUDE) $(AVRDUDE_FLAGS) flash:w:$(TARGET).hex:i

#
# Clean
clean: 
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).map

#
# Cleanall
cleanall: 
	rm -f $(OBJECTS) $(TARGET).hex $(TARGET).elf $(TARGET).map



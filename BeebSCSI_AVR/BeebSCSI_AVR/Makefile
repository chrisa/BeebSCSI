TARGET  = BEEBSCSI

DEVICE  = at90usb1287
DEFINES = -DF_CPU=16000000UL -DNDEBUG

CC      = avr-gcc
SIZE    = avr-size
OBJCOPY = avr-objcopy
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE)
NM      = avr-nm

CFLAGS  = -mmcu=$(DEVICE) -O1 -Wall -std=c99 -I . -I fatfs $(DEFINES)

SOURCES = $(wildcard fatfs/options/*.c fatfs/*.c *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: Makefile.deps $(TARGET).BIN

-include Makefile.deps

$(TARGET).elf: $(OBJECTS)
	$(CC) -mmcu=$(DEVICE) -o $@ $^
	$(SIZE) --mcu=$(DEVICE) -C $(TARGET).elf

$(TARGET).BIN: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

Makefile.deps: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > Makefile.deps

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).BIN Makefile.deps

sizes: $(TARGET).elf
	$(NM) -Crtd --size-sort BEEBSCSI.elf | grep -i ' [dbv] '

TARGET  = BEEBSCSI

DEVICE  = at90usb1287
DEFINES = -DF_CPU=16000000UL
DEBUG   = NDEBUG

CC      = avr-gcc -mmcu=$(DEVICE)
SIZE    = avr-size
OBJCOPY = avr-objcopy
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE)
NM      = avr-nm

CFLAGS  = -O1 -Wall -std=c99 -I . -I fatfs $(DEFINES) -D$(DEBUG)

ifeq ($(CC),avr-gcc -mmcu=$(DEVICE))
SOURCES = $(wildcard fatfs/options/*.c fatfs/*.c *.c)
else
SOURCES = $(wildcard fatfs/ff.c host/*.c filesystem.c fcode.c scsi.c)
endif

OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: Makefile.deps $(TARGET).BIN

test: prove Makefile.deps $(TARGET).elf BeebSCSI_FAT.dmg
	./tap/prove ./$(TARGET).elf 2> /dev/null

BeebSCSI_FAT.dmg:
	rm -f $@
	mkfs.fat -C $@ 550000

-include Makefile.deps

$(TARGET).elf: $(OBJECTS)
	$(CC) -o $@ $^

$(TARGET).BIN: $(TARGET).elf
	$(SIZE) --mcu=$(DEVICE) -C $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

Makefile.deps: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > Makefile.deps

clean:
	$(MAKE) -C tap clean
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).BIN Makefile.deps

sizes: $(TARGET).elf
	$(NM) -Crtd --size-sort BEEBSCSI.elf | grep -i ' [dbv] '

prove:
	$(MAKE) -C tap

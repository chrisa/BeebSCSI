#ifdef __AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "fatfs/mmc_avr.h"
#else
#include <inttypes.h>
#include <stdlib.h>
#define DMA_SIZE 256
#define PSTR
void sei();
void cli();
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Maximum LUNs supported
#define MAX_LUNS 4

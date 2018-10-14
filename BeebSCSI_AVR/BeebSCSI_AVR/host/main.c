#include "beebscsi.h"
#include "uart.h"
#include "debug.h"
#include "filesystem.h"
#include "hostadapter.h"
#include "statusled.h"
#include "scsi.h"
#include "usb.h"

uint8_t command[10] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
extern uint8_t *loadedCommand;

extern uint8_t hostBuffers[256][DMA_SIZE];
extern uint8_t (*hostBuffer)[256];

void runCommand(uint8_t *command)
{
        hostBuffer = &hostBuffers[0];
        loadedCommand = command;
        uint8_t opCode = scsiEmulationCommand();
        switch(opCode)
        {
        case SCSI_READ6:
                scsiCommandRead6();
                break;
        case SCSI_WRITE6:
                scsiCommandWrite6();
                break;
        default:
                fprintf(stderr, "TEST: unhandled opCode: 0x%hhu\n", opCode);
                break;
        }
}

void setLUNBlockAddress(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        command[1] = ((targetLUN << 5) & 0xe0) | ((logicalBlockAddress >> 16) & 0x1f);
        command[2] = (uint8_t) (logicalBlockAddress >> 8);
        command[3] = (uint8_t) logicalBlockAddress;
        command[4] = (uint8_t) numberOfBlocks;
}

uint8_t *readCommand(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        command[0] = 0x08;
        setLUNBlockAddress(targetLUN, logicalBlockAddress, numberOfBlocks);
        return command;
}

uint8_t *writeCommand(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        command[0] = 0x0A;
        setLUNBlockAddress(targetLUN, logicalBlockAddress, numberOfBlocks);
        return command;
}

void fillBuffer(uint8_t *buffer, uint32_t logicalBlockAddress)
{
        srand(logicalBlockAddress);
        for (int i = 0; i < DMA_SIZE; i++)
                buffer[i] = (uint8_t) rand();
}

int writeAndRead(uint32_t logicalBlockAddress)
{
        uint8_t written[16][DMA_SIZE];

        for (int i = 0; i < 16; i++)
        {
                fillBuffer(hostBuffers[i], logicalBlockAddress + i);
                memcpy(written[i], hostBuffers[i], DMA_SIZE);
        }
        runCommand(writeCommand(0, logicalBlockAddress, 16));
        runCommand(readCommand(0, logicalBlockAddress, 16));

        int total = 0;
        for (int i = 0; i < 16; i++)
        {
                total += memcmp(written[i], hostBuffers[i], DMA_SIZE);
        }
        return total;
}

int main(void)
{
	// Initialise the SD Card and FAT file system functions
	filesystemInitialise();

	// Initialise the SCSI emulation
	scsiInitialise();

#define NTESTS 16

        printf("1..%d\n", NTESTS);
        for (int i = 0; i < NTESTS; i++)
        {
                if ((writeAndRead(i)) != 0)
                        printf("not ");
                printf("ok %d - read and write 16 sectors from sector %d\n", i, i);
        }
}

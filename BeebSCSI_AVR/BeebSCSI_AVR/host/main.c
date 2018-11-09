#include "beebscsi.h"
#include "uart.h"
#include "debug.h"
#include "filesystem.h"
#include "hostadapter.h"
#include "statusled.h"
#include "scsi.h"
#include "usb.h"

#define NTESTS 256
#define NBLOCKS 256

uint8_t format[6] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t readWrite[10] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t modesel[28] = { 0x15, 0x00, 0x00, 0x00, 0x16, 0x00,
                        0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
                        0x00, 0x01, 0x06, 0x43, 0x0f, 0x00, 0x80, 0x00, 0x80, 0x00, 0x01 };

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
        case SCSI_FORMAT:
                scsiCommandFormat();
                break;
        case SCSI_MODESELECT:
                scsiCommandModeSelect();
                break;
        default:
                fprintf(stderr, "TEST: unhandled opCode: 0x%hhu\n", opCode);
                break;
        }
}

uint8_t *formatCommand(uint8_t targetLUN)
{
        format[1] = (targetLUN << 5) & 0xe0;
        return format;
}

uint8_t *modeSelectCommand(uint8_t targetLUN)
{
        modesel[1] = (targetLUN << 5) & 0xe0;
        return modesel;
}

void setLUNBlockAddress(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        readWrite[1] = ((targetLUN << 5) & 0xe0) | ((logicalBlockAddress >> 16) & 0x1f);
        readWrite[2] = (uint8_t) (logicalBlockAddress >> 8);
        readWrite[3] = (uint8_t) logicalBlockAddress;
        readWrite[4] = (uint8_t) (numberOfBlocks == 256 ? 0 : numberOfBlocks);
}

uint8_t *readCommand(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        readWrite[0] = 0x08;
        setLUNBlockAddress(targetLUN, logicalBlockAddress, numberOfBlocks);
        return readWrite;
}

uint8_t *writeCommand(uint8_t targetLUN, uint32_t logicalBlockAddress, uint32_t numberOfBlocks)
{
        readWrite[0] = 0x0A;
        setLUNBlockAddress(targetLUN, logicalBlockAddress, numberOfBlocks);
        return readWrite;
}

void fillBuffer(uint8_t *buffer, uint32_t logicalBlockAddress)
{
        srand(logicalBlockAddress);
        for (int i = 0; i < DMA_SIZE; i++)
                buffer[i] = (uint8_t) rand();
}

int writeAndRead(uint32_t logicalBlockAddress)
{
        uint8_t written[NBLOCKS][DMA_SIZE];

        for (int i = 0; i < NBLOCKS; i++)
        {
                fillBuffer(hostBuffers[i], logicalBlockAddress + i);
                memcpy(written[i], hostBuffers[i], DMA_SIZE);
        }
        runCommand(writeCommand(0, logicalBlockAddress, NBLOCKS));
        runCommand(readCommand(0, logicalBlockAddress, NBLOCKS));

        int total = 0;
        for (int i = 0; i < NBLOCKS; i++)
        {
                total += memcmp(written[i], hostBuffers[i], DMA_SIZE);
        }
        return total;
}

int main(void)
{
        // Initialise the SD Card and FAT file system functions,
        // and ensure
        filesystemInitialise();

        // Initialise the SCSI emulation
        scsiInitialise();

        // Create LUNs
        runCommand(modeSelectCommand(0));
        runCommand(formatCommand(0));
        runCommand(modeSelectCommand(1));
        runCommand(formatCommand(1));

        printf("1..%d\n", NTESTS);
        int test = 0;

        for (int i = 0; i < NTESTS * 256; i += 256)
        {
                if ((writeAndRead(i)) != 0)
                        printf("not ");
                printf("ok %d - read and write %d blocks from block %d\n", ++test, NBLOCKS, i);
        }
}

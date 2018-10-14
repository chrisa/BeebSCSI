#include "beebscsi.h"
#include "uart.h"
#include "debug.h"
#include "hostadapter.h"

uint8_t hostBus;
bool nrstFlag = false;

uint8_t hostBuffers[256][DMA_SIZE];
uint8_t (*hostBuffer)[256] = &hostBuffers[0];

uint8_t *loadedCommand;

bool hostadapterConnectedToExternalBus(void)
{
        return true; // hardcode external / winchester
}

// device->host
uint16_t hostadapterPerformReadDMA(uint8_t *dataBuffer)
{
        memcpy(hostBuffer, dataBuffer, DMA_SIZE);
        hostBuffer++;
        return DMA_SIZE;
}

// host->device
uint16_t hostadapterPerformWriteDMA(uint8_t *dataBuffer)
{
        memcpy(dataBuffer, hostBuffer, DMA_SIZE);
        hostBuffer++;
        return DMA_SIZE;
}

uint8_t hostadapterReadByte(void)
{
        return *loadedCommand++;
}

void hostadapterWriteByte(uint8_t databusValue)
{
        hostBus = databusValue;
}

uint8_t hostadapterReadDatabus(void)
{
	return hostBus;
}

void hostadapterWritedatabus(uint8_t databusValue)
{
        hostBus = databusValue;
}

void hostadapterWriteResetFlag(bool flagState)
{
	nrstFlag = flagState;
}

bool hostadapterReadResetFlag(void)
{
	return nrstFlag;
}

void hostadapterWriteDataPhaseFlags(bool message, bool commandNotData, bool inputNotOutput)
{

}

void hostadapterWriteBusyFlag(bool flagState)
{

}

void hostadapterWriteRequestFlag(bool flagState)
{

}

bool hostadapterReadSelectFlag(void)
{
        return true;
}


#include "beebscsi.h"
#include "uart.h"

uint16_t uartAvailable(void)
{
        return 0;
}

void uartFlush(void)
{

}

bool uartPeekForString(void)
{
        return false;
}

uint16_t uartRead(void)
{
        return UART_NO_DATA;
}

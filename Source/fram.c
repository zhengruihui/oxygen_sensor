#include <msp430.h>
#include <Source/fram.h>

void FRAMWrite (unsigned long * FRAM_write_ptr, unsigned long data)
{
    SYSCFG0 = FRWPPW | PFWP;
    *FRAM_write_ptr = data;
    SYSCFG0 = FRWPPW | DFWP | PFWP;
}

unsigned long FRAMRead (unsigned long * FRAM_write_ptr)
{
    return *FRAM_write_ptr;
}

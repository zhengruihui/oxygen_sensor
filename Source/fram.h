#ifndef	_FRAM_H_
#define	_FRAM_H_
void FRAMWrite (unsigned long * FRAM_write_ptr, unsigned long data);
unsigned long FRAMRead (unsigned long * FRAM_write_ptr);
#endif

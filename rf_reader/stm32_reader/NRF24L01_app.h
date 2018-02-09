#include <rtdef.h>
#include <rtthread.h>
#include <NRF24L01.h>
#include <spi.h>
#include <NRF24L01.h>

static void NRF24L01_thread_entry(void * parameter);
void NRF24L01_thread_init(void);


#include <rtdef.h>
#include <led.h>
#include <rtthread.h>

static void led_thread_entry(void * parameter);
void led_thread_init(void);

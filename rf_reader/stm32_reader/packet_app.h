#include <rtthread.h>
#include <rtdef.h>

static void packet_thread_entry(void * parameter);
void packet_thread_init(void);
static void connect_thread_entry(void * parameter);
void connect_thread_init(void);

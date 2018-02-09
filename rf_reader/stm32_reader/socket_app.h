#include <rtthread.h>
#include <rtdef.h>

#include <lwip/sockets.h>

static void socket_thread_entry(void * parameter);
void socket_thread_init(void);

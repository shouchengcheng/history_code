#include <rtthread.h>
#include <rtdef.h>
#include <door.h>

static void door_thread_entry(void * parameter);
void door_thread_init(void);

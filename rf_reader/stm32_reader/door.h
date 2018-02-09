#include <rtthread.h>

#define DOOR_OPEN 0
#define DOOR_CLOSE 1
#define DOOR_STOP 2

void rt_hw_door_init(void);
void rt_hw_door_on(rt_uint32_t door);
void rt_hw_door_off(rt_uint32_t door);

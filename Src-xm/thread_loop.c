#include <entry.h>

rt_thread_t loop_thread = RT_NULL;

void loop (void* parameter);

int loop_start(void)
{
  RTT_CREATE(loop,loop,RT_NULL,1024,5,20);
  return 0;
}

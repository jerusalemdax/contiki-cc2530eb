#include "contiki.h"
#include <stdio.h>

static count;
static struct etimer et_hello;
PROCESS(example_pollhandler, "Pollhandler example");
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process, &example_pollhandler);

static void
exithandler(void)
{
  printf("Process exited\n\r");
}

static void
pollhandler(void)
{
  printf("Process polled\n\r");
}

PROCESS_THREAD(example_pollhandler, ev, data)
{

  PROCESS_POLLHANDLER(pollhandler());
  PROCESS_EXITHANDLER(exithandler());

  PROCESS_BEGIN();
  printf ("this is poll handler example\n\r");

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  PROCESS_END();
}

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et_hello, CLOCK_SECOND * 1);
  count = 0;

  while(1) {
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER) {
      printf("hello world\n\r");
      count++;
      process_poll(&example_pollhandler);
      if (count == 5)
      {
          process_exit(&example_pollhandler);
      }

      etimer_reset(&et_hello);
    }
  }

  PROCESS_END();
}

#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

static process_event_t event_data_ready;

PROCESS(count_process, "count process");
PROCESS(print_process, "print process");

AUTOSTART_PROCESSES(&count_process, &print_process);

PROCESS_THREAD(count_process, ev, data)
{
    static struct etimer count_timer;
    static int count = 0;
    PROCESS_BEGIN();
    event_data_ready = process_alloc_event();
    etimer_set(&count_timer, CLOCK_SECOND / 2);
    while(1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        leds_toggle(LEDS_ALL);
        count ++;
        process_post(&print_process, event_data_ready, &count);
        etimer_reset(&count_timer);
    }
    PROCESS_END();
}

PROCESS_THREAD(print_process, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
        printf("counter is %d\n\r", (*(int *)data));
    }
    PROCESS_END();
}

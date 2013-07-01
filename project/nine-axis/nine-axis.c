#include "contiki.h"
#include "dev/leds.h"
#include "adxl345.h"

#include <stdio.h> /* For printf() */
extern uint8_t POS_TEMP[6];
/*---------------------------------------------------------------------------*/
static struct etimer et;
static ADXL345_DATA ad_dat;
/*---------------------------------------------------------------------------*/
PROCESS(nine_axis_process, "nine axis process");
AUTOSTART_PROCESSES(&nine_axis_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nine_axis_process, ev, data)
{
  PROCESS_BEGIN();
  Init_ADXL345();

  etimer_set(&et, CLOCK_SECOND /10);

  while(1) {
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER) {
        Multiple_Read_ADXL345(&ad_dat);
        printf ("x = %d, y = %d, z = %d\n\r",ad_dat.x,ad_dat.y,ad_dat.z);
        etimer_reset(&et);
    }
  }

  PROCESS_END();
}

#include "dev/port.h"
#include "dev/air-sensor.h"
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
static CC_AT_DATA struct timer debouncetimer;
/*---------------------------------------------------------------------------*/
/* Button 1 - SmartRF and cc2531 USB Dongle */
/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  type;
  return 1;
//  return BUTTON_READ(1) || !timer_expired(&debouncetimer);
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  switch(type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
      return PORT_IRQ_ENABLED(AIR_PORT, AIR_PIN);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
  switch(type) {
  case SENSORS_HW_INIT:
    P0INP |= 2; /* Tri-state */
    PORT_IRQ_EDGE_FALL(AIR_PORT, AIR_PIN);
    PORT_FUNC_GPIO(AIR_PORT, AIR_PIN);
    PORT_DIR_INPUT(AIR_PORT, AIR_PIN);
    return 1;
  case SENSORS_ACTIVE:
    if(value) {
	if(!PORT_IRQ_ENABLED(AIR_PORT, AIR_PIN)) {
        timer_set(&debouncetimer, 0);
        PORT_IRQ_FLAG_OFF(AIR_PORT, AIR_PIN);
        PORT_IRQ_ENABLE(AIR_PORT, AIR_PIN);
      }
    } else {
	PORT_IRQ_DISABLE(AIR_PORT, AIR_PIN);
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
/* ISRs */
/*---------------------------------------------------------------------------*/
/* avoid referencing bits, we don't call code which use them */
#pragma save
#if CC_CONF_OPTIMIZE_STACK_SIZE
#pragma exclude bits
#endif
void
port_0_isr(void) __interrupt(P0INT_VECTOR)
{
  EA = 0;
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  /* This ISR is for the entire port. Check if the interrupt was caused by our
   * button's pin. */
  leds_on(LEDS_GREEN);
  if(PORT_IRQ_CHECK(AIR_PORT, AIR_PIN)) {
    if(timer_expired(&debouncetimer)) {
      timer_set(&debouncetimer, CLOCK_SECOND / 8);
      //sensors_changed(&button_sensor);
    }
  }

  PORT_IRQ_FLAG_OFF(AIR_PORT, AIR_PIN);

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
  EA = 1;
}
#pragma restore
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(air_sensor, AIR_SENSOR, value, configure, status);

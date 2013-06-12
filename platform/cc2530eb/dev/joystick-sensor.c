#include "sfr-bits.h"
#include "cc253x.h"
#include "adc-sensor.h"
#include "joystick-sensor.h"
#include "dev/port.h"

#if JOYSTICK_SENSOR_ON

static CC_AT_DATA struct timer debouncetimer;

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  uint8_t adc;
  /*
   * For single-shot AD conversions, we may only write to ADCCON3[3:0] once
   * (This write triggers the conversion). We thus use the variable 'command'
   * to store intermediate steps (reference, decimation rate, input channel)
   */
  uint8_t command;

  /* VDD ref, 8bit decimation rate */
  command = ADCCON3_EREF1;

  /* Clear the Interrupt Flag */
  ADCIF = 0;

  /* Depending on the desired reading, append the input bits to 'command' and
   * enable the corresponding input channel in ADCCFG if necessary */
  switch(type) {
#if JOYSTICK_SENSOR_ON
  case ADC_SENSOR_TYPE_JOYSTICK:
    command |= ADCCON3_ECH2 | ADCCON3_ECH1;
    break;
#endif
  default:
    /* If the sensor is not present or disabled in conf, return -1 */
    return -1;
  }

  /* Writing in bits 3:0 of ADCCON3 will trigger a single conversion */
  ADCCON3 = command;

  /*
   * When the conversion is complete, the ADC interrupt flag is set. We don't
   * use an ISR here, we just wait on the flag and clear it afterwards.
   */
  while(!ADCIF);

  /* Clear the Interrupt Flag */
  ADCIF = 0;

  /* 8-bit decimation rate: */
  adc = ADCH;
  if ((adc >= 2) && (adc) <= 38)
  {
      return JOYSTICK_UP;
  } else if ((adc >= 74) && (adc <= 88))
  {
      return JOYSTICK_RIGHT;
  } else if ((adc >= 60) && (adc <= 73))
  {
      return JOYSTICK_LEFT;
  } else if ((adc >= 39) && (adc <= 59))
  {
      return JOYSTICK_DOWN;
  } else
  {
      return JOYSTICK_CENTER;
  }
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
    switch(type) {
        case SENSORS_ACTIVE:
        case SENSORS_READY:
            return PORT_IRQ_ENABLED(JOYSTICK_PORT, JOYSTICK_PIN);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
    switch(type) {
        case SENSORS_HW_INIT:
            PORT_IRQ_EDGE_FALL(JOYSTICK_PORT, JOYSTICK_PIN);
            PORT_FUNC_GPIO(JOYSTICK_PORT, JOYSTICK_PIN);
            PORT_DIR_INPUT(JOYSTICK_PORT, JOYSTICK_PIN);
            APCFG |= 0x40; /* ENABLE P0_6 Channels */
            return 1;
        case SENSORS_ACTIVE:
            if(value)
            {
                if (!PORT_IRQ_ENABLED(JOYSTICK_PORT, JOYSTICK_PIN))
                {
                    timer_set(&debouncetimer, 0);
                    PORT_IRQ_FLAG_OFF(JOYSTICK_PORT, JOYSTICK_PIN);
                    PORT_IRQ_ENABLE(JOYSTICK_PORT, JOYSTICK_PIN);
                }
            } else
            {
                PORT_IRQ_DISABLE(JOYSTICK_PORT, JOYSTICK_PIN);
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
port_2_isr(void) __interrupt(P2INT_VECTOR)
{
  EA = 0;
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  /* This ISR is for the entire port. Check if the interrupt was caused by our
   * button's pin. */
  if(PORT_IRQ_CHECK(JOYSTICK_PORT, JOYSTICK_PIN)) {
    if(timer_expired(&debouncetimer)) {
      timer_set(&debouncetimer, CLOCK_SECOND / 8);
      sensors_changed(&joystick_sensor);
    }
  }

  PORT_IRQ_FLAG_OFF(JOYSTICK_PORT, JOYSTICK_PIN);

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
  EA = 1;
}
#pragma restore
SENSORS_SENSOR(joystick_sensor, JOYSTICK_SENSOR, value, configure, status);
#endif




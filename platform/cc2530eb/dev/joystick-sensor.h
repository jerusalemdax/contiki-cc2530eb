#ifndef JOYSTICK_SENSOR_H
#define JOYSTICK_SENSOR_H

#include "contiki-conf.h"
#include "lib/sensors.h"

#define JOYSTICK_SENSOR "Joystick"

#define ADC_SENSOR_TYPE_JOYSTICK    0

#define JOYSTICK_PORT   2
#define JOYSTICK_PIN    0

#ifdef JOYSTICK_SENSOR_CONF_ON
#define JOYSTICK_SENSOR_ON JOYSTICK_SENSOR_CONF_ON
#endif

extern const struct sensors_sensor joystick_sensor;

#if JOYSTICK_SENSOR_ON
void port_2_isr(void) __interrupt(P2INT_VECTOR);
#define JOYSTICK_SENSOR_ACTIVATE() joystick_sensor.configure(SENSORS_ACTIVE, 1)
#else
#define JOYSTICK_SENSOR_ACTIVATE()
#endif

#define JOYSTICK_CENTER 0x01
#define JOYSTICK_UP     0x02
#define JOYSTICK_DOWN   0x04
#define JOYSTICK_LEFT   0x08
#define JOYSTICK_RIGHT  0x10

#endif

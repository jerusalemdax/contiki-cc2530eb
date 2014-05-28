#ifndef __AIR_SENSOR_H__
#define __AIR_SENSOR_H__

#include "contiki-conf.h"
#include "lib/sensors.h"

#define AIR_SENSOR "AIR"

#define AIR_PORT 0
#define AIR_PIN  5

#ifdef AIR_SENSOR_CONF_ON
#define AIR_SENSOR_ON AIR_SENSOR_CONF_ON
#endif

extern const struct sensors_sensor air_sensor;

#if AIR_SENSOR_ON
void port_0_isr(void) __interrupt(P0INT_VECTOR);
#define   AIR_SENSOR_ACTIVATE() air_sensor.configure(SENSORS_ACTIVE, 1)

#else
#define   AIR_SENSOR_ACTIVATE()
#endif 

#endif

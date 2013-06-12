/*
 * Copyright (c) 2010, Loughborough University - Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 * 
 * \author
 *         Daniel-D Dax - <jerusalemdax@gmail.com>
 */

#include "contiki.h"
#include "contiki-conf.h"
#include "dev/leds.h"

#include "dev/joystick-sensor.h"

#define DEBUG 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
/* We overwrite (read as annihilate) all output functions here */
#define PRINTF(...)
#endif /* DEBUG */
/*---------------------------------------------------------------------------*/
#if JOYSTICK_SENSOR_ON
PROCESS(joystick_test_process, "Joystick Test Process");
AUTOSTART_PROCESSES(&joystick_test_process);
#endif
/*---------------------------------------------------------------------------*/
#if JOYSTICK_SENSOR_ON
PROCESS_THREAD(joystick_test_process, ev, data)
{
  struct sensors_sensor *sensor;
  static int keys;

  PROCESS_BEGIN();

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event);

    /* If we woke up after a sensor event, inform what happened */
    sensor = (struct sensors_sensor *)data;
    if(sensor == &joystick_sensor) {
        keys = sensor->value(ADC_SENSOR_TYPE_JOYSTICK);
        if (keys & JOYSTICK_CENTER)
        {
            printf ("Key center press\n\r");
        }
        if (keys & JOYSTICK_UP)
        {
            printf ("Key up press\n\r");
            leds_toggle(LEDS_1);
        }
        if (keys & JOYSTICK_DOWN)
        {
            printf ("key down press\n\r");
            leds_toggle(LEDS_2);
        }
        if (keys & JOYSTICK_LEFT)
        {
            printf ("key left press\n\r");
            leds_toggle(LEDS_3);
        }
        if (keys & JOYSTICK_RIGHT)
        {
            printf ("key right press\n\r");
            leds_toggle(LEDS_4);
        }
    }
  }

  PROCESS_END();
}
#endif

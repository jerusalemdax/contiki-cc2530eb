#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "dev/leds.h"
#include "drivers/adxl345.h"
#include "drivers/l3g4200d.h"
#include "drivers/hmc5883.h"
#include "drivers/bmp085.h"

#include <stdio.h> /* For printf() */

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define MAX_PAYLOAD_LEN		18

struct nine_axis_data{
    ADXL345_DATA ad_dat;
    L3G4200D_DATA l3_dat;
    HMC5883_DATA hm_dat;
    BMP085_DATA bmp_dat;
} send_data;

/*---------------------------------------------------------------------------*/
static struct etimer et;
static struct uip_udp_conn *client_conn;
static uip_ipaddr_t ipaddr;
/*---------------------------------------------------------------------------*/
static void
timeout_handler(void)
{
#if SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION
  uip_udp_packet_send(client_conn, &send_data, UIP_APPDATA_SIZE);
#else /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */
  uip_udp_packet_send(client_conn, &send_data, 15);
#endif /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */
}
/*---------------------------------------------------------------------------*/
PROCESS(nine_axis_process, "nine axis process");
AUTOSTART_PROCESSES(&nine_axis_process);

/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Server IPv6 addresses:\n\r");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n\r");
    }
  }
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(nine_axis_process, ev, data)
{
  PROCESS_BEGIN();
  Init_ADXL345();
  Init_L3G4200D();
  Init_HMC5883();
  Init_BMP085();

#if UIP_CONF_ROUTER
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
#endif /* UIP_CONF_ROUTER */


  print_local_addresses();

  uip_ip6addr(&ipaddr,0xaaaa,0,0,0,0,0,0,0x01);

  client_conn = udp_new(&ipaddr, UIP_HTONS(3000), NULL);

  etimer_set(&et, CLOCK_SECOND);
  while(1) {
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER) {
        Multiple_Read_ADXL345(&send_data.ad_dat);
        Multiple_Read_L3G4200D(&send_data.l3_dat);
        Multiple_Read_HMC5883(&send_data.hm_dat);
        Multiple_Read_BMP085(&send_data.bmp_dat);

        printf ("adxl345: x = %d, y = %d, z = %d\n\r",send_data.ad_dat.x,send_data.ad_dat.y,send_data.ad_dat.z);
        printf ("l3g4200d: x = %d, y= %d, z= %d\n\r",send_data.l3_dat.x,send_data.l3_dat.y,send_data.l3_dat.z);
        printf ("hmc5883: x = %d, y= %d, z= %d\n\r",send_data.hm_dat.x,send_data.hm_dat.y,send_data.hm_dat.z);
        printf ("bmp085: temp = %d, press= %ld\n\r",send_data.bmp_dat.temp,send_data.bmp_dat.press);
        timeout_handler();
        etimer_reset(&et);
    }
  }

  PROCESS_END();
}

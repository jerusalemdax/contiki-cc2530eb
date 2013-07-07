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

#define DEVICE_RH	0x00
#define DEVICE_LH	0x01
#define DEVICE_RL	0x02
#define DEVICE_LL	0x03

#define DEV_ID DEVICE_RH

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])
struct nine_axis_data{
    uint8_t dev_id;
    ADXL345_DATA ad_dat;
    L3G4200D_DATA l3_dat;
    HMC5883_DATA hm_dat;
    BMP085_DATA bmp_dat;
} send_data;

/*---------------------------------------------------------------------------*/
static struct etimer et;
static struct uip_udp_conn *server_conn;
static uip_ipaddr_t ipaddr;
static uint8_t testid;
static char testch[20];
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
    leds_on(LEDS_RED);
    if(uip_newdata())
    {
        if (((char *)uip_appdata)[0] == 0)
        {
            Multiple_Read_ADXL345(&send_data.ad_dat);
            Multiple_Read_L3G4200D(&send_data.l3_dat);
            Multiple_Read_HMC5883(&send_data.hm_dat);
            Multiple_Read_BMP085(&send_data.bmp_dat);

            uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);

            server_conn->rport = UIP_UDP_BUF->srcport;
            printf ("port: %d\n\r", server_conn->rport);
#if SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION
            uip_udp_packet_send(server_conn, &send_data, UIP_APPDATA_SIZE);
#else /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */
            uip_udp_packet_send(server_conn, &send_data, sizeof(send_data));
#endif /* SEND_TOO_LARGE_PACKET_TO_TEST_FRAGMENTATION */
        }
        memset(&server_conn->ripaddr, 0, sizeof(server_conn->ripaddr));
        server_conn->rport = 0;
    }
    leds_off(LEDS_RED);
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
  send_data.dev_id = DEV_ID;

#if UIP_CONF_ROUTER
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
#endif /* UIP_CONF_ROUTER */

  print_local_addresses();

  server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(server_conn, UIP_HTONS(3000));

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
        tcpip_handler();
    }
  }

  PROCESS_END();
}

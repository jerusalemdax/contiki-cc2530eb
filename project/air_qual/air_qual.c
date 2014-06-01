#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>
#include "dev/leds.h"
#include "debug.h"
#include "dev/button-sensor.h"

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define SEND_INTERVAL		10 * CLOCK_SECOND
#define MAX_PAYLOAD_LEN		40

static char buf[MAX_PAYLOAD_LEN];

/* Our destinations and udp conns. One link-local and one global */
#if UIP_CONF_ROUTER
#define GLOBAL_CONN_PORT 1234
static struct uip_udp_conn *g_conn;
#endif

static struct sensors_sensor *sensor;
static int rv;

static char *str;
static struct etimer et;

static uip_ipaddr_t ipaddr;


/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  leds_on(LEDS_GREEN);
  if(uip_newdata()) {
      str = uip_appdata;
      str[uip_datalen()] = '\0';
      printf("Response from the server: '%s'\n\r", str);
  }
  leds_off(LEDS_GREEN);
}
/*---------------------------------------------------------------------------*/
static void
timeout_handler(void)
{
    leds_on(LEDS_RED);
    sensor = sensors_find(BUTTON_SENSOR);
    if(sensor)
    {
	PRINTF("-------------------\n\r");
	rv = sensor->value(0);
	PRINTF("count : %d\n\r", rv);
	if(uip_ds6_get_global(ADDR_PREFERRED) == NULL) {
	    return;
	}
	uip_udp_packet_send(g_conn, &rv, sizeof(rv));

    }
    leds_off(LEDS_RED);
}
static int i;
static uint8_t state;
static void print_local_addresses(void)
{
  PRINTF("Client IPv6 addresses:\n\r");
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
#if UIP_CONF_ROUTER
static void
set_global_address(void)
{

    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
}
#endif /* UIP_CONF_ROUTER */

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{

    PROCESS_BEGIN();
    PRINTF("UDP client process started\n\r");
    
#if UIP_CONF_ROUTER
    set_global_address();
#endif

    print_local_addresses();

    uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);

    g_conn = udp_new(&ipaddr, UIP_HTONS(1234), NULL);
    if(!g_conn)
    {
	PRINTF("udp_new g_conn error.\n\r");
    }
    udp_bind(g_conn, UIP_HTONS(GLOBAL_CONN_PORT));

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
	PROCESS_YIELD();
	if(etimer_expired(&et)) {
	    timeout_handler();
	    etimer_restart(&et);
	} else if(ev == tcpip_event) {
	    tcpip_handler();
	}
    }
          
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/

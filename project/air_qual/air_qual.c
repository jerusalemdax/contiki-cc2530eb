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
#define LOCAL_CONN_PORT 3001
static struct uip_udp_conn *l_conn;
#if UIP_CONF_ROUTER
#define GLOBAL_CONN_PORT 3002
static struct uip_udp_conn *g_conn;
#endif

static struct sensors_sensor *sensor;
static int rv;

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  /* leds_on(LEDS_GREEN); */
  /* if(uip_newdata()) { */
  /*   putstring("0x"); */
  /*   puthex(uip_datalen()); */
  /*   putstring(" bytes response=0x"); */
  /*   puthex((*(uint16_t *) uip_appdata) >> 8); */
  /*   puthex((*(uint16_t *) uip_appdata) & 0xFF); */
  /*   putchar('\n'); */
  /*   putchar('\r'); */
  /* } */
  /* leds_off(LEDS_GREEN); */
  return;
}
/*---------------------------------------------------------------------------*/
static void
timeout_handler(void)
{
  /* static int seq_id; */
  /* struct uip_udp_conn *this_conn; */
    PRINTF("test\n\r");
    sensor = sensors_find(BUTTON_SENSOR);
    if(sensor)
    {
	leds_on(LEDS_RED);
	PRINTF("-------------------\n\r");
	rv = sensor->value(0);
	PRINTF("count : %d\n\r", rv);
    }
  /* memset(buf, 0, MAX_PAYLOAD_LEN); */
  /* seq_id++; */

  /* /\* evens / odds *\/ */
  /* if(seq_id & 0x01) { */
  /*   this_conn = l_conn; */
  /* } else { */
  /*   this_conn = g_conn; */
  /*   if(uip_ds6_get_global(ADDR_PREFERRED) == NULL) { */
  /*     return; */
  /*   } */
  /* } */

  /* PRINTF("Client to: "); */
  /* PRINT6ADDR(&this_conn->ripaddr); */

  /* memcpy(buf, &seq_id, sizeof(seq_id)); */

  /* PRINTF(" Remote Port %u,", UIP_HTONS(this_conn->rport)); */
  /* PRINTF(" (msg=0x%04x), %u bytes\n\r", *(uint16_t *) buf, sizeof(seq_id)); */

  /* uip_udp_packet_send(this_conn, buf, sizeof(seq_id)); */
  leds_off(LEDS_RED);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer et;

  /* uip_ipaddr_t ipaddr; */

  PROCESS_BEGIN();
  PRINTF("UDP client process started\n\r");

  /* uip_ip6addr(&ipaddr, 0xfe80, 0, 0, 0, 0x0212, 0x4b00, 0x0260, 0xd0e4); */
  /* /\* new connection with remote host *\/ */
  /* l_conn = udp_new(&ipaddr, UIP_HTONS(3000), NULL); */
  /* if(!l_conn) { */
  /*   PRINTF("udp_new l_conn error.\n\r"); */
  /* } */
  /* udp_bind(l_conn, UIP_HTONS(LOCAL_CONN_PORT)); */

  /* PRINTF("Link-Local connection with "); */
  /* PRINT6ADDR(&l_conn->ripaddr); */
  /* PRINTF(" local/remote port %u/%u\n\r", */
  /*        UIP_HTONS(l_conn->lport), UIP_HTONS(l_conn->rport)); */

  /* uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0x0212, 0x4b00, 0x0260, 0xd0e4); */
  /* g_conn = udp_new(&ipaddr, UIP_HTONS(3000), NULL); */
  /* if(!g_conn) { */
  /*   PRINTF("udp_new g_conn error.\n\r"); */
  /* } */
  /* udp_bind(g_conn, UIP_HTONS(GLOBAL_CONN_PORT)); */

  /* PRINTF("Global connection with "); */
  /* PRINT6ADDR(&g_conn->ripaddr); */
  /* PRINTF(" local/remote port %u/%u\n\r", */
  /*        UIP_HTONS(g_conn->lport), UIP_HTONS(g_conn->rport)); */

  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&et)) {
      timeout_handler();
      etimer_restart(&et);
    } else if(ev == tcpip_event) {
      /* tcpip_handler(); */
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
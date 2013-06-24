#include "contiki-net.h"
#include <stdio.h>

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

static struct psock ps;
static char buffer[100];
static uip_ipaddr_t addr;

PROCESS(example_psock_client_process, "Example protosocket client");
AUTOSTART_PROCESSES(&example_psock_client_process);

/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

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

static int
handle_connection(struct psock *p)
{
  PSOCK_BEGIN(p);

  PSOCK_SEND_STR(p, "GET / HTTP/1.0\r\n");
  PSOCK_SEND_STR(p, "Server: Contiki example protosocket client\r\n");
  PSOCK_SEND_STR(p, "\r\n");

  while(1) {
    PSOCK_READTO(p, '\n');
    printf("Got: %s", buffer);
  }

  PSOCK_END(p);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_psock_client_process, ev, data)
{

  PROCESS_BEGIN();

  printf ("this is psock client example\n\r");

#if UIP_CONF_ROUTER
  set_global_address();
#endif
  print_local_addresses();

  uip_ip6addr(&addr, 0xaaaa, 0, 0, 0, 0x0212, 0x4b00, 0x0260, 0xd0e4);

  tcp_connect(&addr, UIP_HTONS(1010), NULL);

  printf("Connecting...\n\r");
  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

  if(uip_aborted() || uip_timedout() || uip_closed()) {
    printf("Could not establish connection\n\r");
  } else if(uip_connected()) {
    printf("Connected\n\r");

    PSOCK_INIT(&ps, buffer, sizeof(buffer));

    do {
      handle_connection(&ps);
      PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    } while(!(uip_closed() || uip_aborted() || uip_timedout()));

    printf("\n\rConnection closed.\n\r");
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

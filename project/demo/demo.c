/*
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
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "debug.h"

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define SEND_INTERVAL		2 * CLOCK_SECOND
#define MAX_PAYLOAD_LEN		40

static char buf[MAX_PAYLOAD_LEN];

/* Our destinations and udp conns. One link-local and one global */
#if UIP_CONF_ROUTER
#define GLOBAL_CONN_PORT 2333
static struct uip_udp_conn *g_conn;
#endif

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  leds_on(LEDS_GREEN);
  if(uip_newdata()) {
    putstring("0x");
    puthex(uip_datalen());
    putstring(" bytes response=0x");
    puthex((*(uint16_t *) uip_appdata) >> 8);
    puthex((*(uint16_t *) uip_appdata) & 0xFF);
    putchar('\n');
    putchar('\r');
  }
  leds_off(LEDS_GREEN);
  return;
}
/*---------------------------------------------------------------------------*/
static void
timeout_handler(void)
{
    static int seq_id;
    struct uip_udp_conn *this_conn;

    leds_on(LEDS_RED);
    memset(buf, 0, MAX_PAYLOAD_LEN);

    /* evens / odds */
    this_conn = g_conn;
    if(uip_ds6_get_global(ADDR_PREFERRED) == NULL) {
	return;
    }

    PRINTF("Client to: ");
    PRINT6ADDR(&this_conn->ripaddr);

    memcpy(buf, "hello\n", 6);

    PRINTF(" Remote Port %u \n\r", UIP_HTONS(this_conn->rport));

    uip_udp_packet_send(this_conn, buf, 6);
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
    static struct etimer et;
    uip_ipaddr_t ipaddr;

    PROCESS_BEGIN();
    PRINTF("UDP client process started\n\r");

#if UIP_CONF_ROUTER
    set_global_address();
#endif

    print_local_addresses();

    uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);
    g_conn = udp_new(&ipaddr, UIP_HTONS(GLOBAL_CONN_PORT), NULL);
    if(!g_conn) {
	PRINTF("udp_new g_conn error.\n\r");
    }
    udp_bind(g_conn, UIP_HTONS(GLOBAL_CONN_PORT));
    PRINTF("Global connection with ");
    PRINT6ADDR(&g_conn->ripaddr);
    PRINTF(" local/remote port %u/%u\n\r",
	   UIP_HTONS(g_conn->lport), UIP_HTONS(g_conn->rport));

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

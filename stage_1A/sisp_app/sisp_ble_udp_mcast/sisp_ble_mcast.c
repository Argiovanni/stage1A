#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ipv6/multicast/uip-mcast6.h"
#include "sys/clock.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#include <string.h>
#include <inttypes.h>

#define DEBUG DEBUG_PRINT
#include "net/ipv6/uip-debug.h"
#include "net/routing/routing.h"

#define MAX_PAYLOAD_LEN 120
#define MCAST_SINK_UDP_PORT 3001 /* Host byte order */

#define SEND_INTERVAL 128 /* clock ticks between send*/
#define DELTA 4

/* Start sending messages START_DELAY secs after we start so that routing can
 * converge */
#define START_DELAY 10

static struct uip_udp_conn *mcast_conn;
static struct uip_udp_conn *sink_conn;

static char buf[MAX_PAYLOAD_LEN];

static clock_time_t l_clock = 0; // local clock
static clock_time_t s_clock = 0; // shared clock

#if !NETSTACK_CONF_WITH_IPV6 || !UIP_CONF_ROUTER || !UIP_IPV6_MULTICAST || !UIP_CONF_IPV6_RPL
#error "This example can not work with the current contiki configuration"
#error "Check the values of: NETSTACK_CONF_WITH_IPV6, UIP_CONF_ROUTER, UIP_CONF_IPV6_RPL"
#endif

PROCESS(mcast_node_process, "Multicast Node (Sink and Root)");
AUTOSTART_PROCESSES(&mcast_node_process);

/*---------------------------------------------------------------------------*/
static void multicast_send(void){ // sending msg
    memset(buf, 0, MAX_PAYLOAD_LEN);
    memcpy(buf, &s_clock, sizeof(s_clock));

    PRINTF("Send to: ");
    PRINT6ADDR(&mcast_conn->ripaddr);
    PRINTF(" Remote Port %u,", uip_ntohs(mcast_conn->rport));
    PRINTF(" (s_clock = %lu)", s_clock);
    PRINTF(" %u bytes\n", sizeof(s_clock));

    uip_udp_packet_send(mcast_conn, buf, sizeof(s_clock));
}

/*---------------------------------------------------------------------------*/
static void tcpip_handler(void){ // receiving msg
    clock_time_t rx_clock;
    if (uip_newdata()){
        PRINTF("received uip data : ");
        if (uip_datalen() == sizeof(clock_time_t)){
            memcpy(&rx_clock, uip_appdata, sizeof(clock_time_t));
            PRINTF("%lu\n", rx_clock);
            if ((rx_clock >= s_clock - DELTA) && (rx_clock <= s_clock + DELTA)){
                LOG_INFO("\n//////////////////////////////////////\n\tSYNCHRONIZED !\n//////////////////////////////////////\n");
                // process_exit(&mcast_node_process);
            }
            s_clock = (s_clock + rx_clock) / 2;
        }
    }
    return;
}

/*---------------------------------------------------------------------------*/
static void prepare_mcast(void){
    uip_ipaddr_t ipaddr;

#if UIP_MCAST6_CONF_ENGINE == UIP_MCAST6_ENGINE_MPL
    uip_ip6addr(&ipaddr, 0xFF03, 0, 0, 0, 0, 0, 0, 0xFC);
#else
    uip_ip6addr(&ipaddr, 0xFF1E, 0, 0, 0, 0, 0, 0, 0x89, 0xABCD);
#endif
    mcast_conn = udp_new(&ipaddr, UIP_HTONS(MCAST_SINK_UDP_PORT), NULL);
}

#if UIP_MCAST6_CONF_ENGINE != UIP_MCAST6_ENGINE_MPL
static uip_ds6_maddr_t *join_mcast_group(void){
    uip_ipaddr_t addr;
    uip_ds6_maddr_t *rv;
    const uip_ipaddr_t *default_prefix = uip_ds6_default_prefix();

    uip_ip6addr_copy(&addr, default_prefix);
    uip_ds6_set_addr_iid(&addr, &uip_lladdr);
    uip_ds6_addr_add(&addr, 0, ADDR_AUTOCONF);

    uip_ip6addr(&addr, 0xFF1E, 0, 0, 0, 0, 0, 0x89, 0xABCD);
    rv = uip_ds6_maddr_add(&addr);

    if (rv){
        PRINTF("Joined multicast group ");
        PRINT6ADDR(&uip_ds6_maddr_lookup(&addr)->ipaddr);
        PRINTF("\n");
    }
    return rv;
}
#endif

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(mcast_node_process, ev, data){
    static struct etimer et;
    PROCESS_BEGIN();
    PRINTF("Multicast Engine: '%s'\n", UIP_MCAST6.name);

#if UIP_MCAST6_CONF_ENGINE != UIP_MCAST6_ENGINE_MPL
    if (join_mcast_group() == NULL){
        PRINTF("Failed to join multicast group\n");
        PROCESS_EXIT();
    }
#endif

    sink_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    if (sink_conn == NULL){
        PRINTF("No UDP connection available, exiting the process!\n");
        PROCESS_EXIT();
    }

    udp_bind(sink_conn, UIP_HTONS(MCAST_SINK_UDP_PORT));

    PRINTF("Listening: ");
    PRINT6ADDR(&sink_conn->ripaddr);
    PRINTF(" local/remote port %u/%u\n",
           UIP_HTONS(sink_conn->lport), UIP_HTONS(sink_conn->rport));

    NETSTACK_ROUTING.root_start();
    prepare_mcast();
    etimer_set(&et, START_DELAY * CLOCK_SECOND);
    while (1){
        PROCESS_YIELD();
        l_clock += 1;
        s_clock += 1;
        if (ev == tcpip_event){
            tcpip_handler();
        }
        if(etimer_expired(&et)){
            if ((l_clock % SEND_INTERVAL) == 0){ // s_clock need to be sent
                PRINTF("l_clock = %lu\n", l_clock);
                multicast_send();
            }
            etimer_set(&et,CLOCK_SECOND/128); // (clock_second = 128) but putting 1 directly doesn't seems to work
        }
    }

    PROCESS_END();
}

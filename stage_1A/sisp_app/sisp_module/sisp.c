/**
 * @file sisp.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ ven. 28 juin 2024 16:20:39 CEST
 * $Rev$
 * $Author$ irt (Arthur BONGIOVANNI)
 * $URL$ /home/irt/Iot/contiki-ng/os/services/sisp_module/sisp.c
 * 
 * SISP implementation in Contiki
 * 
 */

#include "contiki.h"
#include "contiki-net.h"

#include "sys/process.h"
#include "sys/clock.h"
#include <sys/etimer.h>

#include "net/netstack.h"
 
#include "lib/random.h"
#include "sys/node-id.h"

#include <stdlib.h>

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "SISP"
#define LOG_LEVEL LOG_LEVEL_DBG

static unsigned int start_delay;
static unsigned int send_interval;
static unsigned int delta;

static clock_time_t l_clock; // local clock
static clock_time_t s_clock; // shared clock
static unsigned int isSync = 0; // boolean

#if SISP_PP
static unsigned int loneliness_counter = 0;
static unsigned int loneliness_thr = 10;
#endif

PROCESS(sisp_process, "sisp_process");


void
sisp_init(int delay, int interval, int synch_thr)
{
    if (delay == 0){ 
        // if no delay, only 1 clock tic before start of main loop
        start_delay = 1;
    } else {
        start_delay = delay * CLOCK_SECOND;
    }
    send_interval = interval;
    delta = synch_thr;
    process_start(&sisp_process,NULL);
}

unsigned int sisp_node_issync(void){
    return isSync;
}

void sisp_get_sCLK(clock_time_t *clock){
    *clock = s_clock;
}
/*-----------------------------------------------------------------------------------------*/
#if NETSTACK_CONF_WITH_NULLNETT
#include "net/nullnet/nullnet.h"
/*-----------------------------------------------------------------------------------------*/

void
input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len == (sizeof(clock_time_t)+sizeof(unsigned int))){
        clock_time_t rx_clock;
        unsigned int rx_sync;
        memcpy(&rx_clock, data, sizeof(clock_time_t));
        memcpy(&rx_sync, data + sizeof(clock_time_t),sizeof(unsigned int));
        LOG_DBG("received %lu and %d from ", rx_clock, rx_sync);
        LOG_DBG_LLADDR(src);

    #if SISP_PP
        loneliness_counter = 0;
        if (isSync && !rx_sync){
            // We are synced but other node is not : ignore it
            LOG_DBG("case 1\n");
        } else if (rx_sync && !isSync){ 
            // We are not synced but other node is : follow it
            LOG_DBG("case 2\n");
            s_clock = rx_clock;
            isSync = 1;
        }else {
            if (abs(rx_clock - s_clock) <= (2*delta)){
                LOG_DBG("\n\tSYNCHRONIZED !\n");
                s_clock = (s_clock + rx_clock) / 2;
                isSync = 1;
            } else { // limite case
                if (rx_clock < s_clock){
                    // Received node is too far behind, ignore it
                    LOG_DBG("case 3\n");
                } else {
                    // Received node is too far ahead, follow it
                    LOG_DBG("case 4\n");
                    s_clock = rx_clock;
                    isSync = 1;
        }   }   }
    #else // simple SISP
        if (abs(rx_clock - s_clock) <= delta){
            LOG_DBG("\n\tSYNCHRONIZED !\n");
            // process_exit(&sisp_process);            
        }
        s_clock = (s_clock + rx_clock) / 2;
    #endif
    }
}

/* ---Process sisp nullnet--- */
PROCESS_THREAD(sisp_process, ev, data) 
{
    static struct etimer periodic_timer;
    PROCESS_BEGIN();
#if SISP_PP
    LOG_DBG("RUNNING SISP_PP\n");
#else
    LOG_DBG("RUNNING SIMPLE SISP\n");
#endif
    /*Initialyze stuff */
    LOG_DBG("Node_ID is : %d\n", node_id);
    l_clock = random_rand() % send_interval;
    s_clock = l_clock  + 1000*(node_id - 48);

    nullnet_buf = (uint8_t *)&s_clock;
    nullnet_len = sizeof(s_clock);
    nullnet_set_input_callback(input_callback);
    
    etimer_set(&periodic_timer, start_delay);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        l_clock +=1;
        s_clock +=1;
    #if SISP_PP
        loneliness_counter ++;
        if ((isSync) && (loneliness_counter > loneliness_thr) ){
            // consider that a node isn't sync anymore if it didn't receive any msg within loneliness_thr period
            isSync = 0;
        }
    #endif
        if ((l_clock % send_interval) == 0){
            LOG_DBG("Broadcasting : %lu and %d\n", s_clock, isSync);
            memcpy(nullnet_buf, &s_clock, sizeof(s_clock));
            memcpy(nullnet_buf+sizeof(s_clock),isSync, sizeof(isSync))
            nullnet_len = sizeof(s_clock)+sizeof(isSync);
            NETSTACK_NETWORK.output(NULL);
        }
        etimer_set(&periodic_timer,CLOCK_SECOND/128); // (clock_second = 128) but putting 1 directly doesn't seems to work
    }

    PROCESS_END();
}
/*-----------------------------------------------------------------------------------------*/
#else // netstack_conf_with_ipv6
#include "net/ipv6/multicast/uip-mcast6.h"
/*-----------------------------------------------------------------------------------------*/
#if !NETSTACK_CONF_WITH_IPV6 || !UIP_CONF_ROUTER || !UIP_IPV6_MULTICAST || !UIP_CONF_IPV6_RPL
#error "This example can not work with the current contiki configuration"
#error "Check the values of: NETSTACK_CONF_WITH_IPV6, UIP_CONF_ROUTER, UIP_CONF_IPV6_RPL"
#endif

#define MAX_PAYLOAD_LEN 120
#define MCAST_SINK_UDP_PORT 3001 /* Host byte order */
static struct uip_udp_conn *mcast_conn;
static struct uip_udp_conn *sink_conn;

static char buf[MAX_PAYLOAD_LEN];


static void multicast_send(void){ // sending msg
#if SISP_PP
    loneliness_counter ++;
    if ((isSync) && (loneliness_counter > loneliness_thr) ){
        // consider that a node isn't sync anymore if it didn't receive any msg within loneliness_thr period
        isSync = 0;
    }
#endif
    // (s_clock << 1) + isSync;
    memset(buf, 0, MAX_PAYLOAD_LEN);
    memcpy(buf, &s_clock, sizeof(s_clock));
    memcpy(buf+sizeof(s_clock), &isSync, sizeof(isSync));
    LOG_DBG("Send to: ");
    LOG_DBG_6ADDR(&mcast_conn->ripaddr);
    LOG_DBG(" Remote Port %u,", uip_ntohs(mcast_conn->rport));
    LOG_DBG(" %u bytes\n", sizeof(s_clock));
    LOG_DBG(" (s_clock = %lu, is_sync = %d)\n", s_clock, isSync);
    uip_udp_packet_send(mcast_conn, buf, sizeof(s_clock)+sizeof(isSync));
}

static void tcpip_handler(void){ // receiving msg
    if (uip_newdata()){
        LOG_DBG("received uip data : ");
        if (uip_datalen() == sizeof(clock_time_t)+sizeof(unsigned int)){
            clock_time_t rx_clock;
            unsigned int rx_sync;
            memcpy(&rx_clock, uip_appdata, sizeof(clock_time_t));
            memcpy(&rx_sync, uip_appdata + sizeof(clock_time_t), sizeof(unsigned int));
            LOG_DBG("rx_clk = %lu, rx_sync = %d\n",rx_clock, rx_sync);
        #if SISP_PP
            loneliness_counter = 0;
            if (isSync && !rx_sync){
                // We are synced but other node is not : ignore it
                LOG_DBG("case 1\n");
            } else if (rx_sync && !isSync){ 
                // We are not synced but other node is : follow it
                LOG_DBG("case 2\n");
                s_clock = rx_clock;
                isSync = 1;
            }else {
                if (abs(rx_clock - s_clock) <= (2*delta)){
                    LOG_DBG("\n\tSYNCHRONIZED !\n");
                    s_clock = (s_clock + rx_clock) / 2;
                    isSync = 1;
                } else { // limite case
                    if (rx_clock < s_clock){
                        // Received node is too far behind, ignore it
                        LOG_DBG("case 3\n");
                    } else {
                        // Received node is too far ahead, follow it
                        LOG_DBG("case 4\n");
                        s_clock = rx_clock;
                        isSync = 1;
            }   }   }
        #else // simple SISP
            if (abs(rx_clock - s_clock) <= delta){
                LOG_DBG("\n\tSYNCHRONIZED !\n");
                // process_exit(&sisp_process);            
            }
            s_clock = (s_clock + rx_clock) / 2;
        #endif
    }   }
    return;
}

static void prepare_mcast(void){
    uip_ipaddr_t ipaddr;
#if UIP_MCAST6_CONF_ENGINE == UIP_MCAST6_ENGINE_MPL
    uip_ip6addr(&ipaddr, 0xFF03, 0, 0, 0, 0, 0, 0, 0xFC);
#else
    uip_ip6addr(&ipaddr, 0xFF1E, 0, 0, 0, 0, 0, 0x89, 0xABCD);
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
        LOG_DBG("Joined multicast group ");
        LOG_DBG_6ADDR(&uip_ds6_maddr_lookup(&addr)->ipaddr);
        LOG_DBG("\n");
    }
    return rv;
}
#endif

/* ---Process sisp mcast ipv6--- */
PROCESS_THREAD(sisp_process, ev, data){
    static struct etimer et;
    PROCESS_BEGIN();
#if SISP_PP
    LOG_DBG("RUNNING SISP_PP\n");
#else
    LOG_DBG("RUNNING SIMPLE SISP\n");
#endif
    LOG_DBG("Multicast Engine: '%s'\n", UIP_MCAST6.name);
#if UIP_MCAST6_CONF_ENGINE != UIP_MCAST6_ENGINE_MPL
    if (join_mcast_group() == NULL){
        LOG_DBG("Failed to join multicast group\n");
        PROCESS_EXIT();
    }
#endif
    sink_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    if (sink_conn == NULL){
        LOG_DBG("No UDP connection available, exiting the process!\n");
        PROCESS_EXIT();
    }
    udp_bind(sink_conn, UIP_HTONS(MCAST_SINK_UDP_PORT));
    LOG_DBG("Listening: ");
    LOG_DBG_6ADDR(&sink_conn->ripaddr);
    LOG_DBG(" local/remote port %u/%u\n",
           UIP_HTONS(sink_conn->lport), UIP_HTONS(sink_conn->rport));

    /*Initialyze stuff */
    LOG_DBG("Node_ID is : %d\n", node_id);
    l_clock = random_rand() % send_interval;
    s_clock = l_clock  + 1000*(node_id - 48);

    NETSTACK_ROUTING.root_start();
    prepare_mcast();
    etimer_set(&et, start_delay);
    while (1){
        PROCESS_YIELD();
        l_clock += 1;
        s_clock += 1;
        if (ev == tcpip_event){
            tcpip_handler();
        }
        if(etimer_expired(&et)){
            if ((l_clock % send_interval) == 0){ // s_clock need to be sent
                LOG_DBG("l_clock = %lu\n", l_clock);
                multicast_send();
            }
            etimer_set(&et,CLOCK_SECOND/128); // (clock_second = 128) but putting 1 directly doesn't seems to work
    }   }
    PROCESS_END();
}
#endif
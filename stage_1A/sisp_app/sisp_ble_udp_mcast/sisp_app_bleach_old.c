/**
 * @file sisp_app.c
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
 * $Author$ irt
 * $URL$ /home/irt/sisp_app
 */

#include "contiki.h"
#include "board-peripherals.h"
#include "sys/clock.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <stdint.h>
#include "sys/etimer.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define L_UDP_PORT 5678 // swap these between board
#define R_UDP_PORT 8765
#define SEND_INTERVAL 5
#define DELTA 1

#define MULTICAST_ADDR "fe80::212:4b00:f23:7002" //fe80::212:4b00:f23:7002 or fe80::212:4b00:1665:5a06 depending of the board
static uip_ipaddr_t addr;

static clock_time_t l_clock =0;
static clock_time_t s_clock =0;

static struct simple_udp_connection broadcast_con;

PROCESS(sisp_app_process, "sisp_app process");
AUTOSTART_PROCESSES(&sisp_app_process);

void send_broadcast_sclock(void){

    char buff[32];
    snprintf(buff, sizeof(buff),"%lu",s_clock);
    LOG_INFO("sending %s\n ",buff);
    simple_udp_sendto(&broadcast_con, buff, strlen(buff)+1, &addr);
}

void
input_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *source_addr,
                uint16_t source_port,
                const uip_ipaddr_t *dest_addr,
                uint16_t dest_port,
                const uint8_t *data, uint16_t datalen)
{
    if (datalen == sizeof(clock_time_t)){
        clock_time_t rx_clock;
        memcpy(&rx_clock, data, sizeof(clock_time_t));
        LOG_INFO("received %lu from ", rx_clock);
        LOG_INFO_6ADDR(source_addr);
        LOG_INFO("\n");
        if ((rx_clock >= s_clock-DELTA) && (rx_clock <= s_clock+DELTA)){
            printf("\tSYNCHRONIZED !\n");
            process_exit(&sisp_app_process);
        }
        s_clock += rx_clock; 
        s_clock /= 2;
    }
    
}

PROCESS_THREAD(sisp_app_process, ev, data) 
{
    static struct etimer periodic_timer;
    PROCESS_BEGIN();

    printf("Contiki-NG is running on %s\nACM2\n", CONTIKI_TARGET_STRING);
    /*Initialyze stuff */
    uiplib_ipaddrconv(MULTICAST_ADDR, &addr);
    if(simple_udp_register(&broadcast_con, L_UDP_PORT, NULL, R_UDP_PORT, input_callback )== 0){
        printf("NO UDP CONNECTION\n");
        PROCESS_EXIT();
    }
    etimer_set(&periodic_timer, CLOCK_SECOND);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        l_clock +=1;
        s_clock +=1;
        if ((l_clock % SEND_INTERVAL) ==0){
            send_broadcast_sclock(); // broadcast to all neighbors
        }
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
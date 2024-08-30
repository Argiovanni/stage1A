#include "contiki.h"
#include "sisp.h"

#define LOG_LEVEL LOG_LEVEL_DBG

#define START_DELAY 10
#define SEND_INTERVAL 128
#define DELTA 15

PROCESS(sisp_app, "sisp_app");
AUTOSTART_PROCESSES(&sisp_app);


PROCESS_THREAD(sisp_app, ev, data) 
{
    PROCESS_BEGIN();

    printf("Contiki-NG is running on %s\n", CONTIKI_TARGET_STRING);
    
    sisp_init(START_DELAY,SEND_INTERVAL,DELTA);
    // clock_time_t sclock;
    // static struct etimer e_timer;
    
    // etimer_set(&e_timer, START_DELAY*CLOCK_SECOND);
    // while (1) {
    //     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&e_timer));
    //     sisp_get_sCLK(&sclock);
    //     printf("sclock = %lu\n",sclock);
    //     if (sisp_node_issync())
    //     {
    //         printf("\tSync ! \n");
    //     }
        
    //     etimer_set(&e_timer, CLOCK_SECOND/32);
    // }
    

    PROCESS_END();
}
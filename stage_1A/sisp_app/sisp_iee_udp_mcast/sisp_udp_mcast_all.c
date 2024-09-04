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
    

    PROCESS_END();
}
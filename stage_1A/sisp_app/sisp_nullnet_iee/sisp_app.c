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
 * 
 * SISP implementation using directly the IEE 802.15.4 contiki driver
 * 
 */

#include "contiki.h"
#include "sisp.h"

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
/**
 * @file helloworld.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ lun. 17 juin 2024 10:48:03 CEST
 * $Rev$
 * $Author$ irt
 * $URL$ /home/irt/IoT/helloworld
 */

#include "contiki.h"
#include "board-peripherals.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/etimer.h>

PROCESS(helloworld_process, "helloworld process");
AUTOSTART_PROCESSES(&helloworld_process);

PROCESS_THREAD(helloworld_process, ev, data) 
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {
    // printf("Hello, world\n");

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }

  PROCESS_END();
}

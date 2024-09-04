#ifndef SISP_H_
#define SISP_H_

#include "contiki.h"

/**
 * The actual sisp process
 */
extern struct process sisp_process;

 /**
   * Start SISP process in a thread
   *
   * \param delay delay in second before broadcast start
   * \param interval interval in clock ticks between each broadcast (for cc2650stk, 1 clk_tick = 7.8125ms )
   * \param synch_thr maximum authorized difference between shared clock and rx_clock to consider them synchronized
   */
void sisp_init(int delay, int interval, int synch_thr);

/**
 * return if the node is currectly sync with sisp
 * \return 1 if node is sync, 0 otherwise. (Without SISP_PP, always return 0.)
*/
unsigned int sisp_node_issync(void);

/**
 * Get current value of Shared_clock and put it at the address pointed by clock
 * \param clock address to stock the shared_clock
*/
void sisp_get_sCLK(clock_time_t *clock);

#endif
/*
 * Copyright (c) 2010, Loughborough University - Computer Science
 * All rights reserved.
 *
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
 */

/**
 * \file
 *         Project specific configuration defines for the RPl multicast
 *         example.
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#include "net/ipv6/multicast/uip-mcast6-engines.h"

/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define BOARD_CONF_DEBUGGER_DEVPACK             1
/*---------------------------------------------------------------------------*/
/* BLEach part */
/*---------------------------------------------------------------------------*/
#define PACKETBUF_CONF_SIZE                   640 // (was 1280)
#define QUEUEBUF_CONF_NUM                       4 // was 1 in ble example, trying 4 (from multicast example)
#define UIP_CONF_BUFFER_SIZE                  640 // (was 1280)

#define NETSTACK_CONF_RADIO                 ble_cc2650_driver

#define LOG_CONF_LEVEL_MAC        LOG_LEVEL_INFO

/* BLE L2CAP settings */
#define BLE_CONF_DEVICE_NAME          "TI CC2650 device"
#define BLE_CONF_ADV_INTERVAL         25

/*/ * 6LoWPAN settings * / */
#define SICSLOWPAN_CONF_MAC_MAX_PAYLOAD      	640 // (was 1280)
#define SICSLOWPAN_CONF_COMPRESSION           SICSLOWPAN_COMPRESSION_6LORH
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD   0  /* always use compression */
#define SICSLOWPAN_CONF_FRAG                    0
#define SICSLOWPAN_FRAMER_HDRLEN                0
/* */
/*/ * network stack settings * / */
#define UIP_CONF_ROUTER                         1 // was 0 but need it to be 1 for multicast exmpl
#define UIP_CONF_ND6_SEND_NA                    1
/*---------------------------------------------------------------------------*/
/* UIP MCAST part */
/*---------------------------------------------------------------------------*/
/* Change this to switch engines. Engine codes in uip-mcast6-engines.h */
#ifndef UIP_MCAST6_CONF_ENGINE
#define UIP_MCAST6_CONF_ENGINE UIP_MCAST6_ENGINE_MPL
#endif

/* For Imin: Use 16 over CSMA, 64 over Contiki MAC */
#define ROLL_TM_CONF_IMIN_1         16
#define MPL_CONF_DATA_MESSAGE_IMIN  16
#define MPL_CONF_CONTROL_MESSAGE_IMIN  16

#define UIP_MCAST6_ROUTE_CONF_ROUTES 1

/* Code/RAM footprint savings so that things will fit on our device */
#ifndef NETSTACK_MAX_ROUTE_ENTRIES
#define NETSTACK_MAX_ROUTE_ENTRIES   4
#endif

#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 4
#endif

#endif /* PROJECT_CONF_H_ */

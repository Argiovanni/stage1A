/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
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
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
/* Enable TCP */
#define UIP_CONF_TCP 1

/* Change to 1 to use with the IBM Watson IoT platform */
#define MQTT_CLIENT_CONF_WITH_IBM_WATSON 0

/*
 * The IPv6 address of the MQTT broker to connect to.
 * Ignored if MQTT_CLIENT_CONF_WITH_IBM_WATSON is 1
 */
#define MQTT_CLIENT_CONF_BROKER_IP_ADDR "fd00::1"

/*
 * The Organisation ID.
 *
 * When in Watson mode, the example will default to Org ID "quickstart" and
 * will connect using non-authenticated mode. If you want to use registered
 * devices, set your Org ID here and then make sure you set the correct token
 * through MQTT_CLIENT_CONF_AUTH_TOKEN.
 */
#ifndef MQTT_CLIENT_CONF_ORG_ID
#define MQTT_CLIENT_CONF_ORG_ID "quickstart"
#endif

/*
 * The MQTT username.
 *
 * Ignored in Watson mode: In this mode the username is always "use-token-auth"
 */
#define MQTT_CLIENT_CONF_USERNAME "mqtt-client-username"

/*
 * The MQTT auth token (password) used when connecting to the MQTT broker.
 *
 * Used with as well as without Watson.
 *
 * Transported in cleartext!
 */
#define MQTT_CLIENT_CONF_AUTH_TOKEN "AUTHTOKEN"
/*---------------------------------------------------------------------------*/
// Part for mcast and sisp
/*---------------------------------------------------------------------------*/
#include "net/ipv6/multicast/uip-mcast6-engines.h"

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

#define QUEUEBUF_CONF_NUM             4

#define UIP_CONF_BUFFER_SIZE        640 // default is 1280

#define SISP_PP         1   // 1 to enable SISP_++
/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
/** @} */

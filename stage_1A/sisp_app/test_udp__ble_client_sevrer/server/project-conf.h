/*
 * Copyright (c) 2017, Graz University of Technology
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

/**
 * \author
 *    Michael Spoerk <michael.spoerk@tugraz.at>
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
#define MODE_IEEE802_15_4 1   /* enable RPL and disable TSCH in Makefile */
#define MODE_BLE      2

#define MODE MODE_BLE
/*---------------------------------------------------------------------------*/
/* Disable button shutdown functionality */
#define BUTTON_SENSOR_CONF_ENABLE_SHUTDOWN    0
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define BOARD_CONF_DEBUGGER_DEVPACK           1
/*---------------------------------------------------------------------------*/
#define PACKETBUF_CONF_SIZE                   1280
#define QUEUEBUF_CONF_NUM                       1
#define UIP_CONF_BUFFER_SIZE                  1280

#define CC26XX_CONF_RADIO_MODE          CC26XX_RADIO_MODE_BLE
#define NETSTACK_CONF_RADIO                 ble_cc2650_driver


// #define LOG_CONF_LEVEL_IPV6             LOG_LEVEL_DBG
// #define LOG_CONF_LEVEL_6LOWPAN          LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_MAC              LOG_LEVEL_INFO
// #define LOG_CONF_LEVEL_RADIO            LOG_LEVEL_DBG

#define RTIMER_CONF_MULTIPLE_ACCESS       1

/* BLE radio settings */
#define BLE_MODE_CONF_INIT_PEER_ADDR      0x98072D3A4482  //this board is : 0xF0F8F2863A86

/* BLE L2CAP settings */
#define BLE_CONF_DEVICE_NAME          "TI CC26xx server device"


/*/ * 6LoWPAN settings * / */
#define SICSLOWPAN_CONF_MAC_MAX_PAYLOAD       1280
#define SICSLOWPAN_CONF_COMPRESSION           SICSLOWPAN_COMPRESSION_6LORH
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD   0  /* always use compression */
#define SICSLOWPAN_CONF_FRAG                    0
#define SICSLOWPAN_FRAMER_HDRLEN                0


/* network layer settings */
#define UIP_CONF_ROUTER                         1
#define UIP_CONF_ND6_SEND_NA                    1
#define UIP_CONF_ND6_SEND_RA                    1
#define UIP_CONF_IP_FORWARD                     0
#define UIP_CONF_ND6_SEND_NS                    1

/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
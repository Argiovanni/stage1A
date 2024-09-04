This module serve as an implementation of the SISP protocole on Contiki-ng
to use it, simply add the following line to your Makefile :
    MODULES += os/services/sisp_module
and start the process with the following call : 
``` C
    sisp_init(START_DELAY,SEND_INTERVAL,DELTA);
```
    Start_delay being the time in second before the start of the process
    Send_interval the number of clock tick between each transmission of a sisp message
    and Delta the maximum difference between to clock_time to consider them synchronized


/!\ for now only work with NullNet over IEEE_802.15.4 (based on nullnet broadcast example)
            and with standard IPV6 over IEEE_802.15.4 (based on multicast example)
                        
/!\ this module has only been tested with cc2650stk board

- with nullnet :
    your makefile need to have the following : 
    -----------------------------------------------------------------------------------
        MAKE_MAC ?= MAKE_MAC_CSMA
        MAKE_NET = MAKE_NET_NULLNET
    -----------------------------------------------------------------------------------

- with ipv6 :
    your makefile need to have the following : 
    -----------------------------------------------------------------------------------
        MODULES += $(CONTIKI_NG_NET_DIR)/ipv6/multicast
        MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
        MAKE_MAC ?= MAKE_MAC_CSMA
        MAKE_NET ?= MAKE_NET_IPV6
    -----------------------------------------------------------------------------------

    your project-conf.h need to have the following :
    -----------------------------------------------------------------------------------
    ```c
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
    ```
    -----------------------------------------------------------------------------------

in order to use SISP_++ (an improved version of SISP), your project-conf.h need to define SISP_PP
-----------------------------------------------------------------------------------
```c
    #define SISP_PP  1
```
-----------------------------------------------------------------------------------
CONTIKI_PROJECT = sisp_udp_mcast_all

BOARD=sensortag/cc2650
# PORT=/dev/ttyACM0

TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

MODULES += os/services/shell 

MODULES += os/services/sisp_module

PLATFORMS_ONLY = cc26x0-cc13x0

all: $(CONTIKI_PROJECT)

CONTIKI = /home/irt/IoT/contiki-ng

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_NET_DIR)/ipv6/multicast

MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET ?= MAKE_NET_IPV6

include $(CONTIKI)/Makefile.include

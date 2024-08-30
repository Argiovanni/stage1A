CONTIKI_PROJECT = sisp_ble_mcast

BOARD=sensortag/cc2650
PORT=/dev/ttyACM0

TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

MODULES += os/services/shell 

PLATFORMS_ONLY = cc26x0-cc13x0

all: $(CONTIKI_PROJECT)

CONTIKI = /home/irt/IoT/contiki-ng

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_NET_DIR)/ipv6/multicast

MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MAKE_MAC = MAKE_MAC_BLE
MAKE_NET = MAKE_NET_IPV6

include $(CONTIKI)/Makefile.include

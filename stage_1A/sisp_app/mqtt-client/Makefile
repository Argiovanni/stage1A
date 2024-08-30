CONTIKI_PROJECT = mqtt-client
all: $(CONTIKI_PROJECT)

BOARD=sensortag/cc2650
TARGET = cc26x0-cc13x0
# PORT=/dev/ttyACM0

CONTIKI = /home/irt/IoT/contiki-ng

MODULES += os/services/shell 
MODULES += $(CONTIKI_NG_NET_DIR)/ipv6/multicast
MODULES += os/services/sisp_module

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_APP_LAYER_DIR)/mqtt

MODULES_REL += arch/platform/$(TARGET)

PLATFORMS_ONLY = cc26x0-cc13x0

# MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET ?= MAKE_NET_IPV6

include $(CONTIKI)/Makefile.include

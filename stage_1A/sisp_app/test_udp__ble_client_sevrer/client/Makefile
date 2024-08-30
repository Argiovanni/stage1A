CONTIKI_PROJECT = test_client

BOARD=sensortag/cc2650

TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

PLATFORMS_ONLY = cc26x0-cc13x0

# Pour avoir acces a un shell et quelques commande de base
MODULES += os/services/shell 

MAKE_MAC = MAKE_MAC_BLE
MAKE_NET = MAKE_NET_IPV6
MAKE_ROUTING = MAKE_ROUTING_NULLROUTING

all: $(CONTIKI_PROJECT)

CONTIKI = /home/irt/IoT/contiki-ng
include $(CONTIKI)/Makefile.include

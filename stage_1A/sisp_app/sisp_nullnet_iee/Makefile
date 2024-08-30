CONTIKI_PROJECT = sisp_app

BOARD=sensortag/cc2650
# PORT=/dev/ttyACM0 
# PORT=/dev/ttyACM2

TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

PLATFORMS_ONLY = cc26x0-cc13x0

# Pour avoir acces a un shell et quelques commande de base
MODULES += os/services/shell 

MODULES += os/services/sisp_module

MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_NET = MAKE_NET_NULLNET

all: $(CONTIKI_PROJECT)


CONTIKI = /home/irt/IoT/contiki-ng
include $(CONTIKI)/Makefile.include


# include $(CONTIKI)/Makefile.dir-variables
# MODULES += $(CONTIKI_NG_NET_DIR)/ipv6/multicast

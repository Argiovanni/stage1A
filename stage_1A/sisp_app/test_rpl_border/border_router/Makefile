CONTIKI_PROJECT = border-router
all: $(CONTIKI_PROJECT)
CONTIKI = /home/irt/IoT/contiki-ng

BOARD=sensortag/cc2650
PLATFORMS_ONLY = cc26x0-cc13x0

# Include RPL BR module
include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/rpl-border-router
# Include webserver module
MODULES_REL += webserver

# include shell module
MODULES += os/services/shell

# Include optional target-specific module
TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

include $(CONTIKI)/Makefile.include

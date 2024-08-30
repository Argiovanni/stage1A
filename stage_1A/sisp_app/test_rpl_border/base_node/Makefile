CONTIKI_PROJECT = helloworld

BOARD=sensortag/cc2650
# PORT=/dev/ttyACM0

TARGET = cc26x0-cc13x0
MODULES_REL += $(TARGET)

PLATFORMS_ONLY = cc26x0-cc13x0

MODULES += os/services/shell

all: $(CONTIKI_PROJECT)

CONTIKI = /home/irt/IoT/contiki-ng
include $(CONTIKI)/Makefile.include

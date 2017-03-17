# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

# Add your debugging flag (or not) to EXTRA_CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS) -I$(LDDINCDIR)

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= simple.o

else




KERNELDIR ?= /lib/modules/$(shell uname -r)/build
KERNELDIR = /home/zdu/data/cgm_sw_r7.0er6-rdkb-2.1-20161219/build-puma7-atom-wifi/tmp/work/puma7_atom_wifi-rdk-linux/linux-yocto-cougarmountain-external/3.12.59+gitAUTOINC+0fd090c8b6-r0/linux-puma7_atom_wifi-standard-build
#CC = i586-rdk-linux-gcc
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINCDIR=$(PWD)/../include modules

endif



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order

depend .depend dep:
	$(CC) $(EXTRA_CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif

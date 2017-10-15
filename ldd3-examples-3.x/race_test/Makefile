
# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.
ifeq ($(KERNELRELEASE),)

    # Assume the source tree is where the running kernel was built
    # You should set KERNELDIR in the environment if it's elsewhere
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    # The current directory is passed to sub-makes as argument
    PWD := $(shell pwd)

KERNELDIR=/home/zdu/data/comcast-rdkb/arris-xb6-sdk7.1-puma7/build-puma7-atom-sdk7x/tmp/work/puma7_atom_sdk7x-rdk-linux/linux-yocto-cougarmountain/3.12.59+gitAUTOINC+0fd090c8b6-r0/linux-puma7_atom_sdk7x-standard-build

CROSS = ARCH=x86 

modules:
	$(MAKE) -C $(KERNELDIR) $(CROSS) M=$(PWD) EXTRA_CFLAGS=-g  modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order

.PHONY: modules modules_install clean

else
    # called from kernel build system: just declare what our modules are
    obj-m := kthread_test.o kthread_tasklet.o tasklet_tasklet.o timer_timer.o rcu_test.o rcu_test_list.o percpu_test.o completion_test.o
endif



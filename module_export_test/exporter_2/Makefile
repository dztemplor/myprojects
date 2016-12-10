#for Cross-compile

#for Local compile
#KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


MODULE_NAME = exporter_2


obj-m := $(MODULE_NAME).o

.PHONY: modules modules_install clean


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order .*.o.d

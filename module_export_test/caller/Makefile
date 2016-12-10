#for Cross-compile
KBUILD_EXTRA_SYMBOLS = /home/zhdu/data/Linux_module_func_export/caller/../exporter_1/Module.symvers
KBUILD_EXTRA_SYMBOLS += /home/zhdu/data/Linux_module_func_export/caller/../exporter_2/Module.symvers
#for Local compile
#KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


MODULE_NAME = caller


obj-m := $(MODULE_NAME).o

.PHONY: modules modules_install clean


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order .*.o.d

CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE), )
CONFIG_MODULE_SIG=n
obj-m :=myCharDevice.o #要编译成的目标文件
#modules-obj :=testDev.o;
else
CONFIG_MODULE_SIG=n
PWD := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
endif
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

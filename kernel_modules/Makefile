MODULE_NAME = shstk_status

# The kernel build system's path
KERNEL_BUILD = /lib/modules/$(shell uname -r)/build
# Command to build the module
obj-m += $(MODULE_NAME).o

EXTRA_CFLAGS += -mshstk
EXTRA_LDFLAGS += -Wl,-z,shstk

all:
	$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) clean

install:
	$(MAKE) -C $(KERNEL_BUILD) M=$(PWD) modules_install
	depmod

remove:
	modprobe -r $(MODULE_NAME)

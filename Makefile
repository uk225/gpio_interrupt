# Name the final module something unique (e.g., uk_i2c_driver.o)
obj-m := gpio_interrupt.o


KERN := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

TARGET_PATH := ~/beaglebone/linux

all:
	make -C $(KERN) M=$(PWD) modules
target:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(TARGET_PATH) M=$(PWD) modules

make_dts:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(TARGET_PATH) M=$(PWD) clean
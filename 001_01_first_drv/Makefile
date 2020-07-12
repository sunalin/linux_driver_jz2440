KERN_DIR = ../../linux-2.6.22.6

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	arm-linux-gcc -o test test.c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -rf test Module.symvers

obj-m += first_drv.o

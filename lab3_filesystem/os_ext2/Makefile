# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the linux ext2-filesystem routines.
#
NAME	= os_ext2
KDIR	= /lib/modules/$(shell uname -r)/build
PWD	= $(shell pwd)

obj-m		+= $(NAME).o

$(NAME)-y	:= balloc.o dir.o file.o ialloc.o inode.o \
			ioctl.o namei.o super.o symlink.o

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean 

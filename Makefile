#
# Makefile for Universal Parallel Bus 
#

ccflags-y := -DDEBUG

umbdriver-y := umb.o
#usbskeldriver-y := usb-skeleton.o

obj-m               += umb.o #usb-skeleton.o

include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Linux driver folder
#ccflags-y += -I$(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/touchpanel/focaltech_hd/
#ccflags-y += -I$(srctree)/arch/arm/mach-$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/touchpanel/focaltech_hd/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/ft6x06_qhd/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/

#obj-y += ft_gesture_lib.o
#$(obj)/ft_gesture_lib.o: $(srctree)/../$(obj)/touchpanel/ft_gesture_lib.o
#	cp $(srctree)/../$(obj)/touchpanel/ft_gesture_lib.o $(obj)/ft_gesture_lib.o

obj-y	+=  ft6x06_driver.o
#obj-y	+=  gt1x_tpd.o
#obj-y	+=  gt1x_update.o


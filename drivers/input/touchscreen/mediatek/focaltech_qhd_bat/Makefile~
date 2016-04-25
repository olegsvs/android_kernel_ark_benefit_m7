include $(srctree)/drivers/misc/mediatek/Makefile.custom


#ccflags-y += -I$(MTK_PROJECT_PATH_ROOT)/touchpanel/ft6x36/
# Linux driver folder
#ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/ft6x36/
#ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/touchpanel/focaltech_qhd/
ccflags-y += -I$(srctree)/arch/arm/mach-$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/touchpanel/focaltech_qhd/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/focaltech_qhd/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/


#obj-y := $(patsubst %.c,%.o,$(subst $(path)/,,$(call wildcard2,$(path)/*.c)))
obj-y += focaltech_ctl.o
obj-y += focaltech_driver.o
obj-y += focaltech_ex_fun.o
obj-y += ft_gesture_lib.a

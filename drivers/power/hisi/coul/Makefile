obj-$(CONFIG_HISI_COUL_HI6555V200)      += hi6xxx/
obj-$(CONFIG_HISI_COUL_HI6555V300)      += hi6xxx/
obj-$(CONFIG_HISI_COUL_HI6421V600)      += hi6xxx/
obj-$(CONFIG_HISI_COUL_HI6421V700)      += hi6xxx/
obj-$(CONFIG_HISI_COUL_HI6421V800)      += hi6xxx/
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver/libhwsecurec/

ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)/

ifneq ($(TARGET_BUILD_VARIANT),user)
ccflags-y += -DISC_TEST
else
ifeq ($(TARGET_VERSION_MODE),factory)
ccflags-y += -DISC_TEST
endif
endif

obj-$(CONFIG_HISI_COUL)             += hisi_coul_core.o hisi_coul_drv.o
obj-$(CONFIG_HISI_COUL)             += hisi_coul_event.o hisi_coul_imonitor.o
obj-$(CONFIG_HISI_COUL_POLAR)       += hisi_coul_polar.o
obj-$(CONFIG_HISI_COUL_TEST)        += hisi_coul_drv_test.o


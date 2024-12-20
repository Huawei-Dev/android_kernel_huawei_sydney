/*
 * Copyright (C) 2018 Hisilicon.
 * Author: Hisilicon <>
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>

#include "securec.h"

#include "include/tcpci.h"
#include "include/tcpci_typec.h"


#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
#include "pd_dpm_prv.h"
#endif /* CONFIG_USB_POWER_DELIVERY_SUPPORT */

#define to_tcpc_device(obj) container_of(obj, struct tcpc_device, dev)
static struct class *hisi_tcpc_class;
static struct device_type tcpc_dev_type;

static inline void tcpc_init_attrs(struct device_type *dev_type){}

static int tcpc_match_device_by_name(struct device *dev, const void *data)
{
	const char *name = data;
	struct tcpc_device *tcpc = dev_get_drvdata(dev);

	return strcmp(tcpc->desc.name, name) == 0;
}

struct tcpc_device *hisi_tcpc_dev_get_by_name(const char *name)
{
	struct device *dev;

	D("+-\n");

	dev = class_find_device(hisi_tcpc_class,
			NULL, (const void *)name, tcpc_match_device_by_name);

	return dev ? dev_get_drvdata(dev) : NULL;
}


static void tcpc_device_release(struct device *dev)
{
	struct tcpc_device *tcpc_dev = to_tcpc_device(dev);

	D("+\n");
	/* Un-init pe thread */
#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	hisi_tcpci_event_deinit(tcpc_dev);
#endif /* CONFIG_USB_POWER_DELIVERY_SUPPORT */

	/* Un-init timer thread */
	hisi_tcpci_timer_deinit(tcpc_dev);
	/* Un-init Mutex */
	/* Do initialization */
	devm_kfree(dev, tcpc_dev);
	D("-\n");
}

struct tcpc_device *hisi_tcpc_device_register(struct device *parent,
	    struct tcpc_desc *tcpc_desc, struct tcpc_ops *ops, void *drv_data)
{
	struct tcpc_device *tcpc;
	int ret = 0;

	D("+\n");

	tcpc = devm_kzalloc(parent, sizeof(*tcpc), GFP_KERNEL);
	if (!tcpc) {
		pr_err("%s : allocate tcpc memeory failed\n", __func__);
		return NULL;
	}

	tcpc->dev.class = hisi_tcpc_class;
	tcpc->dev.type = &tcpc_dev_type;
	tcpc->dev.parent = parent;
	tcpc->dev.release = tcpc_device_release;
	dev_set_drvdata(&tcpc->dev, tcpc);
	tcpc->drv_data = drv_data;
	dev_set_name(&tcpc->dev, tcpc_desc->name);
	tcpc->desc = *tcpc_desc;
	tcpc->ops = ops;
	tcpc->typec_local_rp_level = tcpc_desc->rp_lvl;
	D("typec_local_rp_level %d\n", tcpc->typec_local_rp_level);

	D("to devices_register\n");
	ret = device_register(&tcpc->dev);
	if (ret) {
		devm_kfree(parent, tcpc);
		return ERR_PTR(ret);
	}

	srcu_init_notifier_head(&tcpc->evt_nh);

	mutex_init(&tcpc->access_lock);
	mutex_init(&tcpc->typec_lock);
	mutex_init(&tcpc->timer_lock);
	sema_init(&tcpc->timer_enable_mask_lock, 1);
	spin_lock_init(&tcpc->timer_tick_lock);

	/* If system support "WAKE_LOCK_IDLE",
	 * please use it instead of "WAKE_LOCK_SUSPEND" */
	wake_lock_init(&tcpc->attach_wake_lock, WAKE_LOCK_SUSPEND,
			"hisi_tcpc_attach_wakelock");
	wake_lock_init(&tcpc->dettach_temp_wake_lock, WAKE_LOCK_SUSPEND,
			"hisi_tcpc_detach_wakelock");

	D("to timer_init\n");
	hisi_tcpci_timer_init(tcpc);

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	hisi_tcpci_event_init(tcpc);
	hisi_pd_core_init(tcpc);
#endif /* CONFIG_USB_POWER_DELIVERY_SUPPORT */

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	ret = hisi_tcpc_dual_role_phy_init(tcpc);
	if (ret < 0)
		dev_err(&tcpc->dev, "dual role usb init fail\n");
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

	D("-\n");
	return tcpc;
}
EXPORT_SYMBOL(hisi_tcpc_device_register);


int hisi_tcpc_tcpci_init(struct tcpc_device *tcpc, bool sw_reset)
{
	int ret;
	uint16_t power_status = 0;

	D("+\n");
	if (tcpc->ops->init) {
		ret = tcpc->ops->init(tcpc, sw_reset);
		if (ret)
			return ret;
	} else {
		pr_err("%s Please implment tcpc ops init function\n", __func__);
		return -EINVAL;
	}

	ret = tcpci_get_power_status(tcpc, &power_status);
	if (ret)
		return ret;
	D("power_status 0x%x\n", power_status);
	hisi_tcpci_vbus_level_init(tcpc, power_status);
	D("-\n");
	return 0;
}


static int tcpc_device_irq_enable(struct tcpc_device *tcpc)
{
	int ret;

	D("+\n");

	ret = hisi_tcpc_tcpci_init(tcpc, false);
	if (ret < 0) {
		pr_err("%s tcpc init fail\n", __func__);
		return ret;
	}

	hisi_tcpci_lock_typec(tcpc);
	ret = hisi_tcpc_typec_init(tcpc, tcpc->desc.role_def);
	hisi_tcpci_unlock_typec(tcpc);

	if (ret < 0) {
		pr_err("%s : tcpc typec init fail\n", __func__);
		return ret;
	}

	D("-\n");
	return 0;
}

int hisi_tcpc_schedule_init_work(struct tcpc_device *tcpc)
{
	D("\n");
	return tcpc_device_irq_enable(tcpc);
}
EXPORT_SYMBOL(hisi_tcpc_schedule_init_work);

int hisi_register_tcp_dev_notifier(struct tcpc_device *tcp_dev,
			      struct notifier_block *nb)
{
	int ret;

	D("+\n");
	ret = srcu_notifier_chain_register(&tcp_dev->evt_nh, nb);
	if (ret != 0)
		D("srcu_notifier_chain_register ret:%d\n", ret);

	D("-\n");
	return ret;
}
EXPORT_SYMBOL(hisi_register_tcp_dev_notifier);

int hisi_unregister_tcp_dev_notifier(struct tcpc_device *tcp_dev,
				struct notifier_block *nb)
{
	return srcu_notifier_chain_unregister(&tcp_dev->evt_nh, nb);
}
EXPORT_SYMBOL(hisi_unregister_tcp_dev_notifier);


void hisi_tcpc_device_unregister(struct device *dev, struct tcpc_device *tcpc)
{
	D("+\n");
	if (!tcpc)
		return;

	hisi_tcpc_typec_deinit(tcpc);

	wake_lock_destroy(&tcpc->dettach_temp_wake_lock);
	wake_lock_destroy(&tcpc->attach_wake_lock);

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	devm_dual_role_instance_unregister(&tcpc->dev, tcpc->dr_usb);
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

	device_unregister(&tcpc->dev);
	D("-\n");
}
EXPORT_SYMBOL(hisi_tcpc_device_unregister);

static int __init hisi_tcpc_class_init(void)
{
	D("+\n");
#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	hisi_dpm_check_supported_modes();
#endif /* CONFIG_USB_POWER_DELIVERY_SUPPORT */

	hisi_tcpc_class = class_create(THIS_MODULE, "hisi_tcpc_class");
	if (IS_ERR(hisi_tcpc_class)) {
		pr_info("Unable to create tcpc class; errno = %ld\n", PTR_ERR(hisi_tcpc_class));
		return PTR_ERR(hisi_tcpc_class);
	}
	tcpc_init_attrs(&tcpc_dev_type);
	hisi_tcpc_class->suspend = NULL;
	hisi_tcpc_class->resume = NULL;

	D("-\n");
	return 0;
}

static void __exit hisi_tcpc_class_exit(void)
{
	D("\n");
	class_destroy(hisi_tcpc_class);
}

subsys_initcall(hisi_tcpc_class_init);
module_exit(hisi_tcpc_class_exit);

MODULE_DESCRIPTION("hisilicon tcpc");
MODULE_AUTHOR("hisilicon");
MODULE_LICENSE("GPL");

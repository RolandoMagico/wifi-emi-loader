/*
 * Copyright (c) 2017 MediaTek Inc.
 * Author: Kun-Ze <kun-ze.syue@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef VXWORKS
#include <linux/of_address.h>
#include <linux/of_reserved_mem.h>
#endif
#include "include/hw_ctrl.h"
#include "include/chip/chip.h"


UINT32 gConEmiPhyBase = 0;
UINT32 gConEmiPhySize = 0;
#define OF_EMI_RESERVED_MEMORY_STR "mediatek,leopard-N9-reserved"

INT32 hw_init(struct CORE_ADAPTER *pAd, UINT32 op_mode)
{
	INT32 ret = FUNC_STATUS_SUCCESS;
#ifdef CONFIG_OF
	struct device_node *node = NULL;
	UINT32 emi_phyaddr_info[4] = {0, 0, 0, 0};
#endif /* CONFIG_OF */

	pAd->op_mode = op_mode;

	ret = chip_hook_init(pAd, &pAd->chip_hook);
	if (ret != FUNC_STATUS_SUCCESS) {
		DBG_ERR_FUNC("chip hook init failed!\n");
		return ret;
	}
#ifdef CONFIG_OF
	node = of_find_compatible_node(NULL, NULL, OF_EMI_RESERVED_MEMORY_STR);
	if (!node) {
		DBG_ERR_FUNC("can't found node of %s from dts!\n", OF_EMI_RESERVED_MEMORY_STR);
		return FUNC_STATUS_FAILURE;
	}

	if (of_property_read_u32_array(node, "reg", emi_phyaddr_info, ARRAY_SIZE(emi_phyaddr_info))) {
		DBG_ERR_FUNC("can't get emi physical address from dts!\n");
		return FUNC_STATUS_FAILURE;
	}

	gConEmiPhyBase = emi_phyaddr_info[1];
	gConEmiPhySize = emi_phyaddr_info[3];
#else
	{
		struct CHIP_HOOK *pHook = &pAd->chip_hook;
		if (pHook && pHook->get_phy_emi) {
			pHook->get_phy_emi(&gConEmiPhyBase, &gConEmiPhySize);
		} else {
			DBG_ERR_FUNC("No hook for get_phy_emi!\n");
			return FUNC_STATUS_FAILURE;
		}
	}
#endif /* CONFIG_OF */

	DBG_LOUD_FUNC("base: 0x%08x, size: 0x%08x\n", gConEmiPhyBase, gConEmiPhySize);

	return ret;
}

BOOLEAN is_emi_ready(VOID)
{
	return (gConEmiPhyBase != 0) ? TRUE:FALSE;
}

INT32 set_connsys_emi_remapping(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = FUNC_STATUS_FAILURE;

	if (pHook && pHook->set_emi_remapping)
		ret = pHook->set_emi_remapping(gConEmiPhyBase);
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_connsys_emi_download(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = FUNC_STATUS_FAILURE;

	if (pHook && pHook->do_emi_download)
		ret = pHook->do_emi_download(gConEmiPhyBase, gConEmiPhySize);
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 set_emi_hw_mode_bypass(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = FUNC_STATUS_FAILURE;

	if (pHook && pHook->set_emi_hw_mode_bypass)
		ret = pHook->set_emi_hw_mode_bypass();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 set_connsys_sw_mcu_reset(struct CORE_ADAPTER *pAd, BOOLEAN bAssert)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->set_sw_mcu_reset)
		ret = pHook->set_sw_mcu_reset(bAssert);
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 set_connsys_sw_conn_reset(struct CORE_ADAPTER *pAd, BOOLEAN bAssert)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->set_sw_conn_reset)
		ret = pHook->set_sw_conn_reset(bAssert);
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}


INT32 do_turn_on_spm_clock(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_turn_on_spm_clock)
		ret = pHook->do_turn_on_spm_clock();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_ahb_axi_bus_init(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_ahb_axi_bus_init)
		ret = pHook->do_ahb_axi_bus_init();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 set_spm_hw_mask_disable(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->set_spm_hw_mask_disable)
		ret = pHook->set_spm_hw_mask_disable();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_check_connsys_version(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_check_connsys_version)
		ret = pHook->do_check_connsys_version();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_check_connsys_complete(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_check_connsys_complete)
		ret = pHook->do_check_connsys_complete(pAd->op_mode);
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_adie_coclock_init(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_adie_coclock_init)
		ret = pHook->do_adie_coclock_init();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

INT32 do_rtc_clock_init(struct CORE_ADAPTER *pAd)
{
	struct CHIP_HOOK *pHook = &pAd->chip_hook;
	INT32 ret = 0;

	if (pHook && pHook->do_rtc_clock_init)
		ret = pHook->do_rtc_clock_init();
	else
		ret = FUNC_STATUS_NOT_SUPPORT;

	return ret;
}

#if 0
/* reserved memory from device tree */
INT32 hw_reserve_memory_connsys_hook(struct reserved_mem *rmem)
{
	DBG_LOUD_FUNC(" name: %s, base: 0x%llx, size: 0x%llx\n",
		rmem->name, (ULONGLONG)rmem->base, (ULONGLONG)rmem->size);

	gConEmiPhyBase = rmem->base;
	return FUNC_STATUS_SUCCESS;
}

RESERVEDMEM_OF_DECLARE(reserve_N9_memory, OF_EMI_RESERVED_MEMORY_STR, hw_reserve_memory_connsys_hook);
#endif

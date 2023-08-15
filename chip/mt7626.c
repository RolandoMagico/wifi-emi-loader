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
#include <linux/delay.h>
#endif
#include "include/chip/chip.h"
#include "include/emi_bin2h.h"

static CHIP_INFO ChipInfo =
{
#ifndef CONFIG_OF
#ifndef VXWORKS
	.emi_phy_base			  = 0x41000000,
	.emi_phy_size			  = 0x200000,
#else
	/* Set to correct memory address and memory size.
	 * For Vxworks, we set emi memory to the beginning of DDR.
	 */
	.emi_phy_base			  = 0x40000000,
	.emi_phy_size			  = 0x200000,
#endif
#endif /* CONFIG_OF */
	.connsys_emi_remapping_reg         = 0x10000370, /* conn2infra_ahb_remap_emi_base */
	.connsys_emi_remapping_reg_mask    = 0x00000FFF,	/* 0x10000370[11:0] */
	.connsys_emi_ctrl_reg		   = 0x18002150,
	.connsys_emi_ctrl_bypass_hw_mask   = 0x00208001, /* 0x18002150[21]=1'b1 0x18002150[15]=1'b1 0x18002150[0]=1'b1 */
	.ap_side_emi_offset                = 24,         /* AP side EMI address[35:24] for Connsys MCU */

	.swsys_mcu_reset_reg              = 0x10212018,
	.swsys_mcu_reset_mask             = 0x00001000, /* conn_mcu_rst[12] */
	.swsys_mcu_reset_writekey         = 0x88,       /* write key to unblock write protection */
	.swsys_mcu_reset_writekey_offset  = 24,         /* 0x10212018[31:24] */

	.swsys_conn_reset_reg		  = 0x10212018,
	.swsys_conn_reset_mask		  = 0x00000200, /* conn_rst[9] */
	.swsys_conn_reset_writekey	  = 0x88,	/* write key to unblock write protection */
	.swsys_conn_reset_writekey_offset = 24, 	/* 0x10212018[31:24] */

	.infra_topaxi_prot_reg            = 0x10000230,
	.infra_topaxi_prot_ahb_rx_mask    = 0x00000001, /* 0x10000230[0]: AP2CONN AHB RX bus related setting */
	.infra_topaxi_prot_ahb_tx_mask    = 0x00000002, /* 0x10000230[1]: CONN2AP AHB TX bus related setting */
	.infra_topaxi_prot_axi_tx_mask    = 0x00000004, /* 0x10000230[2]: CONN2AP AXI TX bus related setting */
	.infra_topaxi_prot_status_reg     = 0x10000238,
	.infra_topaxi_prot_status_ahb_rx_mask = 0x00000001, /* 0x10000238[0] polling: AP2CONN AHB RX bus related setting */
	.infra_topaxi_prot_status_ahb_tx_mask = 0x00000002, /* 0x10000238[1] polling: CONN2AP AHB TX bus related setting */
	.infra_topaxi_prot_status_axi_tx_mask = 0x00000004, /* 0x10000238[2] polling: CONN2AP AXI TX bus related setting */

	.topaxi_prot_en_reg               = 0x10000220,
	.topaxi_prot_en_ahb_tx_mask       = 0x00000100, /* 0x10000220[8]: AP2CONN AHB TX bus related setting */
	.topaxi_prot_en_ahb_rx_mask       = 0x00008000, /* 0x10000220[15]: CONN2AP AHB RX bus related setting */
	.topaxi_prot_en_axi_rx_mask       = 0x00000004, /* 0x10000220[2]: CONN2AP AXI RX bus related setting */
	.topaxi_prot_status_reg           = 0x10000228,
	.topaxi_prot_status_ahb_tx_mask   = 0x00000100, /* 0x10000228[8] polling: AP2CONN AHB TX bus related setting */
	.topaxi_prot_status_ahb_rx_mask   = 0x00008000, /* 0x10000228[15] polling: CONN2AP AHB RX bus related setting */
	.topaxi_prot_status_axi_rx_mask   = 0x00000004, /* 0x10000228[2] polling: CONN2AP AXI RX bus related setting */

	.ap_standby_con_reg               = 0x10006608,
	.ap_standby_con_conn_mask         = 0x00800000, /* 0x10006608[23] */
	.ap_standby_md_vrf18_req_mask     = 0x20000000, /* 0x10006608[29] */

	.connsys_ver_id_reg                = 0x180B1010,
	.connsys_ver_id_mask               = 0xFFFFFFFF, /* 0x180B1010[31:0] */
	.connsys_ver_id_expected           = 0x10040100,
	.connsys_conf_id_reg               = 0x180B101C,
	.connsys_conf_id_mask              = 0xF,        /* 0x180B101C[3:0] */
	.connsys_hw_ver_id_reg             = 0x18002000,
	.connsys_hw_ver_id_mask            = 0xFFFF,     /* 0x18002000[15:0] */
	.connsys_fw_ver_id_reg             = 0x18002004,
	.connsys_fw_ver_id_mask            = 0xFFFF,     /* 0x18002004[15:0] */

	.connsys_pwr_on_complete_reg       = 0x18002600,
	.connsys_pwr_on_complete_expected  = 0x00001D1E,
	.connsys_pwr_on_complete_mask      = 0xFFFFFFFF, /* 0x18002600[31:0] */

	.connsys_conn_on_rsv_reg           = 0x18001130,
	.connsys_conn_on_rsv_mask	   = 0x00000001, /* for 7761 co-clock setting */

	.clock_mode_topckgen_reg	   = 0x10210000, /* 10210000[2:3] rtc_sel */
	.clock_mode_topckgen_mask          = 0xC,

	.spm_clock_reg              = 0x10006000,
	.spm_clock_mask             = 0x00000001, /* 1'b1 Enable */
	.spm_clock_writekey         = 0x0b16,     /* write key to unblock write protection */
	.spm_clock_writekey_offset  = 16,         /* 0x10006000[31:16] */

	/* EMI Download Load Address */
	.mcu_rom_emi_addr_offset         = 0,		/* 0xF0000000 */
	.wifi_rom_emi_addr_offset        = 0x43000,	/* 0xF0043000 */
#if !defined(WIFI_RAM_EMI_SUPPORT) && defined(VXWORKS)
	/* Already move these download to wifi driver */
	.wifi_ram_ilm_emi_addr_offset    = 0x75000,	/* 0xF0075000 */
	.wifi_ram_dlm_emi_addr_offset    = 0x146800, /* 0xF0146800 */
#endif
};

INT32 set_emi_remapping_reg(UINT32 EmiPhyBase)
{
	UINT32 prefix_phy_addr = 0;
	UINT8 __iomem *vir_addr = NULL;

	if (EmiPhyBase == 0)
		return FUNC_STATUS_FAILURE;

	vir_addr = ioremap_nocache(ChipInfo.connsys_emi_remapping_reg, 4);
	if (vir_addr != 0) {
		prefix_phy_addr = ((EmiPhyBase >> ChipInfo.ap_side_emi_offset) & ChipInfo.connsys_emi_remapping_reg_mask);
		IO_CLR_BIT_32(vir_addr, ChipInfo.connsys_emi_remapping_reg_mask);
		IO_SET_BIT_32(vir_addr, prefix_phy_addr);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}
	return FUNC_STATUS_SUCCESS;
}

INT32 do_emi_download_proc(UINT32 phy_emi_addr, UINT32 phy_emi_size)
{
	UINT8 __iomem *vir_addr = NULL, *target_vir_addr = NULL;

	if (phy_emi_addr == 0 || phy_emi_size == 0)
		return FUNC_STATUS_FAILURE;

	vir_addr = ioremap_nocache(phy_emi_addr, phy_emi_size);
	if (vir_addr != 0) {
		target_vir_addr = vir_addr + ChipInfo.mcu_rom_emi_addr_offset;
		/* mcu rom emi download */
		memmove(target_vir_addr, mcu_emi_rom, sizeof(mcu_emi_rom));

		/* wifi rom emi download */
		target_vir_addr = vir_addr + ChipInfo.wifi_rom_emi_addr_offset;
		memmove(target_vir_addr, wifi_emi_rom, sizeof(wifi_emi_rom));
#if !defined(WIFI_RAM_EMI_SUPPORT) && defined(VXWORKS)
		/* Already move these download to wifi driver */

		/* wifi ram ilm emi download */
		target_vir_addr = vir_addr + ChipInfo.wifi_ram_ilm_emi_addr_offset;
		memmove(target_vir_addr, wifi_emi_ram_ilm, sizeof(wifi_emi_ram_ilm));

		/* wifi ram dlm emi download */
		target_vir_addr = vir_addr + ChipInfo.wifi_ram_dlm_emi_addr_offset;
		memmove(target_vir_addr, wifi_emi_ram_dlm, sizeof(wifi_emi_ram_dlm));
#endif
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}
	return FUNC_STATUS_SUCCESS;
}

INT32 set_emi_hw_mode_bypass_reg(VOID)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.connsys_emi_ctrl_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val |= ChipInfo.connsys_emi_ctrl_bypass_hw_mask;
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 set_sw_mcu_reset_reg(BOOLEAN bAssert)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.swsys_mcu_reset_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		if (bAssert)
			reg_val |= ChipInfo.swsys_mcu_reset_mask;
		else
			reg_val &= ~ChipInfo.swsys_mcu_reset_mask;
		reg_val |= (ChipInfo.swsys_mcu_reset_writekey << ChipInfo.swsys_mcu_reset_writekey_offset);
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 set_sw_conn_reset_reg(BOOLEAN bAssert)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.swsys_conn_reset_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		if (bAssert)
			reg_val |= ChipInfo.swsys_conn_reset_mask;
		else
			reg_val &= ~ChipInfo.swsys_conn_reset_mask;
		reg_val |= (ChipInfo.swsys_conn_reset_writekey << ChipInfo.swsys_conn_reset_writekey_offset);
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_turn_on_spm_clock_reg(VOID)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.spm_clock_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val |= ChipInfo.spm_clock_mask;
		reg_val |= (ChipInfo.spm_clock_writekey << ChipInfo.spm_clock_writekey_offset);
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_ahb_axi_bus_init_proc(VOID)
{
	UINT32 reg_val = 0;
	UINT8  loop = 0, max_loop = 10;
	UINT8 __iomem *infra_topaxi_prot_addr = NULL;
	UINT8 __iomem *infra_topaxi_prot_status_addr = NULL;
	UINT8 __iomem *topaxi_prot_en_addr = NULL;
	UINT8 __iomem *topaxi_prot_status_addr = NULL;

	infra_topaxi_prot_addr = ioremap_nocache(ChipInfo.infra_topaxi_prot_reg, 4);
	infra_topaxi_prot_status_addr = ioremap_nocache(ChipInfo.infra_topaxi_prot_status_reg, 4);
	topaxi_prot_en_addr = ioremap_nocache(ChipInfo.topaxi_prot_en_reg, 4);
	topaxi_prot_status_addr = ioremap_nocache(ChipInfo.topaxi_prot_status_reg, 4);

	if ((infra_topaxi_prot_addr == NULL) ||
	    (infra_topaxi_prot_status_addr == NULL) ||
	    (topaxi_prot_en_addr == NULL) ||
	    (topaxi_prot_status_addr == NULL))
	{
		if (infra_topaxi_prot_addr != NULL)
			iounmap(infra_topaxi_prot_addr);

		if (infra_topaxi_prot_status_addr != NULL)
			iounmap(infra_topaxi_prot_status_addr);

		if (topaxi_prot_en_addr != NULL)
			iounmap(topaxi_prot_en_addr);

		if (topaxi_prot_status_addr != NULL)
			iounmap(topaxi_prot_status_addr);

		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	/* Turn off AP2CONN AHB RX bus sleep protect */
	IO_CLR_BIT_32(infra_topaxi_prot_addr, ChipInfo.infra_topaxi_prot_ahb_rx_mask);

	/* check AP2CONN AHB RX bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(infra_topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.infra_topaxi_prot_status_ahb_rx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	/* Turn off AP2CONN AHB TX bus sleep protect */
	IO_CLR_BIT_32(topaxi_prot_en_addr, ChipInfo.topaxi_prot_en_ahb_tx_mask);

	/* check AP2CONN AHB TX bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.topaxi_prot_status_ahb_tx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	/* Turn off CONN2AP AHB RX bus sleep protect */
	IO_CLR_BIT_32(topaxi_prot_en_addr, ChipInfo.topaxi_prot_en_ahb_rx_mask);

	/* check CONN2AP AHB RX bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.topaxi_prot_status_ahb_rx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	/* Turn off CONN2AP AHB TX bus sleep protect */
	IO_CLR_BIT_32(infra_topaxi_prot_addr, ChipInfo.infra_topaxi_prot_ahb_tx_mask);

	/* check CONN2AP AHB TX bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(infra_topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.infra_topaxi_prot_status_ahb_tx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	/* Turn off AXI Rx bus sleep protect */
	IO_CLR_BIT_32(topaxi_prot_en_addr, ChipInfo.topaxi_prot_en_axi_rx_mask);

	/* check AXI Rx bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.topaxi_prot_status_axi_rx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	/* Turn off AXI TX bus sleep protect */
	IO_CLR_BIT_32(infra_topaxi_prot_addr, ChipInfo.infra_topaxi_prot_axi_tx_mask);

	/* check AXI Tx bus sleep protect turn off */
	loop = 0;
	while (loop < max_loop) {
		reg_val = IO_R_32(infra_topaxi_prot_status_addr);
		if ((reg_val & ChipInfo.infra_topaxi_prot_status_axi_tx_mask) == 0)
			break;

		loop++;
		udelay(500); /* 0.5ms */
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 set_spm_hw_mask_disable_reg(VOID)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.ap_standby_con_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val |= (ChipInfo.ap_standby_con_conn_mask | ChipInfo.ap_standby_md_vrf18_req_mask);
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_check_connsys_version_proc(VOID)
{
	UINT8  loop = 0, max_loop = 10;
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	/* check CONNSYS version ID */
	vir_addr = ioremap_nocache(ChipInfo.connsys_ver_id_reg, 4);
	if (vir_addr != 0) {
		while (loop < max_loop) {
			reg_val = IO_R_32(vir_addr);
			reg_val &= ChipInfo.connsys_ver_id_mask;
			if (reg_val >= ChipInfo.connsys_ver_id_expected)
				break;

			loop++;
			mdelay(20); /* 20ms */
		}
		iounmap(vir_addr);

		if (loop >= max_loop) {
			DBG_ERR_FUNC("check CONNSYS version ID fail! over %d times!\n", max_loop);
			return FUNC_STATUS_FAILURE;
		}
		DBG_LOUD_FUNC("CONNSYS version ID=0x%08x\n", reg_val);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	/* check CONNSYS configuration ID */
	vir_addr = ioremap_nocache(ChipInfo.connsys_conf_id_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val &= ChipInfo.connsys_conf_id_mask;
		DBG_LOUD_FUNC("CONNSYS configuration ID=0x%08x\n", reg_val);

		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	/* check CONNSYS HW version ID */
	vir_addr = ioremap_nocache(ChipInfo.connsys_hw_ver_id_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val &= ChipInfo.connsys_hw_ver_id_mask;
		DBG_LOUD_FUNC("CONNSYS HW version ID=0x%08x\n", reg_val);

		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	/* check CONNSYS FW version ID */
	vir_addr = ioremap_nocache(ChipInfo.connsys_fw_ver_id_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val &= ChipInfo.connsys_fw_ver_id_mask;
		DBG_LOUD_FUNC("CONNSYS FW version ID=0x%08x\n", reg_val);

		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_check_connsys_complete_proc(UINT32 op_mode)
{
	UINT8  loop = 0, max_loop = 50;
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.connsys_pwr_on_complete_reg, 4);
	if (vir_addr != 0) {
		if (op_mode == FPGA_MODE) {
			reg_val = IO_R_32(vir_addr);
			iounmap(vir_addr);
		} else {
			while (loop < max_loop) {
				reg_val = IO_R_32(vir_addr);
				reg_val &= ChipInfo.connsys_pwr_on_complete_mask;
				if (reg_val == ChipInfo.connsys_pwr_on_complete_expected) {
					break;
				}

				loop++;
				mdelay(1); /* 1ms */
			}
			iounmap(vir_addr);

			if (loop >= max_loop) {
				DBG_ERR_FUNC("connsys power on complete fail! over %d times!\n", max_loop);
				return FUNC_STATUS_FAILURE;
			}
		}
		/* expect 0x0000_1D1E */
		DBG_LOUD_FUNC("connsys power on complete ID=0x%08x\n", reg_val);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_adie_coclock_init_proc(VOID)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.connsys_conn_on_rsv_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val |= ChipInfo.connsys_conn_on_rsv_mask;
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}

INT32 do_rtc_clock_init_proc(VOID)
{
	UINT32 reg_val = 0;
	UINT8 __iomem *vir_addr = NULL;

	vir_addr = ioremap_nocache(ChipInfo.clock_mode_topckgen_reg, 4);
	if (vir_addr != 0) {
		reg_val = IO_R_32(vir_addr);
		reg_val |= ChipInfo.clock_mode_topckgen_mask;
		IO_W_32(vir_addr, reg_val);
		iounmap(vir_addr);
	} else {
		DBG_ERR_FUNC("ioremap fail!\n");
		return FUNC_STATUS_FAILURE;
	}

	return FUNC_STATUS_SUCCESS;
}


#ifndef CONFIG_OF
INT32 get_phy_emi(PUINT32 phy_emi_addr, PUINT32 phy_emi_size)
{
	*phy_emi_addr = ChipInfo.emi_phy_base;
	*phy_emi_size = ChipInfo.emi_phy_size;

	return FUNC_STATUS_SUCCESS;
}
#endif /* CONFIG_OF */

INT32 chip_hook_init(struct CORE_ADAPTER *pAd, struct CHIP_HOOK *pChipHook)
{
	if (pAd == NULL || pChipHook == NULL)
		return FUNC_STATUS_FAILURE;

	pChipHook->set_emi_remapping = set_emi_remapping_reg;
	pChipHook->do_emi_download = do_emi_download_proc;
	pChipHook->set_emi_hw_mode_bypass = set_emi_hw_mode_bypass_reg;
	pChipHook->set_sw_mcu_reset = set_sw_mcu_reset_reg;
	pChipHook->set_sw_conn_reset = set_sw_conn_reset_reg;
	pChipHook->do_turn_on_spm_clock = do_turn_on_spm_clock_reg;
	pChipHook->do_ahb_axi_bus_init = do_ahb_axi_bus_init_proc;
	pChipHook->set_spm_hw_mask_disable = set_spm_hw_mask_disable_reg;
	pChipHook->do_check_connsys_version = do_check_connsys_version_proc;
	pChipHook->do_check_connsys_complete = do_check_connsys_complete_proc;
	pChipHook->do_adie_coclock_init = do_adie_coclock_init_proc;
	pChipHook->do_rtc_clock_init = do_rtc_clock_init_proc;
#ifndef CONFIG_OF
	pChipHook->get_phy_emi = get_phy_emi;
#endif /* CONFIG_OF */
	return FUNC_STATUS_SUCCESS;
}


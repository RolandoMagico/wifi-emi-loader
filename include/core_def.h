#ifndef __CORE_DEF_H__
#define __CORE_DEF_H__

#ifndef VXWORKS
#include <linux/device.h>
#else
#include "include/vxworks.h"
#endif
#include "include/reg_io.h"
#include "include/debug.h"




enum {
	ASIC_MODE = 0,
	FPGA_MODE
};

struct CORE_ADAPTER;

struct CHIP_HOOK {
	INT32 (*set_emi_remapping)(UINT32 EmiPhyBase);
	INT32 (*do_emi_download)(UINT32 phy_emi_addr, UINT32 phy_emi_size);
	INT32 (*set_emi_hw_mode_bypass)(VOID);
	INT32 (*set_sw_mcu_reset)(BOOLEAN bAssert);
	INT32 (*set_sw_conn_reset)(BOOLEAN bAssert);
	INT32 (*do_turn_on_spm_clock)(VOID);
	INT32 (*do_ahb_axi_bus_init)(VOID);
	INT32 (*set_spm_hw_mask_disable)(VOID);
	INT32 (*do_check_connsys_version)(VOID);
	INT32 (*do_check_connsys_complete)(UINT32 op_mode);
	INT32 (*do_adie_coclock_init)(VOID);
	INT32 (*do_rtc_clock_init)(VOID);
#ifndef CONFIG_OF
	INT32 (*get_phy_emi)(PUINT32 phy_emi_addr, PUINT32 phy_emi_size);
#endif /* CONFIG_OF */
};

typedef struct _CHIP_INFO {
#ifndef CONFIG_OF
	UINT32 emi_phy_base;
	UINT32 emi_phy_size;
#endif /* CONFIG_OF */
	UINT32 connsys_emi_remapping_reg;
	UINT32 connsys_emi_remapping_reg_mask;
	UINT32 connsys_emi_ctrl_reg;
	UINT32 connsys_emi_ctrl_bypass_hw_mask;
	UINT32 ap_side_emi_offset;
	UINT32 swsys_mcu_reset_reg;
	UINT32 swsys_mcu_reset_mask;
	UINT32 swsys_mcu_reset_writekey;
	UINT32 swsys_mcu_reset_writekey_offset;

	UINT32 swsys_conn_reset_reg;
	UINT32 swsys_conn_reset_mask;
	UINT32 swsys_conn_reset_writekey;
	UINT32 swsys_conn_reset_writekey_offset;

	UINT32 spm_clock_reg;
	UINT32 spm_clock_mask;
	UINT32 spm_clock_writekey;
	UINT32 spm_clock_writekey_offset;

	UINT32 infra_topaxi_prot_reg;
	UINT32 infra_topaxi_prot_ahb_rx_mask;
	UINT32 infra_topaxi_prot_ahb_tx_mask;
	UINT32 infra_topaxi_prot_axi_tx_mask;
	UINT32 infra_topaxi_prot_status_reg;
	UINT32 infra_topaxi_prot_status_ahb_rx_mask;
	UINT32 infra_topaxi_prot_status_ahb_tx_mask;
	UINT32 infra_topaxi_prot_status_axi_tx_mask;

	UINT32 topaxi_prot_en_reg;
	UINT32 topaxi_prot_en_ahb_tx_mask;
	UINT32 topaxi_prot_en_ahb_rx_mask;
	UINT32 topaxi_prot_en_axi_rx_mask;
	UINT32 topaxi_prot_status_reg;
	UINT32 topaxi_prot_status_ahb_tx_mask;
	UINT32 topaxi_prot_status_ahb_rx_mask;
	UINT32 topaxi_prot_status_axi_rx_mask;

	UINT32 ap_standby_con_reg;
	UINT32 ap_standby_con_conn_mask;
	UINT32 ap_standby_md_vrf18_req_mask;

	UINT32 connsys_ver_id_reg;
	UINT32 connsys_ver_id_mask;
	UINT32 connsys_ver_id_expected;
	UINT32 connsys_conf_id_reg;
	UINT32 connsys_conf_id_mask;
	UINT32 connsys_hw_ver_id_reg;
	UINT32 connsys_hw_ver_id_mask;
	UINT32 connsys_fw_ver_id_reg;
	UINT32 connsys_fw_ver_id_mask;

	UINT32 connsys_pwr_on_complete_reg;
	UINT32 connsys_pwr_on_complete_expected;
	UINT32 connsys_pwr_on_complete_mask;

	UINT32 connsys_conn_on_rsv_reg;
	UINT32 connsys_conn_on_rsv_mask;

	UINT32 clock_mode_topckgen_reg;
	UINT32 clock_mode_topckgen_mask;

	UINT32 mcu_rom_emi_addr_offset;
	UINT32 wifi_rom_emi_addr_offset;
	UINT32 wifi_ram_ilm_emi_addr_offset;
	UINT32 wifi_ram_dlm_emi_addr_offset;
}CHIP_INFO;

struct CORE_ADAPTER {
	CHAR name[32];
	struct device *dev;
	BOOLEAN init_done;
	UINT32 op_mode;
	struct CHIP_HOOK chip_hook;
};

#endif /* __CORE_DEF_H__ */

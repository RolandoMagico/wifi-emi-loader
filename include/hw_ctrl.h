#ifndef __HW_CTRL_H__
#define __HW_CTRL_H__

#include "include/core_def.h"

INT32 hw_init(struct CORE_ADAPTER *pAd, UINT32 op_mode);
BOOLEAN is_emi_ready(VOID);
INT32 set_connsys_emi_remapping(struct CORE_ADAPTER *pAd);
INT32 do_connsys_emi_download(struct CORE_ADAPTER *pAd);
INT32 set_emi_hw_mode_bypass(struct CORE_ADAPTER *pAd);
INT32 set_connsys_sw_mcu_reset(struct CORE_ADAPTER *pAd, BOOLEAN bAssert);
INT32 set_connsys_sw_conn_reset(struct CORE_ADAPTER *pAd, BOOLEAN bAssert);
INT32 do_turn_on_spm_clock(struct CORE_ADAPTER *pAd);
INT32 do_ahb_axi_bus_init(struct CORE_ADAPTER *pAd);
INT32 set_spm_hw_mask_disable(struct CORE_ADAPTER *pAd);
INT32 do_check_connsys_version(struct CORE_ADAPTER *pAd);
INT32 do_check_connsys_complete(struct CORE_ADAPTER *pAd);
INT32 do_adie_coclock_init(struct CORE_ADAPTER *pAd);
INT32 do_rtc_clock_init(struct CORE_ADAPTER *pAd);


#endif /* __HW_CTRL_H__ */

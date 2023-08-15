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
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/delay.h>
#else
#include "include/vxworks.h"
#endif
#include "include/hw_ctrl.h"


#define MODULE_PARA_MAX 2

static char this_mdoule_string[] = "wifi emi loader";
UINT32 gDbgLvl = DBG_LOG_LOUD;
UINT32 ModulePara[MODULE_PARA_MAX] = {0};
UINT32 ModuleParaNum = 0;
struct CORE_ADAPTER *pAd = NULL;


#ifdef CONFIG_OF
/* platform device & platform driver match name */
#define OF_WIFI_EMI_NAME_STR "mediatek,rebb-wifi-emi-loader"

static const struct of_device_id wifi_emi_of_ids[] = {
	{.compatible = OF_WIFI_EMI_NAME_STR,},
	{},
};
#endif /* CONFIG_OF */

static INT32 wifi_emi_probe(struct platform_device *pdev)
{
#ifdef CONFIG_OF
	struct device_node *node = NULL;
#endif /* CONFIG_OF */
	INT32 ret = 0;
	UINT8 step = 1, i;
	UINT32 op_mode = ASIC_MODE; //operation mode
	UINT32 is_board_v11 = FALSE; //for new D-die clock modified board

	for(i = 0; i < ModuleParaNum; i++) {
		if (i == 0) //operation mode
			op_mode = ModulePara[i];
		else if (i == 1) // debug level
			gDbgLvl = ModulePara[i];
	}

#ifdef CONFIG_OF
	node = pdev->dev.of_node;
	if (!node) {
		node = of_find_compatible_node(NULL, NULL, OF_WIFI_EMI_NAME_STR);
		if (!node) {
			DBG_ERR_FUNC("can't found node of %s from dts!\n", OF_WIFI_EMI_NAME_STR);
			ret = -ENODEV;
			goto err;
		}
	}
	of_property_read_u32_index(node, "op_mode", 0, &op_mode);
	of_property_read_u32_index(node, "board_v11", 0, &is_board_v11);
#endif /* CONFIG_OF */
	pAd = devm_kzalloc(&pdev->dev, sizeof(*pAd), GFP_KERNEL);
	if (!pAd) {
		DBG_ERR_FUNC("memory allocation failed!\n");
		ret = -ENOMEM;
		goto err;
	}

	if (hw_init(pAd, op_mode) != FUNC_STATUS_SUCCESS) {
		DBG_ERR_FUNC("hw_init failed!\n");
		goto err;
	}

	/* set priv_data to pdev */
	snprintf(pAd->name,sizeof(pAd->name),this_mdoule_string);
	platform_set_drvdata(pdev, pAd);

	if (is_emi_ready()) {
		/* set emi remapping register */
		ret = set_connsys_emi_remapping(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_connsys_emi_remapping failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not support hook for emi remapping!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. set emi remapping successfully!\n", step++);
		}

		/* assert mcu reset */
		ret = set_connsys_sw_mcu_reset(pAd, TRUE);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_connsys_sw_mcu_reset(assert) failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not support hook for de-assert mcu reset!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. assert mcu successfully!\n", step++);
		}

		/* turn on SPM clock */
		ret = do_turn_on_spm_clock(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_turn_on_spm_clock failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_INFO_FUNC("Not support hook for turn on SPM clock!\n");
			}
		} else {
			DBG_LOUD_FUNC("%d. turn on SPM clock successfully!\n", step++);
		}

		/* de-assert CONNSYS S/W reset */
		ret = set_connsys_sw_conn_reset(pAd, FALSE);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_connsys_sw_conn_reset(de-assert) failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_INFO_FUNC("Not hook for de-assert connsys reset!\n");
			}
		} else {
			DBG_LOUD_FUNC("%d. de-assert connsys successfully!\n", step++);
		}

		/* Turn off AP2CONN/CONN2AP AHB/AXI TX/RX bus sleep protect configuration */
		ret = do_ahb_axi_bus_init(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_ahb_bus_init failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_INFO_FUNC("Not hook for AHB bus initialization!\n");
			}
		} else {
			DBG_LOUD_FUNC("%d. AHB bus initialized successfully!\n", step++);
		}

		/* SPM apsrc/ddr_en hardware mask disable */
		ret = set_spm_hw_mask_disable(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_spm_hw_mask_disable failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_INFO_FUNC("Not hook for SPM hw mask control!\n");
			}
		} else {
			DBG_LOUD_FUNC("%d. SPM hardware mask disabled successfully!\n", step++);
		}

		/* wait 5ms (apply this for CONNSYS XO clock ready) */
		mdelay(5);

		/* wait 10us (apply this for CONNSYS CPU memory power-on) */
		udelay(10);

		/* check CONNSYS version read ready */
		ret = do_check_connsys_version(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_check_connsys_version failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_INFO_FUNC("Not hook for check connsys version!\n");
			}
		} else {
			DBG_LOUD_FUNC("%d. connsys version check successfully!\n", step++);
		}

		/* EMI download process */
		ret = do_connsys_emi_download(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_connsys_emi_download failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not hook for EMI download!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. emi download successfully!\n", step++);
		}

		/* EMI control setting (bypass hw mode) */
		ret = set_emi_hw_mode_bypass(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_emi_hw_mode_bypass failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not hook for EMI bypass hw mode!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. set EMI bypass hw mode successfully!\n", step++);
		}

		/* do adie co-clock init */
		if (is_board_v11) {
			ret = do_adie_coclock_init(pAd);
			if (ret != FUNC_STATUS_SUCCESS) {
				if (ret == FUNC_STATUS_FAILURE) {
					DBG_ERR_FUNC("do_adie_coclock_init failed!\n");
					goto err;
				} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
					DBG_ERR_FUNC("Not hook for do_adie_coclock_init!\n");
					goto err;
				}
			} else {
				DBG_LOUD_FUNC("%d. do adie co-clock init successfully!\n", step++);
			}
		}

		/* do rtc clock init */
		ret = do_rtc_clock_init(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_rtc_clock_init failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not hook for do_rtc_clock_init!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. do rtc clock init successfully!\n", step++);
		}

		/* de-assert mcu reset */
		ret = set_connsys_sw_mcu_reset(pAd, FALSE);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("set_connsys_sw_mcu_reset(de-assert) failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not hook for de-assert mcu reset!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d. de-assert mcu successfully!\n", step++);
		}

		/* check CONNSYS power-on completion */
		ret = do_check_connsys_complete(pAd);
		if (ret != FUNC_STATUS_SUCCESS) {
			if (ret == FUNC_STATUS_FAILURE) {
				DBG_ERR_FUNC("do_check_connsys_complete failed!\n");
				goto err;
			} else if (ret == FUNC_STATUS_NOT_SUPPORT) {
				DBG_ERR_FUNC("Not hook for CONNSYS power-on completion!\n");
				goto err;
			}
		} else {
			DBG_LOUD_FUNC("%d.check CONNSYS power-on completion successfully!\n", step++);
		}
	} else {
		DBG_ERR_FUNC("Opps! EMI isn't ready!\n");
		goto err;
	}

	pAd->init_done = TRUE;

	return ret;

err:
	ret = -1;
	if (pAd) {
		devm_kfree(&pdev->dev, pAd);
		pAd = NULL;
	}

	return ret;
}

static INT32 wifi_emi_remove(struct platform_device *pdev)
{
	if (pAd) {
		devm_kfree(&pdev->dev, pAd);
		pAd = NULL;
	}

	return 0;
}

#ifndef VXWORKS
static struct platform_driver wifi_emi_driver = {
	.probe  = wifi_emi_probe,
	.remove = wifi_emi_remove,
	.driver = {
		.name   = this_mdoule_string,
		.owner  = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = wifi_emi_of_ids,
#endif /* CONFIG_OF */
	},
};

/* don't allow module unload */
INT32 __init wifi_emi_init(void)
{
	return platform_driver_register(&wifi_emi_driver);
}

VOID __exit wifi_emi_exit(void)
{
	platform_driver_unregister(&wifi_emi_driver);
}

module_param_array(ModulePara, uint, &ModuleParaNum, 0);
MODULE_PARM_DESC(ModulePara, "A integer array: Argv 1 is operation mode; Argv2 is debug level");
module_init(wifi_emi_init);
module_exit(wifi_emi_exit);


MODULE_DESCRIPTION("Mediatek Wifi EMI Loader Driver");
MODULE_LICENSE("GPL v2");
#endif
#ifdef VXWORKS
static struct platform_device platform_dev;

INT32 wifi_emi_init(void)
{
	return wifi_emi_probe(&platform_dev);
}

VOID wifi_emi_exit(void)
{
	wifi_emi_remove(&platform_dev);
}
#endif

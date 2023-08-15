/*
 ***************************************************************************
 * MediaTek Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 1997-2017, MediaTek, Inc.
 *
 * All rights reserved. MediaTek source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek Technology, Inc. is obtained.
 ***************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MCU_EMI_OUT_NAME "mcu_emi_rom"
#define WIFI_EMI_OUT_NAME "wifi_emi_rom"
#define WIFI_EMI_RAM_ILM_OUT_NAME "wifi_emi_ram_ilm"
#define WIFI_EMI_RAM_DLM_OUT_NAME "wifi_emi_ram_dlm"
#define OUTPUT_HEADER_FILE_NAME "emi_bin2h.h"

#define DEFAULT_MCU_EMI_NAME "rom/test_mcu.bin"
#define DEFAULT_WIFI_EMI_ROM_NAME "rom/test_wifi.bin"
#define DEFAULT_WIFI_EMI_RAM_ILM_NAME "rebb/WIFI_RAM_CODE_iemi.bin"
#define DEFAULT_WIFI_EMI_RAM_DLM_NAME "rebb/WIFI_RAM_CODE_demi.bin"

int bin2h(char *infname, char *outfname, char *fw_name, const char *mode)
{
	int ret = 0;
	FILE *infile, *outfile;
	unsigned char c;
	int i = 0;

	infile = fopen(infname, "r");

	if (infile == (FILE *) NULL) {
		printf("Can't read file %s\n", infname);
		return -1;
	}

	outfile = fopen(outfname, mode);

	if (outfile == (FILE *) NULL) {
		printf("Can't open write file %s\n", outfname);
		fclose(infile);
		return -1;
	}

	fputs("/* AUTO GEN PLEASE DO NOT MODIFY IT */\n", outfile);
	fputs("\n", outfile);
	fputs("\n", outfile);
	fprintf(outfile, "UCHAR %s[] = {\n", fw_name);

	while (1) {
		char cc[3];

		c = getc(infile);

		if (feof(infile))
			break;

		memset(cc, 0, 2);

		if (i >= 16) {
			fputs("\n", outfile);
			i = 0;
		}

		if (i == 0)
			fputs("\t", outfile);
		else if (i < 16)
			fputs(" ", outfile);

		fputs("0x", outfile);
		sprintf(cc, "%02x", c);
		fputs(cc, outfile);
		fputs(",", outfile);
		i++;
	}

	fputs("\n};\n", outfile);
	fclose(infile);
	fclose(outfile);

	return 0;
}

int main(int argc, char *argv[])
{
	char *bin_in_path = NULL, *dir_out_path = NULL, *chip_name = NULL;
	char *mcu_emi_rom_filename = DEFAULT_MCU_EMI_NAME;
	char *wifi_emi_rom_filename = DEFAULT_WIFI_EMI_ROM_NAME;
	char *wifi_emi_ram_ilm_filename = DEFAULT_WIFI_EMI_RAM_ILM_NAME;
	char *wifi_emi_ram_dlm_filename = DEFAULT_WIFI_EMI_RAM_DLM_NAME;
	char mcu_emi_rom_full_path[PATH_MAX];
	char wifi_emi_rom_full_path[PATH_MAX];
	char wifi_emi_ram_ilm_full_path[PATH_MAX];
	char wifi_emi_ram_dlm_full_path[PATH_MAX];
	char output_full_path[PATH_MAX];
	int ret = 0;


	bin_in_path = (char *)getenv("BIN_IN_PATH");
	dir_out_path = (char *)getenv("DIR_OUT_PATH");
	chip_name = (char *)getenv("CHIP_NAME");

	if (!bin_in_path) {
		printf("Environment value \"BIN_IN_PATH\" not export\n");
		return -1;
	}

	if (!dir_out_path) {
		printf("Environment value \"DIR_OUT_PATH\" not export\n");
		return -1;
	}

	if (!chip_name) {
		printf("Environment value \"CHIP_NAME\" not export\n");
		return -1;
	}

	/* get full path for mcu emi rom */
	snprintf(mcu_emi_rom_full_path, sizeof(mcu_emi_rom_full_path),
		"%s/%s/%s", bin_in_path, chip_name, mcu_emi_rom_filename);

	/* get full path for wifi emi rom */
	snprintf(wifi_emi_rom_full_path, sizeof(wifi_emi_rom_full_path),
		"%s/%s/%s", bin_in_path, chip_name, wifi_emi_rom_filename);

#if defined(VXWORKS) && !defined(WIFI_RAM_EMI_SUPPORT)
	/* Already move these download to wifi driver */

	/* get full path for wifi emi ram ilm */
	snprintf(wifi_emi_ram_ilm_full_path, sizeof(wifi_emi_ram_ilm_full_path),
		"%s/%s/%s", bin_in_path, chip_name, wifi_emi_ram_ilm_filename);

	/* get full path for wifi emi ram dlm */
	snprintf(wifi_emi_ram_dlm_full_path, sizeof(wifi_emi_ram_dlm_full_path),
		"%s/%s/%s", bin_in_path, chip_name, wifi_emi_ram_dlm_filename);

#endif
	/* start to run bin to header process */
	snprintf(output_full_path, sizeof(output_full_path),
		"%s/%s", dir_out_path, OUTPUT_HEADER_FILE_NAME);

	ret = bin2h(mcu_emi_rom_full_path, output_full_path, MCU_EMI_OUT_NAME, "w");
	if (ret != 0) {
		printf("bin2h failed for %s\n",MCU_EMI_OUT_NAME);
		return -1;
	}

	ret = bin2h(wifi_emi_rom_full_path, output_full_path, WIFI_EMI_OUT_NAME, "a");
	if (ret != 0) {
		printf("bin2h failed for %s\n",WIFI_EMI_OUT_NAME);
		return -1;
	}

#if defined(VXWORKS) && !defined(WIFI_RAM_EMI_SUPPORT)

	/* Already move these download to wifi driver */

	ret = bin2h(wifi_emi_ram_ilm_full_path, output_full_path, WIFI_EMI_RAM_ILM_OUT_NAME, "a");
	if (ret != 0) {
		printf("bin2h failed for %s\n",WIFI_EMI_RAM_ILM_OUT_NAME);
		return -1;
	}

	ret = bin2h(wifi_emi_ram_dlm_full_path, output_full_path, WIFI_EMI_RAM_DLM_OUT_NAME, "a");
	if (ret != 0) {
		printf("bin2h failed for %s\n",WIFI_EMI_RAM_DLM_OUT_NAME);
		return -1;
	}
#endif
	printf("generate bin to emi header successfully!\n");
	return 0;
}


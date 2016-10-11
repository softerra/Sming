/*
 * rbootconf.c
 *
 *  Created on: Oct 11, 2016
 *      Author: alex
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "rbootconf.h"

#include "../rboot-private.h"

static void usage(const char *prog_name)
{
	printf("Use: %s [-o file] [-n node name] [-a AP name ] [-p AP password]\n", prog_name);
}

static int set_string(const char *val, char *setto, size_t max_len)
{
	size_t len = strlen(val);
	if (len > max_len) {
		return 0;
	}
	if (len > 0) {
		strcpy(setto, val);
	}
	return 1;
}

// From rboot.c
#if defined(BOOT_CONFIG_CHKSUM)
// calculate checksum for block of data
// from start up to (but excluding) end
static uint8 calc_chksum(uint8 *start, uint8 *end) {
	uint8 chksum = CHKSUM_INIT;
	while(start < end) {
		chksum ^= *start;
		start++;
	}
	return chksum;
}
#endif

static int save_config(const char *conf_file, rboot_config *conf)
{
	FILE *fp = fopen(conf_file, "w");
	if (!fp) {
		printf("Failed to open %s: %s\n", conf_file, strerror(errno));
		return -1;
	}

//TODO: ensure the same alignment!
//TODO: ensure the same endianness!

	conf->oprog.magic = OPROG_MAGIC;

	conf->magic = BOOT_CONFIG_MAGIC;
	conf->version = BOOT_CONFIG_VERSION;
	conf->count = 1;
	conf->roms[0] = SECTOR_SIZE * (BOOT_CONFIG_SECTOR + 1);
#ifdef BOOT_CONFIG_CHKSUM
	romconf->chksum = calc_chksum((uint8*)conf, (uint8*)&conf->chksum);
#endif

	if (fwrite(conf, sizeof(*conf), 1, fp) != 1) {
		printf("Failed to write to %s: %s\n", conf_file, strerror(errno));
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	int opt;
	const char *conf_file = "rboot-config.bin";

	rboot_config conf;
	memset(&conf, 0, sizeof(conf));

	while ((opt = getopt(argc, argv, "ho:n:a:p:")) != -1) {
		switch (opt) {
		case 'o':
			conf_file = optarg;
			break;
		case 'n':
			if (!set_string(optarg, conf.oprog.node_name, OPROG_NODE_NAME_SIZE_MAX - 1)) {
				return -2;
			}
			break;
		case 'a':
			if (!set_string(optarg, conf.oprog.ap_ssid, 31)) {
				return -2;
			}
			break;
		case 'p':
			if (!set_string(optarg, conf.oprog.ap_pass, OPROG_AP_PASS_SIZE_MAX - 1)) {
				return -2;
			}
			break;

		case 'h':
			usage(argv[0]);
			return 0;

		case '?':
		default:
			printf("Unknown option '%c'", optopt);
			usage(argv[0]);
			return -1;
		}
	}

	if (save_config(conf_file, &conf) != 0) {
		return -3;
	}

	return 0;
}

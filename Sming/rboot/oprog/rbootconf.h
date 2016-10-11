/*
 * rbootconf.h
 *
 *  Created on: Oct 11, 2016
 *      Author: alex
 */

#ifndef RBOOT_OPROG_RBOOTCONF_H_
#define RBOOT_OPROG_RBOOTCONF_H_

#ifdef BOOT_CONFIG_OPROG

typedef unsigned short int uint16;

#define OPROG_MAGIC					0xb6c3
#define OPROG_NODE_NAME_SIZE_MAX	32	/* =max of SSID length, see 802.11-2007, we use 31 + terminating 0 */
#define OPROG_AP_PASS_SIZE_MAX		32	/* we use 31 + terminating 0 */

typedef struct {
	uint16 magic;
	char node_name[OPROG_NODE_NAME_SIZE_MAX];
	char ap_ssid[32];
	char ap_pass[OPROG_AP_PASS_SIZE_MAX];
} rboot_config_oprog;

#endif

#endif /* RBOOT_OPROG_RBOOTCONF_H_ */

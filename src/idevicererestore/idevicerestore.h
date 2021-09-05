/*
 * idevicerestore.h
 * Restore device firmware and filesystem
 *
 * Copyright (c) 2010-2012 Martin Szulecki. All Rights Reserved.
 * Copyright (c) 2012-2015 Nikias Bassen. All Rights Reserved.
 * Copyright (c) 2010 Joshua Hill. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef IDEVICERESTORE_H
#define IDEVICERESTORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <plist/plist.h>

// the flag with value 1 is reserved for internal use only. don't use it.
#define FLAG_DEBUG           1 << 1
#define FLAG_ERASE           1 << 2
#define FLAG_CUSTOM          1 << 3
#define FLAG_EXCLUDE         1 << 4
#define FLAG_PWN             1 << 5
#define FLAG_NOACTION        1 << 6
#define FLAG_SHSHONLY        1 << 7
#define FLAG_LATEST          1 << 8
#define FLAG_RERESTORE       1 << 9
#define FLAG_UPDATE          1 << 10
#define FLAG_OTA_BBFW        1 << 11
#define FLAG_UNOFFICIAL_BBFW 1 << 12
#define FLAG_OLD_OTA_BBFW    1 << 13
#define FLAG_OTA_BLOB        1 << 14
    
struct idevicerestore_client_t;

enum {
	RESTORE_STEP_DETECT = 0,
	RESTORE_STEP_PREPARE,
	RESTORE_STEP_UPLOAD_FS,
	RESTORE_STEP_VERIFY_FS,
	RESTORE_STEP_FLASH_FW,
	RESTORE_STEP_FLASH_BB,
	RESTORE_NUM_STEPS
};

typedef void (*idevicerestore_progress_cb_t)(int step, double step_progress, void* userdata);

struct idevicerestore_client_t* idevicerestore_client_new(void);
void idevicerestore_client_free(struct idevicerestore_client_t* client);

void idevicerestore_set_ecid(struct idevicerestore_client_t* client, unsigned long long ecid);
void idevicerestore_set_udid(struct idevicerestore_client_t* client, const char* udid);
void idevicerestore_set_flags(struct idevicerestore_client_t* client, int flags);
void idevicerestore_set_ipsw(struct idevicerestore_client_t* client, const char* path);
void idevicerestore_set_cache_path(struct idevicerestore_client_t* client, const char* path);
void idevicerestore_set_progress_callback(struct idevicerestore_client_t* client, idevicerestore_progress_cb_t cbfunc, void* userdata);
void idevicerestore_set_info_stream(FILE* strm);
void idevicerestore_set_error_stream(FILE* strm);
void idevicerestore_set_debug_stream(FILE* strm);

int idevicerestore_start(struct idevicerestore_client_t* client);
const char* idevicerestore_get_error(void);

void usage(int argc, char* argv[]);
int check_mode(struct idevicerestore_client_t* client);
const char* check_hardware_model(struct idevicerestore_client_t* client);
int get_ecid(struct idevicerestore_client_t* client, uint64_t* ecid);
int is_image4_supported(struct idevicerestore_client_t* client);
int get_ap_nonce(struct idevicerestore_client_t* client, unsigned char** nonce, int* nonce_size);
int get_sep_nonce(struct idevicerestore_client_t* client, unsigned char** nonce, int* nonce_size);
int get_tss_response(struct idevicerestore_client_t* client, plist_t build_identity, plist_t* tss);
void fixup_tss(plist_t tss);
int build_manifest_get_identity_count(plist_t build_manifest);
int build_manifest_check_compatibility(plist_t build_manifest, const char* product);
void build_manifest_get_version_information(plist_t build_manifest, struct idevicerestore_client_t* client);
plist_t build_manifest_get_build_identity(plist_t build_manifest, uint32_t identity);
plist_t build_manifest_get_build_identity_for_model(plist_t build_manifest, const char *hardware_model);
plist_t build_manifest_get_build_identity_for_model_with_restore_behavior(plist_t build_manifest, const char *hardware_model, const char *behavior);
int build_manifest_get_build_count(plist_t build_manifest);
void build_identity_print_information(plist_t build_identity);
int build_identity_has_component(plist_t build_identity, const char* component);
int build_identity_get_component_path(plist_t build_identity, const char* component, char** path);
int ipsw_extract_filesystem(const char* ipsw, plist_t build_identity, char** filesystem);
int extract_component(const char* ipsw, const char* path, unsigned char** component_data, unsigned int* component_size);
int personalize_component(const char *component, const unsigned char* component_data, unsigned int component_size, plist_t tss_response, unsigned char** personalized_component, unsigned int* personalized_component_size);

const char* get_component_name(const char* filename);
    

    
#ifdef __cplusplus
}
#endif

#endif

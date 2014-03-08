// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cloud_print/service/service_switches.h"

const char kConsoleSwitch[] = "console";
const char kDeleteSwitch[] = "delete";
const char kInstallSwitch[] = "install";
const char kRequirementsSwitch[] = "requirements";
const char kServiceSwitch[] = "service";
const char kStartSwitch[] = "start";
const char kStopSwitch[] = "stop";
const char kUninstallSwitch[] = "uninstall";


asdfsadf


static int ngx_http_lua_shdict_set(lua_State *L);
static int ngx_http_lua_shdict_safe_set(lua_State *L);
static int ngx_http_lua_shdict_get(lua_State *L);
static int ngx_http_lua_shdict_expire(ngx_http_lua_shdict_ctx_t *ctx,
    ngx_uint_t n);
static ngx_int_t ngx_http_lua_shdict_lookup(ngx_shm_zone_t *shm_zone,
    ngx_uint_t hash, u_char *kdata, size_t klen,
    ngx_http_lua_shdict_node_t **sdp);
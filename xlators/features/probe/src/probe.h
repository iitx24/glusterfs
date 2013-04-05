/*
   Copyright (c) 2006-2013 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#ifndef __PROBE_H__
#define __PROBE_H__

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

typedef struct {
        gf_boolean_t encrypt_write;
	gf_boolean_t decrypt_read;
	gf_boolean_t probe_start;
	gf_boolean_t probe_end;
	char *probe_name;
	char *directory;
	char *probe_group;
	int interval_secs;
	probe_stats_t probe_stats;
} probe_private_t;

#endif /* __PROBE_H__ */

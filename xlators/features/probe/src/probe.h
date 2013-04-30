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
	char *probe_name;
	char *probe_group;
	probe_stats_t write_stats;
	probe_stats_t read_stats;
	probe_stats_t write_cbk_stats;
	probe_stats_t read_cbk_stats;
} probe_private_t;

#endif /* __PROBE_H__ */

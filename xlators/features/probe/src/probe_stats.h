/*
   Copyright (c) 2006-2012 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#ifndef __PROBE_STATUS_H__
#define __PROBE_STATUS_H__

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

typedef struct {
	probe_time_t accumulated_time;
	uint64_t count;
} probe_stats_time_accumulator_t;

typedef struct {
	probe_stats_time_accumulator_t xlator_latency;
} probe_stats_t;

int probe_stats_init( probe_stats_t *probe_stats );
int probe_stats_destroy( probe_stats_t *probe_stats );
int probe_stats_xlator_latency_add( probe_stats_t *probe_stats, probe_time_t usec_time );

#endif /* __PROBE_STATUS_H__ */

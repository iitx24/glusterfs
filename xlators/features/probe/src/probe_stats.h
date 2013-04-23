/*
   Copyright (c) 2006-2013 Red Hat, Inc. <http://www.redhat.com>
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

#include "probe_time_accumulator.h"

typedef struct {
	probe_time_accumulator_t latency;
} probe_write_stats_t;

typedef struct {
	probe_time_accumulator_t latency;
} probe_write_cbk_stats_t;

typedef struct {
	probe_write_stats_t write_stats;
	probe_write_cbk_stats_t write_cbk_stats;
} probe_stats_t;

int probe_stats_init( probe_stats_t *probe_stats );
int probe_stats_destroy( probe_stats_t *probe_stats );

#endif /* __PROBE_STATUS_H__ */

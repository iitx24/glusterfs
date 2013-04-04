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
	probe_usec_t accumulated_time;
	uint64_t count;
} probe_stats_time_accumulator_t;

typedef struct {
	probe_stats_time_accumulator_t xlator_latency;
} probe_stats_t;


#endif /* __PROBE_STATUS_H__ */

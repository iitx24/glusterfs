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
	char *stat_name;
	char *probe_dict_key;
} probe_stats_t;
	
/*
 * Example of a new type of stats
 *
typdef struct {
	probe_stats_t super;
	int other_data;
} probe_new_stats_t;
*/

int32_t probe_stats_init( probe_stats_t *self,
		const char *stat_name,
		const char *probe_dict_key );
int probe_stats_destroy( probe_stats_t *self );
int32_t probe_stats_dump_write( probe_stats_t *self );
int32_t probe_stats_record_time( probe_stats_t *self, dict_t *xdata);

#endif /* __PROBE_STATUS_H__ */

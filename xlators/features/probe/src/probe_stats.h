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

typedef enum {
	PROBE_WRITEV_STATS  = 0,
	PROBE_WRITEV_CBK_STATS,

	/**
	 * Must be last
	 */
	PROBE_NUM_STATS_TYPES
} probe_stats_type_t;

typedef struct {
	probe_time_t accumulated_time;
	uint64_t count;
} probe_stats_time_accumulator_t;

typedef struct {
	probe_stats_time_accumulator_t xlator_latency;
} probe_stats_type_info_t;

typedef struct {
	probe_stats_time_accumulator_t latency;
} probe_write_stats_t;

typedef struct {
	probe_stats_time_accumulator_t latency;
} probe_write_cbk_stats_t;

typedef struct {
	probe_stats_type_info_t ops_stats[PROBE_NUM_STATS_TYPES];
	probe_write_stats_t write_stats;
	probe_write_cbk_stats_t write_cbk_stats;
} probe_stats_t;

int probe_stats_init( probe_stats_t *probe_stats );
int probe_stats_destroy( probe_stats_t *probe_stats );

int probe_stats_xlator_latency_add( probe_stats_t *probe_stats, 
		probe_stats_type_t stats_type,
		probe_time_t usec_time );
probe_time_t probe_stats_xlator_latency( probe_stats_t *probe_stats, 
		probe_stats_type_t stats_type);







int32_t 
probe_stats_time_accumulator_sample( probe_stats_time_accumulator_t *self,
		dict_t *xdata,
		char *dict_key);
probe_time_t
probe_stats_time_accumulator_latency( probe_stats_time_accumulator_t *self);
int32_t 
probe_stats_time_accumulator_add( probe_stats_time_accumulator_t *self,
		probe_time_t usec_time);
int32_t 
probe_stats_time_accumulator_destroy( probe_stats_time_accumulator_t *self );
int32_t 
probe_stats_time_accumulator_init( probe_stats_time_accumulator_t *self );

#endif /* __PROBE_STATUS_H__ */

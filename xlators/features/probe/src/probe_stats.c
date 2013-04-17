/*
   Copyright (c) 2006-2013 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#include <ctype.h>
#include <sys/uio.h>

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#include "glusterfs.h"
#include "xlator.h"
#include "logging.h"

#include "probe_time.h"
#include "probe_stats.h"


int32_t 
probe_stats_time_accumulator_init( probe_stats_time_accumulator_t *self )
{
	self->accumulated_time = 0;
	self->count = 0;

	return 0;
}

int32_t 
probe_stats_time_accumulator_destroy( probe_stats_time_accumulator_t *self )
{
	return 0;
}

int32_t 
probe_stats_time_accumulator_add( probe_stats_time_accumulator_t *self,
		probe_time_t usec_time)
{
	self->accumulated_time += usec_time;
	self->count++;
	return 0;
}

int32_t 
probe_stats_time_accumulator_sample( probe_stats_time_accumulator_t *self,
		dict_t *xdata,
		char *dict_key)
{
	data_t *data = NULL;
	probe_time_t current_time;
	probe_time_t dict_time;

	if (NULL == xdata) {
		goto xdata_null;
	}

	current_time = probe_time_gettime();
	data = dict_get(xdata, dict_key);
	if (NULL == data) {
		dict_set(xdata, dict_key, data_from_uint64(current_time));
	} else if (0 == gf_string2uint64(data->data, &dict_time)) {
			probe_stats_time_accumulator_add(self,
					probe_time_elapsed(dict_time, current_time));
	}

xdata_null:
	return 0;
}

probe_time_t
probe_stats_time_accumulator_latency( probe_stats_time_accumulator_t *self)
{
	if (0 != self->count) {
		return self->accumulated_time / self->count;
	} else {
		return 0;
	}
}


int
probe_stats_init( probe_stats_t *probe_stats )
{
	memset(probe_stats, 0, sizeof(probe_stats_t));
	probe_stats_time_accumulator_init(&probe_stats->write_stats.latency);
	probe_stats_time_accumulator_init(&probe_stats->write_cbk_stats.latency);

	return 0;
}

int
probe_stats_destroy( probe_stats_t *probe_stats )
{
	return 0;
}

int
probe_stats_xlator_latency_add( probe_stats_t *probe_stats, 
		probe_stats_type_t stats_type,
		probe_time_t usec_time )
{
	/* assert here */
	probe_stats_type_info_t *stats;
	
	stats = &probe_stats->ops_stats[ stats_type ];
	probe_stats_time_accumulator_add(&stats->xlator_latency, usec_time);
	return 0;
}

probe_time_t
probe_stats_xlator_latency( probe_stats_t *probe_stats, 
		probe_stats_type_t stats_type) 
{
	/* assert here */
	probe_stats_type_info_t *stats;
	
	stats = &probe_stats->ops_stats[ stats_type ];

	return probe_stats_time_accumulator_latency(&stats->xlator_latency);
}


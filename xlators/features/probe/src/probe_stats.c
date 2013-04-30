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
#include "statedump.h"

#include "probe_time.h"
#include "probe_stats.h"

int32_t 
probe_stats_init( probe_stats_t *self,
		const char *stat_name,
		const char *probe_dict_key )
{
	memset(self, 0, sizeof(probe_stats_t));
	self->stat_name = gf_strdup(stat_name);
	GF_ASSERT(self->stat_name != NULL);
	self->probe_dict_key = gf_strdup(probe_dict_key);
	GF_ASSERT(self->probe_dict_key != NULL);
	return 0;
}

int
probe_stats_destroy( probe_stats_t *self )
{
	if (NULL != self->stat_name) {
		GF_FREE(self->stat_name);
		self->stat_name = NULL;
	}

	if (NULL != self->probe_dict_key) {
		GF_FREE(self->probe_dict_key);
		self->probe_dict_key = NULL;
	}

	return 0;
}

int32_t
probe_stats_dump_write( probe_stats_t *self )
{
        char  buf[GF_DUMP_MAX_BUF_LEN];

	snprintf(buf, GF_DUMP_MAX_BUF_LEN, "%s latency",
			self->stat_name);
	gf_proc_dump_write(buf, "%"PRIu64"us", 
			probe_time_accumulator_latency(&self->latency));
	return 0;
}

int32_t 
probe_stats_record_time( probe_stats_t *self, dict_t *xdata)
{
	data_t *data = NULL;
	probe_time_t current_time;
	probe_time_t dict_time;

	if (NULL == xdata) {
		goto xdata_null;
	}

	current_time = probe_time_gettime();
	data = dict_get(xdata, self->probe_dict_key);
	if (NULL == data) {
		dict_set(xdata, self->probe_dict_key, data_from_uint64(current_time));
	} else if (0 == gf_string2uint64(data->data, &dict_time)) {
			probe_time_accumulator_add(&self->latency,
					probe_time_elapsed(dict_time, current_time));
	}

xdata_null:
	return 0;
}

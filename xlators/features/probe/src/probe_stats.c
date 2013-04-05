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

int
probe_stats_init( probe_stats_t *probe_stats )
{
	memset(probe_stats, 0, sizeof(probe_stats_t));
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
	stats->xlator_latency.accumulated_time += usec_time;
	stats->xlator_latency.count++;

	return 0;
}

probe_time_t
probe_stats_xlator_latency( probe_stats_t *probe_stats, 
		probe_stats_type_t stats_type) 
{
	/* assert here */
	probe_stats_type_info_t *stats;
	
	stats = &probe_stats->ops_stats[ stats_type ];

	if (0 != stats->xlator_latency.count) {
		return stats->xlator_latency.accumulated_time / stats->xlator_latency.count;
	} else {
		return 0;
	}
}


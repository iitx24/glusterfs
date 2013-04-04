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

static probe_usec_t
probe_time_to_usec(const probe_time_t *probe_time) 
{
	probe_usec_t sec_to_usec;
	probe_usec_t nsec_to_usec;

	sec_to_usec = probe_time->linux_time.tv_sec * 1000000LL;
	nsec_to_usec = probe_time->linux_time.tv_nsec / 1000LL;

	return sec_to_usec + nsec_to_usec;
}

int
probe_time_gettime(probe_time_t *time_now) 
{
	return clock_gettime(CLOCK_MONOTONIC, &time_now->linux_time); 
}

probe_usec_t
probe_time_elapsed(const probe_time_t *start_time,
		const probe_time_t *end_time)
{
	return probe_time_to_usec(end_time) - probe_time_to_usec(start_time);
}


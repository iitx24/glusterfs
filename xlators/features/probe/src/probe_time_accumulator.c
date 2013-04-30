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
#include "probe_time_accumulator.h"

int32_t 
probe_time_accumulator_init( probe_time_accumulator_t *self )
{
	self->accumulated_time = 0;
	self->count = 0;

	return 0;
}

int32_t 
probe_time_accumulator_destroy( probe_time_accumulator_t *self )
{
	return 0;
}

int32_t 
probe_time_accumulator_add( probe_time_accumulator_t *self,
		probe_time_t usec_time)
{
	self->accumulated_time += usec_time;
	self->count++;
	return 0;
}


probe_time_t
probe_time_accumulator_latency( probe_time_accumulator_t *self)
{
	if (0 != self->count) {
		return self->accumulated_time / self->count;
	} else {
		return 0;
	}
}



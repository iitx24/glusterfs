/*
   Copyright (c) 2006-2013 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#ifndef __PROBE_TIME_ACCUMULATOR_H__
#define __PROBE_TIME_ACCUMULATOR_H__

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

typedef struct {
	probe_time_t accumulated_time;
	uint64_t count;
} probe_time_accumulator_t;

int32_t 
probe_time_accumulator_record( probe_time_accumulator_t *self,
		dict_t *xdata,
		char *dict_key);
probe_time_t
probe_time_accumulator_latency( probe_time_accumulator_t *self);

int32_t 
probe_time_accumulator_add( probe_time_accumulator_t *self,
		probe_time_t usec_time);
int32_t 
probe_time_accumulator_destroy( probe_time_accumulator_t *self );
int32_t 
probe_time_accumulator_init( probe_time_accumulator_t *self );

#endif /* __PROBE_TIME_ACCUMULATOR_H__ */

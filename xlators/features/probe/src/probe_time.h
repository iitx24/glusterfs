/*
   Copyright (c) 2006-2013 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#ifndef __PROBE_TIME_H__
#define __PROBE_TIME_H__

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

/* Check for Linux - this may not work on other OSs */
#include <time.h>
	
/** Time in microseconds */
typedef uint64_t probe_time_t;

probe_time_t probe_time_gettime( void );

static inline probe_time_t
probe_time_elapsed(probe_time_t start_time, probe_time_t end_time)
{
	return end_time - start_time;
}

#endif /* __PROBE_TIME_H__ */

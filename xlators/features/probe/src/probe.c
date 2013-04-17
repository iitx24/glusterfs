/*
   Copyright (c) 2006-2012 Red Hat, Inc. <http://www.redhat.com>
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
#include "probe.h"


int32_t
probe_readv_cbk (call_frame_t *frame,
                 void *cookie,
                 xlator_t *this,
                 int32_t op_ret,
                 int32_t op_errno,
                 struct iovec *vector,
                 int32_t count,
		 struct iatt *stbuf,
                 struct iobref *iobref, dict_t *xdata)
{


	STACK_UNWIND_STRICT (readv, frame, op_ret, op_errno, vector, count,
                             stbuf, iobref, xdata);
	return 0;
}

int32_t
probe_readv (call_frame_t *frame,
             xlator_t *this,
             fd_t *fd,
             size_t size,
             off_t offset, uint32_t flags, dict_t *xdata)
{
	STACK_WIND (frame,
		    probe_readv_cbk,
		    FIRST_CHILD (this),
		    FIRST_CHILD (this)->fops->readv,
		    fd, size, offset, flags, xdata);
	return 0;
}

int32_t
probe_writev_cbk (call_frame_t *frame,
                  void *cookie,
                  xlator_t *this,
                  int32_t op_ret,
                  int32_t op_errno,
                  struct iatt *prebuf,
		  struct iatt *postbuf, dict_t *xdata)
{
	probe_private_t *priv = (probe_private_t *)this->private;
	gf_boolean_t need_unref = 0;

	if (NULL == xdata) {
		xdata = dict_new();
		if (NULL == xdata) {
			goto xdata_null;
		}
		need_unref = 1;
	}

	probe_stats_time_accumulator_sample(&priv->probe_stats.write_cbk_stats.latency,
			xdata,
			"probe-unwind");

xdata_null:
	STACK_UNWIND_STRICT (writev, frame, op_ret, op_errno, prebuf, postbuf,
                             xdata);
	if ( (NULL != xdata) && (need_unref)) {
		dict_unref(xdata);
	}
	return 0;
}

int32_t
probe_writev (call_frame_t *frame,
              xlator_t *this,
              fd_t *fd,
              struct iovec *vector,
              int32_t count,
              off_t offset, uint32_t flags,
              struct iobref *iobref, dict_t *xdata)
{
	probe_private_t *priv = (probe_private_t *)this->private;
	gf_boolean_t need_unref = 0;

	if (NULL == xdata) {
		xdata = dict_new();
		if (NULL == xdata) {
			goto xdata_null;
		}
		need_unref = 1;
	}

	probe_stats_time_accumulator_sample(&priv->probe_stats.write_stats.latency,
			xdata,
			"probe-wind");

xdata_null:
	STACK_WIND (frame,
		    probe_writev_cbk,
		    FIRST_CHILD (this),
		    FIRST_CHILD (this)->fops->writev,
		    fd, vector, count, offset, flags,
                    iobref, xdata);

	if ( (NULL != xdata) && (need_unref)) {
		dict_unref(xdata);
	}
	return 0;
}

int32_t
init (xlator_t *this)
{
	probe_private_t *priv = NULL;

	/*
	 * No need to check for children or parents
	 */

	/*
	 * Allocate private data for this object
	 */
	priv = GF_CALLOC (sizeof (probe_private_t), 1, 0);
        if (!priv) {
                return -1;
	}

	/*
	 * Initialize private data to default values
	 */
	priv->probe_start = _gf_false;
	priv->probe_end = _gf_false;
	probe_stats_init(&priv->probe_stats);

	/*
	 * Get options from volfile
	 */
        if (0 != dict_get_str (this->options, "probe-name", &priv->probe_name)) {
		priv->probe_name = "NONAME";
        }
	gf_log ("probe", GF_LOG_DEBUG, "probe-name is %s", priv->probe_name);

        if (0 != dict_get_str (this->options, "probe-group", &priv->probe_group)){
		priv->probe_group = "NOGROUP";
        }
	gf_log ("probe", GF_LOG_DEBUG, "probe-group is %s", priv->probe_group);

	/*
	 * Save private state 
	 */
	this->private = priv;

	/*
	 * Debug
	 */
	gf_log ("probe", GF_LOG_DEBUG, "probe xlator loaded");
	return 0;
}

void
fini (xlator_t *this)
{
	probe_private_t *priv = this->private;

#if 0
	gf_log ("probe", GF_LOG_DEBUG, "%s: %"PRId64"us", 
			priv->probe_name,
			probe_stats_xlator_latency(&priv->probe_stats,
				PROBE_WRITEV_CBK_STATS));

	gf_log ("probe", GF_LOG_DEBUG, "%s: %"PRId64"us", 
			priv->probe_name,
			probe_stats_xlator_latency(&priv->probe_stats,
				PROBE_WRITEV_STATS));
#endif

	/*
	 * Destroy stats object first
	 */
	probe_stats_destroy(&priv->probe_stats);

	/*
	 * Destroy private state
	 */
        if (NULL != priv) {
		this->private = NULL;
		GF_FREE (priv);
	}

	return;
}

int32_t
probe_priv (xlator_t *this)
{
        probe_private_t *priv = NULL;
        char  buf[GF_DUMP_MAX_BUF_LEN];

	if (NULL == this) {
		goto exit;
	}
        priv = this->private;
	if (NULL == priv) {
		goto exit;
	}

        snprintf(buf, GF_DUMP_MAX_BUF_LEN, "%s.%s.%s.%s", 
			this->type,
                 	this->name,
			priv->probe_group,
			priv->probe_name);
        gf_proc_dump_add_section(buf);

	gf_proc_dump_write("writev_latency", "%"PRIu64"us", 
			probe_stats_time_accumulator_latency(&priv->probe_stats.write_stats.latency));
	gf_proc_dump_write("writev_cbk_latency", "%"PRIu64"us", 
			probe_stats_time_accumulator_latency(&priv->probe_stats.write_cbk_stats.latency));

exit:
	return 0;
}

/*
 * Dump information 
 */
struct xlator_dumpops dumpops = {
        .priv    = probe_priv
};

/*
 * Set methods handled by this translator
 */
struct xlator_fops fops = {
	.readv        = probe_readv,
	.writev       = probe_writev
};

/*
 * Still a mistery 
 */
struct xlator_cbks cbks;

/*
 * Volfile options
 */
struct volume_options options[] = {
	{ .key = {"probe-name"},
	  .type = GF_OPTION_TYPE_STR
	},
	{ .key = {"probe-group"},
	  .type = GF_OPTION_TYPE_STR
	},
	{ .key  = {NULL} },
};

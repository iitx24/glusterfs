
#include "logging.h"
#include "xlator.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <inttypes.h>

#include <cmockery/cmockery.h>

int _gf_log (const char *domain, const char *file,
             const char *function, int32_t line, gf_loglevel_t level,
             const char *fmt, ...)
{
    return 0;
}

int _gf_log_callingfn (const char *domain, const char *file,
                       const char *function, int32_t line, gf_loglevel_t level,
                       const char *fmt, ...)
{
    return 0;
}

int _gf_log_nomem (const char *domain, const char *file,
                   const char *function, int line, gf_loglevel_t level,
                   size_t size)
{
    return 0;
}

xlator_t **__glusterfs_this_location () { return NULL; }

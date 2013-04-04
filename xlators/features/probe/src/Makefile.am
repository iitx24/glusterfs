xlator_LTLIBRARIES = probe.la
xlatordir = $(libdir)/glusterfs/$(PACKAGE_VERSION)/xlator/features

probe_la_LDFLAGS = -module -avoid-version

probe_la_SOURCES = probe.c probe_time.c probe_stats.c
probe_la_LIBADD = $(top_builddir)/libglusterfs/src/libglusterfs.la 

noinst_HEADERS = probe.h probe_time.h probe_stats.h

AM_CPPFLAGS = $(GF_CPPFLAGS) -I$(top_srcdir)/libglusterfs/src

AM_CFLAGS = -Wall $(GF_CFLAGS)

CLEANFILES = 

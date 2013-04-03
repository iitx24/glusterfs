xlator_LTLIBRARIES = nullx.la
xlatordir = $(libdir)/glusterfs/$(PACKAGE_VERSION)/xlator/encryption

nullx_la_LDFLAGS = -module -avoid-version

nullx_la_SOURCES = nullx.c
nullx_la_LIBADD = $(top_builddir)/libglusterfs/src/libglusterfs.la 

noinst_HEADERS = nullx.h

AM_CPPFLAGS = $(GF_CPPFLAGS) -I$(top_srcdir)/libglusterfs/src

AM_CFLAGS = -Wall $(GF_CFLAGS)

CLEANFILES = 

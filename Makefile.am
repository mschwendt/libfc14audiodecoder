AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = m4 images HISTORY.md

SUBDIRS = src

dist-hook:
		@RM@ -f $(distdir)/src/Config.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfc14audiodecoder.pc

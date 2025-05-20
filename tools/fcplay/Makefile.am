bin_PROGRAMS = fcplay
noinst_HEADERS = fcplay.h

fcdecincludedir = $(includedir)

fcplay_LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections -Wl,-s

fcplay_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_srcdir)/src -fdata-sections -ffunction-sections
fcplay_LDADD = $(top_srcdir)/src/libfc14audiodecoder.la -lasound

fcplay_SOURCES = fcplay.cpp


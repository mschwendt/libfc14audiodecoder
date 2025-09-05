EXTRA_DIST = Config.h.in

DISTCLEANFILES = Config.h

fcdecincludedir = $(includedir)
fcdecinclude_HEADERS = fc14audiodecoder.h

lib_LTLIBRARIES = libfc14audiodecoder.la

libfc14audiodecoder_la_LDFLAGS = -version-info @LIBVERSION@ -no-undefined

libfc14audiodecoder_la_SOURCES = \
fc14audiodecoder.h \
fc14audiodecoder.cpp \
Analyze.cpp \
Analyze.h \
Player.cpp \
Config.h \
COSO.cpp \
Debug.h \
Dump.h \
Dump.cpp \
Envelope.cpp \
FC.cpp \
FC.h \
Instrument.cpp \
MCMD.cpp \
ModPack.cpp \
Paula.h \
Paula.cpp \
Portamento.cpp \
Probe.cpp \
LamePaulaVoice.h \
LamePaulaMixer.h \
LamePaulaVoice.cpp \
LamePaulaMixer.cpp \
LiveFix.h \
MyEndian.h \
MyTypes.h \
SmartPtr.h \
SMOD.cpp \
TFMX.cpp \
TFMX7V.cpp \
TraitsByChecksum.cpp \
Vibrato.cpp

AM_CFLAGS = -W -Wall

## Cygwin
if HAVE_CYGWIN
AM_CFLAGS += -DWIN32
AM_LDFLAGS = -mwindows
endif # Cygwin

## MinGW32
if HAVE_MINGW32
AM_LDFLAGS = -mwindows
endif # MinGW32

noinst_LTLIBRARIES = libltdl.la

libltdl_la_SOURCES = \
	ltdl.c
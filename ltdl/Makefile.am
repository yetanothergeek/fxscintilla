AM_CFLAGS = -W -Wall

## No need for ltdl with Fox 1.1
if HAVE_FOX_1_0

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
endif # HAVE_FOX_1_0
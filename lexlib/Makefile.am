INCLUDES = -I$(top_srcdir)/include
AM_CXXFLAGS = -W -Wall -DFOX

## Cygwin
if HAVE_CYGWIN
AM_CXXFLAGS += -DWIN32
AM_LDFLAGS = -mwindows
endif # Cygwin

## MinGW32
if HAVE_MINGW32
AM_LDFLAGS = -mwindows
endif # MinGW32

noinst_LTLIBRARIES = liblexlib.la

liblexlib_la_SOURCES = \
  StyleContext.cxx

noinst_HEADERS = \
  Accessor.h \
  CharacterSet.h \
  PropSetSimple.h \
  StyleContext.h


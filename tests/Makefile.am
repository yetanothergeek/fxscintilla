INCLUDES = -I$(top_srcdir)/include $(FOX_CFLAGS)
LDADD = $(top_builddir)/fox/libfxscintilla.la $(FOX_LIBS) -L$(prefix)/lib


AM_CXXFLAGS = $(PTHREAD_CFLAGS) -W -Wall -DFOX -DSCI_LEXER

## Cygwin
if HAVE_CYGWIN
AM_CXXFLAGS += -DWIN32
AM_LDFLAGS = -mwindows
endif # Cygwin

## MinGW32
if HAVE_MINGW32
AM_LDFLAGS = -mwindows
endif # MinGW32

noinst_PROGRAMS = test

test_SOURCES = test.cpp

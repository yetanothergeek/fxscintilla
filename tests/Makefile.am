INCLUDES = -I$(top_srcdir)/include -I$(FOX_INCLUDE_DIR)
LDADD = -L$(FOX_LIB_DIR) $(top_builddir)/src/libfxscintilla.la -lFOX
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

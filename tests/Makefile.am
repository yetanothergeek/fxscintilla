INCLUDES = -I$(top_srcdir)/include -I$(FOX_INCLUDE_DIR)
LDADD = -L$(FOX_LIB_DIR) -lFOX $(top_builddir)/src/libfxscintilla.la
AM_CXXFLAGS = $(PTHREAD_CFLAGS) -W -Wall -DFOX -DSCI_LEXER

noinst_PROGRAMS = test

test_SOURCES = test.cpp

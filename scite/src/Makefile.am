scitedir = $(datadir)/scite

INCLUDES = -I../../scintilla/include
AM_CXXFLAGS = @PTHREAD_CFLAGS@ -W -Wall -DFOX -DSCI_LEXER -DSYSCONF_PATH=\"$(scitedir)\"

noinst_LTLIBRARIES = libscite.la

libscite_la_SOURCES = \
	SciTEBase.cxx \
	SciTEBuffers.cxx \
	SciTEIO.cxx \
	Exporters.cxx \
	SciTEProps.cxx

scite_DATA = *.properties

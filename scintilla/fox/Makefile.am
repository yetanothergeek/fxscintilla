INCLUDES = -I../include -I../src
AM_CXXFLAGS = -W -Wall -DFOX -DSCI_LEXER

lib_LTLIBRARIES = libfxscintilla.la
pkginclude_HEADERS = FXScintilla.h

libfxscintilla_la_SOURCES = \
	PlatFOX.cxx \
	ScintillaFOX.cxx

libfxscintilla_la_LIBADD = ../src/libscintilla.la
libfxscintilla_la_LDFLAGS = -version-info @LT_FXSCINTILLA_CURRENT@:@LT_FXSCINTILLA_REVISION@:@LT_FXSCINTILLA_AGE@

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/scintilla/include -I$(top_srcdir)/scintilla/src
AM_CXXFLAGS = -W -Wall -DFOX

lib_LTLIBRARIES = libfxscintillanolexer.la libfxscintilla.la

libfxscintillanolexer_la_SOURCES = \
	PlatFOX.cxx \
	ScintillaFOX.cxx

libfxscintilla_la_SOURCES = \
	$(libfxscintillanolexer_la_SOURCES)

libfxscintillanolexer_la_LIBADD = $(top_builddir)/scintilla/src/libscintillanolexer.la
libfxscintilla_la_LIBADD = $(top_builddir)/scintilla/src/libscintilla.la

libfxscintillanolexer_la_LDFLAGS = \
	-version-info $(LT_FXSCINTILLA_CURRENT):$(LT_FXSCINTILLA_REVISION):$(LT_FXSCINTILLA_AGE)
libfxscintilla_la_LDFLAGS = $(libfxscintillanolexer_la_LDFLAGS)

libfxscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

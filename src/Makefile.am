INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/scintilla/include \
	-I$(top_srcdir)/scintilla/src \
	-I$(FOX_INCLUDE_DIR)

AM_CXXFLAGS = -W -Wall -DFOX

libfxscintilla_la_SOURCES = \
	PlatFOX.cxx \
	ScintillaFOX.cxx

libfxscintilla_la_LIBADD = $(top_builddir)/scintilla/src/libscintilla.la

libfxscintilla_la_LDFLAGS = \
	-version-info $(LT_FXSCINTILLA_CURRENT):$(LT_FXSCINTILLA_REVISION):$(LT_FXSCINTILLA_AGE)

libfxscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

## NoLexer enabled
if NOLEXER

libfxscintillanolexer = libfxscintillanolexer.la

libfxscintillanolexer_la_SOURCES = \
	PlatFOX.cxx \
	ScintillaFOX.cxx

libfxscintillanolexer_la_LIBADD = $(top_builddir)/scintilla/src/libscintillanolexer.la

libfxscintillanolexer_la_LDFLAGS = \
	-version-info $(LT_FXSCINTILLANOLEXER_CURRENT):$(LT_FXSCINTILLANOLEXER_REVISION):$(LT_FXSCINTILLANOLEXER_AGE)

endif # NOLEXER

lib_LTLIBRARIES = libfxscintilla.la $(libfxscintillanolexer)
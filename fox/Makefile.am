AM_CPPFLAGS = \
  -I$(top_srcdir)/include \
  -I$(top_srcdir)/src \
  -I$(top_srcdir)/lexlib \
  $(FOX_CFLAGS)

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

lib_LTLIBRARIES = libfxscintilla.la

libfxscintilla_la_SOURCES = \
  PlatFOX.cxx \
  ScintillaFOX.cxx

libfxscintilla_la_LIBADD = \
  $(top_builddir)/src/libscintilla.la

libfxscintilla_la_LDFLAGS = -version-info \
  $(LT_FXSCINTILLA_CURRENT):$(LT_FXSCINTILLA_REVISION):$(LT_FXSCINTILLA_AGE)

libfxscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

## NoLexer enabled
if NOLEXER

lib_LTLIBRARIES += libfxscintillanolexer.la

libfxscintillanolexer_la_SOURCES = \
  PlatFOX.cxx \
  ScintillaFOX.cxx

libfxscintillanolexer_la_LIBADD = $(top_builddir)/src/libscintillanolexer.la

libfxscintillanolexer_la_LDFLAGS = -version-info \
  $(LT_FXSCINTILLANOLEXER_CURRENT):$(LT_FXSCINTILLANOLEXER_REVISION):$(LT_FXSCINTILLANOLEXER_AGE)

endif # NOLEXER


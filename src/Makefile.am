AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/lexlib
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

noinst_LTLIBRARIES = libscintilla.la

libscintilla_la_SOURCES = \
  CharClassify.cxx \
  Document.cxx \
  CallTip.cxx \
  Catalogue.cxx \
  ScintillaBase.cxx \
  ContractionState.cxx \
  Editor.cxx \
  KeyMap.cxx \
  LineMarker.cxx \
  CellBuffer.cxx \
  ViewStyle.cxx \
  RESearch.cxx \
  Style.cxx \
  Indicator.cxx \
  AutoComplete.cxx \
  XPM.cxx \
  Decoration.cxx \
  PositionCache.cxx \
  RunStyles.cxx \
  UniConversion.cxx \
  ExternalLexer.cxx \
  PerLine.cxx \
  Selection.cxx \
  CaseConvert.cxx \
  CaseFolder.cxx


libscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

libscintilla_la_LIBADD = \
  $(top_builddir)/lexers/liblexers.la \
  $(top_builddir)/lexlib/liblexlib.la

## NoLexer enabled
if NOLEXER

noinst_LTLIBRARIES += libscintillanolexer.la

libscintillanolexer_la_SOURCES = \
  CharClassify.cxx \
  Document.cxx \
  CallTip.cxx \
  ScintillaBase.cxx \
  ContractionState.cxx \
  Editor.cxx \
  KeyMap.cxx \
  LineMarker.cxx \
  CellBuffer.cxx \
  ViewStyle.cxx \
  RESearch.cxx \
  Style.cxx \
  Indicator.cxx \
  AutoComplete.cxx \
  XPM.cxx

endif # NOLEXER


noinst_HEADERS = \
  AutoComplete.h \
  CallTip.h \
  CellBuffer.h \
  Catalogue.h \
  CharClassify.h \
  ContractionState.h \
  Decoration.h \
  Document.h \
  Editor.h \
  ExternalLexer.h \
  Indicator.h \
  KeyMap.h \
  LineMarker.h \
  Partitioning.h \
  PerLine.h \
  PositionCache.h \
  RESearch.h \
  RunStyles.h \
  ScintillaBase.h \
  SplitVector.h \
  Style.h \
  UniConversion.h \
  ViewStyle.h \
  XPM.h \
  Selection.h \
  CaseConvert.h \
  CaseFolder.h \
  UnicodeFromUTF8.h



AM_CPPFLAGS = -I$(top_srcdir)/include
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
  Accessor.cxx \
  CharacterCategory.cxx \
  CharacterSet.cxx \
  LexerBase.cxx \
  LexerModule.cxx \
  LexerNoExceptions.cxx \
  LexerSimple.cxx \
  PropSetSimple.cxx \
  StyleContext.cxx \
  WordList.cxx

noinst_HEADERS = \
  Accessor.h \
  CharacterCategory.h \
  CharacterSet.h \
  LexAccessor.h \
  LexerBase.h \
  LexerModule.h \
  LexerNoExceptions.h \
  LexerSimple.h \
  OptionSet.h \
  PropSetSimple.h \
  SparseState.h \
  StyleContext.h \
  SubStyles.h \
  WordList.h





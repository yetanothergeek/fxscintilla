INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/scintilla/include
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
	DocumentAccessor.cxx \
	WindowAccessor.cxx \
	StyleContext.cxx \
	Document.cxx \
	CallTip.cxx \
	ScintillaBase.cxx \
	ContractionState.cxx \
	Editor.cxx \
	PropSet.cxx \
	KeyMap.cxx \
	LineMarker.cxx \
	CellBuffer.cxx \
	ViewStyle.cxx \
	RESearch.cxx \
	Style.cxx \
	Indicator.cxx \
	AutoComplete.cxx \
	XPM.cxx \
  \
	ExternalLexer.cxx \
	KeyWords.cxx \
	LexAda.cxx \
	LexAsm.cxx \
	LexAVE.cxx \
	LexBaan.cxx \
	LexBullant.cxx \
	LexCLW.cxx \
	LexConf.cxx \
	LexCPP.cxx \
	LexCrontab.cxx \
	LexCSS.cxx \
	LexEiffel.cxx \
	LexErlang.cxx \
	LexEScript.cxx \
	LexForth.cxx \
	LexFortran.cxx \
	LexHTML.cxx \
	LexLisp.cxx \
	LexLout.cxx \
	LexLua.cxx \
	LexMatlab.cxx \
	LexMetapost.cxx \
	LexMMIXAL.cxx \
	LexMPT.cxx \
	LexNsis.cxx \
	LexOthers.cxx \
	LexPascal.cxx \
	LexPB.cxx \
	LexPerl.cxx \
	LexPOV.cxx \
	LexPS.cxx \
	LexPython.cxx \
	LexRuby.cxx \
	LexScriptol.cxx \
	LexSQL.cxx \
	LexTeX.cxx \
	LexVB.cxx \
	LexYAML.cxx

libscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

## NoLexer enabled
if NOLEXER

noinst_LTLIBRARIES += libscintillanolexer.la

libscintillanolexer_la_SOURCES = \
	DocumentAccessor.cxx \
	WindowAccessor.cxx \
	StyleContext.cxx \
	Document.cxx \
	CallTip.cxx \
	ScintillaBase.cxx \
	ContractionState.cxx \
	Editor.cxx \
	PropSet.cxx \
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

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/scintilla/include
AM_CXXFLAGS = -W -Wall -DFOX

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
  \
	KeyWords.cxx \
	LexAda.cxx \
	LexAVE.cxx \
	LexBaan.cxx \
	LexBullant.cxx \
	LexCPP.cxx \
	LexConf.cxx \
	LexCrontab.cxx \
	LexEiffel.cxx \
	LexHTML.cxx \
	LexLisp.cxx \
	LexLua.cxx \
	LexMatlab.cxx \
	LexOthers.cxx \
	LexPascal.cxx \
	LexPerl.cxx \
	LexPython.cxx \
	LexRuby.cxx \
	LexSQL.cxx \
	LexVB.cxx

libscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

## NoLexer enabled
if NOLEXER

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
	AutoComplete.cxx

libscintillanolexer = libscintillanolexer.la

endif # NOLEXER

noinst_LTLIBRARIES = libscintilla.la $(libscintillanolexer)

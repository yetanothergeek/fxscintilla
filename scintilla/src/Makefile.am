INCLUDES = -I../include
AM_CXXFLAGS = -W -Wall -DFOX -DSCI_LEXER

noinst_LTLIBRARIES = libscintilla.la
lib_LTLIBRARIES = libscilexers.la

libscintilla_la_SOURCES = \
	DocumentAccessor.cxx \
	WindowAccessor.cxx \
	KeyWords.cxx \
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

libscilexers_la_SOURCES = \
	LexAda.cxx \
	LexAVE.cxx \
	LexBaan.cxx \
	LexBullant.cxx \
	LexConf.cxx \
	LexCPP.cxx \
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

libscilexers_la_LDFLAGS = -version-info @LT_SCILEXER_CURRENT@:@LT_SCILEXER_REVISION@:@LT_SCILEXER_AGE@

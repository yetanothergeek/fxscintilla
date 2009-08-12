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
	CharClassify.cxx \
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
    Decoration.cxx \
    PositionCache.cxx \
    RunStyles.cxx \
    UniConversion.cxx \
	ExternalLexer.cxx \
	KeyWords.cxx \
    PerLine.cxx \
    \
    LexAPDL.cxx \
    LexASY.cxx \
    LexAU3.cxx \
    LexAVE.cxx \
    LexAbaqus.cxx \
    LexAda.cxx \
    LexAsm.cxx \
    LexAsn1.cxx \
    LexBaan.cxx \
    LexBash.cxx \
    LexBasic.cxx \
    LexBullant.cxx \
    LexCLW.cxx \
    LexCOBOL.cxx \
    LexCPP.cxx \
    LexCSS.cxx \
    LexCaml.cxx \
    LexCmake.cxx \
    LexConf.cxx \
    LexCrontab.cxx \
    LexCsound.cxx \
    LexD.cxx \
    LexEScript.cxx \
    LexEiffel.cxx \
    LexErlang.cxx \
    LexFlagship.cxx \
    LexForth.cxx \
    LexFortran.cxx \
    LexGAP.cxx \
    LexGui4Cli.cxx \
    LexHTML.cxx \
    LexHaskell.cxx \
    LexInno.cxx \
    LexKix.cxx \
    LexLisp.cxx \
    LexLout.cxx \
    LexLua.cxx \
    LexMMIXAL.cxx \
    LexMPT.cxx \
    LexMSSQL.cxx \
    LexMagik.cxx \
    LexMatlab.cxx \
    LexMetapost.cxx \
    LexMySQL.cxx \
    LexNimrod.cxx \
    LexNsis.cxx \
    LexOpal.cxx \
    LexOthers.cxx \
    LexPB.cxx \
    LexPLM.cxx \
    LexPOV.cxx \
    LexPS.cxx \
    LexPascal.cxx \
    LexPerl.cxx \
    LexPowerPro.cxx \
    LexPowerShell.cxx \
    LexProgress.cxx \
    LexPython.cxx \
    LexR.cxx \
    LexRebol.cxx \
    LexRuby.cxx \
    LexSML.cxx \
    LexSQL.cxx \
    LexScriptol.cxx \
    LexSmalltalk.cxx \
    LexSorcus.cxx \
    LexSpecman.cxx \
    LexSpice.cxx \
    LexTACL.cxx \
    LexTADS3.cxx \
    LexTAL.cxx \
    LexTCL.cxx \
    LexTeX.cxx \
    LexVB.cxx \
    LexVHDL.cxx \
    LexVerilog.cxx \
    LexYAML.cxx

libscintilla_la_CXXFLAGS = $(AM_CXXFLAGS) -DSCI_LEXER

## NoLexer enabled
if NOLEXER

noinst_LTLIBRARIES += libscintillanolexer.la

libscintillanolexer_la_SOURCES = \
	DocumentAccessor.cxx \
	WindowAccessor.cxx \
	StyleContext.cxx \
	CharClassify.cxx \
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

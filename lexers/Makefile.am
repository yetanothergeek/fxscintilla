INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src -I$(top_srcdir)/lexlib
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

noinst_LTLIBRARIES = liblexers.la

liblexers_la_SOURCES = \
  LexA68k.cxx \
  LexAPDL.cxx \
  LexASY.cxx \
  LexAU3.cxx \
  LexAVE.cxx \
  LexAVS.cxx \
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
  LexCoffeeScript.cxx \
  LexConf.cxx \
  LexCrontab.cxx \
  LexCsound.cxx \
  LexD.cxx \
  LexECL.cxx \
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
  LexMarkdown.cxx \
  LexMatlab.cxx \
  LexMetapost.cxx \
  LexModula.cxx \
  LexMySQL.cxx \
  LexNimrod.cxx \
  LexNsis.cxx \
  LexOScript.cxx \
  LexOpal.cxx \
  LexOthers.cxx \
  LexPB.cxx \
  LexPLM.cxx \
  LexPO.cxx \
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
  LexTCMD.cxx \
  LexTeX.cxx \
  LexTxt2tags.cxx \
  LexVB.cxx \
  LexVHDL.cxx \
  LexVerilog.cxx \
  LexVisualProlog.cxx \
  LexYAML.cxx



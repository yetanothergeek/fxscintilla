# Microsoft Developer Studio Project File - Name="fxscintilla" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fxscintilla - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fxscintilla.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fxscintilla.mak" CFG="fxscintilla - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fxscintilla - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fxscintilla - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "fxscintilla - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "d:/fox-1.0.36/include" /I "../../../include" /I "../../../scintilla/include" /I "../../../scintilla/src" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "FOX" /D "SCI_LEXER" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/fxscintilla.lib"

!ELSEIF  "$(CFG)" == "fxscintilla - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "d:/fox-1.0.36/include" /I "../../../include" /I "../../../scintilla/include" /I "../../../scintilla/src" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "FOX" /D "SCI_LEXER" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/fxscintillad.lib"

!ENDIF 

# Begin Target

# Name "fxscintilla - Win32 Release"
# Name "fxscintilla - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\scintilla\src\AutoComplete.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\CallTip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\CellBuffer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\ContractionState.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\Document.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\DocumentAccessor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\Editor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\ExternalLexer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\Indicator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\KeyMap.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\KeyWords.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexAda.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexAsm.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexAVE.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexBaan.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexBullant.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexCLW.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexConf.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexCPP.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexCrontab.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexCSS.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexEiffel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexErlang.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexEScript.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexFortran.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexHTML.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexLisp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexLout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexLua.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexMatlab.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexMMIXAL.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexMPT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexNsis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexOthers.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexPascal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexPerl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexPOV.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexPS.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexPython.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexRuby.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexScriptol.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexSQL.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexVB.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexYAML.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LineMarker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PlatFOX.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\PropSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\RESearch.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\ScintillaBase.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ScintillaFOX.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\Style.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\StyleContext.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\UniConversion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\ViewStyle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\WindowAccessor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\XPM.cxx
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\include\FXScintilla.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\SciLexer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Scintilla.h
# End Source File
# End Group
# End Target
# End Project

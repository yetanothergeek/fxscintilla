# Microsoft Developer Studio Project File - Name="fxscintilladll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fxscintilladll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fxscintilladll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fxscintilladll.mak" CFG="fxscintilladll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fxscintilladll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fxscintilladll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fxscintilladll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /dll /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /dll /nologo /warn:nofileopt
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXSCINTILLADLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "d:/fox-1.0.3/include" /I "../../../include" /I "../../../scintilla/include" /I "../../../scintilla/src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXSCINTILLADLL_EXPORTS" /D "FOX" /D "FOXDLL" /D "SCI_LEXER" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 foxdll.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../lib/fxscintilladll.dll" /libpath:"d:/fox-1.0.3/lib"

!ELSEIF  "$(CFG)" == "fxscintilladll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /dll /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /dll /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXSCINTILLADLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "d:/fox-1.0.3/include" /I "../../../include" /I "../../../scintilla/include" /I "../../../scintilla/src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FXSCINTILLADLL_EXPORTS" /D "FOX" /D "FOXDLL" /D "SCI_LEXER" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 foxdlld.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/fxscintilladlld.dll" /pdbtype:sept /libpath:"d:/fox-1.0.3/lib"

!ENDIF 

# Begin Target

# Name "fxscintilladll - Win32 Release"
# Name "fxscintilladll - Win32 Debug"
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

SOURCE=..\..\..\scintilla\src\LexFortran.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexHTML.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexLisp.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexLua.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexMatlab.cxx
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

SOURCE=..\..\..\scintilla\src\LexPython.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexRuby.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexSQL.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\scintilla\src\LexVB.cxx
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
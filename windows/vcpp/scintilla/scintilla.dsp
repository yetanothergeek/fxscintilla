# Microsoft Developer Studio Project File - Name="scintilla" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=scintilla - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "scintilla.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "scintilla.mak" CFG="scintilla - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "scintilla - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "scintilla - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "scintilla - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "scintilla - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /I "src" /I "d:\fox-0.99.181\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "FOX" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "scintilla - Win32 Release"
# Name "scintilla - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\src\AutoComplete.cxx
# End Source File
# Begin Source File

SOURCE=.\src\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=.\src\CallTip.cxx
# End Source File
# Begin Source File

SOURCE=.\src\CallTip.h
# End Source File
# Begin Source File

SOURCE=.\src\CellBuffer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=.\src\ContractionState.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ContractionState.h
# End Source File
# Begin Source File

SOURCE=.\src\Document.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Document.h
# End Source File
# Begin Source File

SOURCE=.\src\DocumentAccessor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=.\src\Editor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Editor.h
# End Source File
# Begin Source File

SOURCE=.\src\Indicator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Indicator.h
# End Source File
# Begin Source File

SOURCE=.\src\KeyMap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\KeyMap.h
# End Source File
# Begin Source File

SOURCE=.\src\KeyWords.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexAda.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexAVE.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexBaan.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexBullant.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexConf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexCPP.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexCrontab.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexEiffel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexHTML.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexLisp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexLua.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexOthers.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexPascal.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexPerl.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexPython.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexRuby.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexSQL.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LexVB.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LineMarker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\LineMarker.h
# End Source File
# Begin Source File

SOURCE=.\src\PropSet.cxx
# End Source File
# Begin Source File

SOURCE=.\src\RESearch.cxx
# End Source File
# Begin Source File

SOURCE=.\src\RESearch.h
# End Source File
# Begin Source File

SOURCE=.\src\ScintillaBase.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=.\src\Style.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Style.h
# End Source File
# Begin Source File

SOURCE=.\src\StyleContext.cxx
# End Source File
# Begin Source File

SOURCE=.\src\StyleContext.h
# End Source File
# Begin Source File

SOURCE=.\src\SVector.h
# End Source File
# Begin Source File

SOURCE=.\src\UniConversion.cxx
# End Source File
# Begin Source File

SOURCE=.\src\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\src\ViewStyle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=.\src\WindowAccessor.cxx
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\include\Accessor.h
# End Source File
# Begin Source File

SOURCE=.\include\KeyWords.h
# End Source File
# Begin Source File

SOURCE=.\include\Platform.h
# End Source File
# Begin Source File

SOURCE=.\include\PropSet.h
# End Source File
# Begin Source File

SOURCE=.\include\SciLexer.h
# End Source File
# Begin Source File

SOURCE=.\include\Scintilla.h
# End Source File
# Begin Source File

SOURCE=.\include\ScintillaWidget.h
# End Source File
# Begin Source File

SOURCE=.\include\SString.h
# End Source File
# Begin Source File

SOURCE=.\include\WindowAccessor.h
# End Source File
# End Group
# Begin Group "fox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fox\FXScintilla.h
# End Source File
# Begin Source File

SOURCE=.\fox\PlatFOX.cxx
# End Source File
# Begin Source File

SOURCE=.\fox\ScintillaFOX.cxx
# End Source File
# End Group
# End Target
# End Project

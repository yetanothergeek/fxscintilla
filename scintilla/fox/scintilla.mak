# FXScintilla source code edit control
#
# scintilla.mak - Make file for FXScintilla on Windows Visual C++
#
# Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
#
# Adapted from the Scintilla Win32 makefile 
# Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
#
# ======================================================================
#
# This file is part of FXScintilla.
# 
# FXScintilla is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# FXScintilla is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with FXScintilla; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


# ***Borland C++ does not work currently***
#
# This makefile is for using Visual C++ with nmake or Borland C++ with make depending on 
# the setting of the VENDOR macro. If no VENDOR is defined n the command line then
# the tool used is automatically detected.
# Usage for Microsoft:
#     nmake -f scintilla.mak

.SUFFIXES: .cxx

DIR_O=.
DIR_BIN=..\bin

COMPONENT=$(DIR_BIN)\Scintilla.dll
LEXCOMPONENT=$(DIR_BIN)\SciLexer.dll
STATIC_LIB=$(DIR_BIN)\Scintilla-static.lib

!IFNDEF VENDOR
!IFDEF _NMAKE_VER
#Microsoft nmake so make default VENDOR MICROSOFT
VENDOR=MICROSOFT
!ELSE
VENDOR=BORLAND
!ENDIF
!ENDIF

!IF "$(VENDOR)"=="MICROSOFT"

CC=cl
RC=rc
LD=link

FOX_TOP= D:\fox-1.0.3
FOX_INCLUDES= -I $(FOX_TOP)/include
FOX_LIBS_NDEBUG=$(FOX_TOP)/lib/fox.lib
FOX_LIBS_DEBUG=$(FOX_TOP)/lib/foxd.lib

INCLUDEDIRS=-I ../include -I ../src $(FOX_INCLUDES)
CXXFLAGS=/nologo /W3 /GX /D "WIN32" /D "_MBCS" /D "_LIB" /D "FOX" /YX /FD /c
# For something scary:/Wp64
CXXDEBUG=/MTd /GZ /Gm /ZI /Od /D "_DEBUG"
CXXNDEBUG=/Ox /MT /D "_NDEBUG"
NAMEFLAG=-Fo
LDFLAGS=/opt:nowin98
LDDEBUG=/DEBUG
LDNDEBUG=
#LIBS=KERNEL32.lib USER32.lib GDI32.lib IMM32.lib OLE32.LIB
LIBS_NDEBUG=$(FOX_LIBS_NDEBUG)
LIBS_DEBUG=$(FOX_LIBS_DEBUG)

!IFDEF QUIET
CC=@$(CC)
CXXDEBUG=$(CXXDEBUG) /nologo 
CXXNDEBUG=$(CXXNDEBUG) /nologo
LDFLAGS=$(LDFLAGS) /nologo
!ENDIF

!ELSE
# BORLAND
!error Borland C++ not supported 
!ENDIF

!IFDEF DEBUG
CXXFLAGS=$(CXXFLAGS) $(CXXDEBUG)
LDFLAGS=$(LDDEBUG) $(LDFLAGS)
LIBS=$(LIBS_DEBUG)
!ELSE
CXXFLAGS=$(CXXFLAGS) $(CXXNDEBUG)
LDFLAGS=$(LDNDEBUG) $(LDFLAGS)
LIBS=$(LIBS_NDEBUG)
!ENDIF

#ALL:	$(STATIC_LIB) $(COMPONENT) $(LEXCOMPONENT) $(DIR_O)\ScintillaFOXS.obj $(DIR_O)\WindowAccessor.obj
ALL:	$(STATIC_LIB) $(DIR_O)\ScintillaFOXS.obj $(DIR_O)\WindowAccessor.obj

clean:
	-del /q $(DIR_O)\*.obj $(DIR_O)\*.pdb $(COMPONENT) $(LEXCOMPONENT) $(DIR_O)\*.res $(DIR_BIN)\*.map

SOBJS=\
	$(DIR_O)\AutoComplete.obj \
	$(DIR_O)\CallTip.obj \
	$(DIR_O)\CellBuffer.obj \
	$(DIR_O)\ContractionState.obj \
	$(DIR_O)\Document.obj \
	$(DIR_O)\Editor.obj \
	$(DIR_O)\Indicator.obj \
	$(DIR_O)\KeyMap.obj \
	$(DIR_O)\LineMarker.obj \
	$(DIR_O)\PlatFOX.obj \
	$(DIR_O)\RESearch.obj \
	$(DIR_O)\PropSet.obj \
	$(DIR_O)\ScintillaBase.obj \
	$(DIR_O)\ScintillaFOX.obj \
	$(DIR_O)\Style.obj \
	$(DIR_O)\UniConversion.obj \
	$(DIR_O)\ViewStyle.obj

LEXOBJS=\
	$(DIR_O)\LexAda.obj \
	$(DIR_O)\LexAVE.obj \
	$(DIR_O)\LexBaan.obj \
	$(DIR_O)\LexBullant.obj \
	$(DIR_O)\LexConf.obj \
	$(DIR_O)\LexCPP.obj \
	$(DIR_O)\LexCrontab.obj \
	$(DIR_O)\LexEiffel.obj \
	$(DIR_O)\LexHTML.obj \
	$(DIR_O)\LexLisp.obj \
	$(DIR_O)\LexLua.obj \
	$(DIR_O)\LexOthers.obj \
	$(DIR_O)\LexPascal.obj \
	$(DIR_O)\LexPerl.obj \
	$(DIR_O)\LexPython.obj \
	$(DIR_O)\LexRuby.obj \
	$(DIR_O)\LexSQL.obj \
	$(DIR_O)\LexVB.obj

LOBJS=\
	$(DIR_O)\AutoComplete.obj \
	$(DIR_O)\CallTip.obj \
	$(DIR_O)\CellBuffer.obj \
	$(DIR_O)\ContractionState.obj \
	$(DIR_O)\Document.obj \
	$(DIR_O)\DocumentAccessor.obj \
	$(DIR_O)\Editor.obj \
	$(DIR_O)\Indicator.obj \
	$(DIR_O)\KeyMap.obj \
	$(DIR_O)\KeyWords.obj \
	$(DIR_O)\LineMarker.obj \
	$(DIR_O)\PlatFOX.obj \
	$(DIR_O)\RESearch.obj \
	$(DIR_O)\PropSet.obj \
	$(DIR_O)\ScintillaBaseL.obj \
	$(DIR_O)\ScintillaFOXL.obj \
	$(DIR_O)\Style.obj \
	$(DIR_O)\StyleContext.obj \
	$(DIR_O)\UniConversion.obj \
	$(DIR_O)\ViewStyle.obj \
	$(LEXOBJS)

!IF "$(VENDOR)"=="MICROSOFT"

$(STATIC_LIB): $(LOBJS) #$(DIR_O)\ScintRes.res
	lib.exe /OUT:$@ $(LOBJS)

$(COMPONENT): $(SOBJS) #$(DIR_O)\ScintRes.res
	$(LD) $(LDFLAGS) /DLL /OUT:$@ $(SOBJS) $(LIBS)

$(DIR_O)\ScintRes.res : ScintRes.rc
	$(RC) /fo$@ $(*B).rc

$(LEXCOMPONENT): $(LOBJS) #$(DIR_O)\ScintRes.res
	$(LD) $(LDFLAGS) /DLL /OUT:$@ $(LOBJS) $(LIBS)

!ELSE

$(STATIC_LIB): $(LOBJS) #$(DIR_O)\ScintRes.res
	$(LD) /OUT:$@ $(LOBJS) $(LIBS)

$(COMPONENT): $(SOBJS) ScintRes.res
	$(LD) $(LDFLAGS) -Tpd -c c0d32 $(SOBJS), $@, , $(LIBS), , ScintRes.res

$(DIR_O)\ScintRes.res: ScintRes.rc
	$(RC) $*.rc

$(LEXCOMPONENT): $(LOBJS)
	$(LD) $(LDFLAGS) -Tpd -c c0d32 $(LOBJS), $@, , $(LIBS), , ScintRes.res

!ENDIF

# Define how to build all the objects and what they depend on

# Most of the source is in ..\src with a couple in this directory
{..\src}.cxx{$(DIR_O)}.obj:
	$(CC) $(INCLUDEDIRS) $(CXXFLAGS) -c $(NAMEFLAG)$@ $<
{.}.cxx{$(DIR_O)}.obj:
	$(CC) $(INCLUDEDIRS) $(CXXFLAGS) -c $(NAMEFLAG)$@ $<

# Some source files are compiled into more than one object because of different conditional compilation
$(DIR_O)\ScintillaBaseL.obj: ..\src\ScintillaBase.cxx
	$(CC) $(INCLUDEDIRS) $(CXXFLAGS) -DSCI_LEXER -c $(NAMEFLAG)$@ ..\src\ScintillaBase.cxx

$(DIR_O)\ScintillaFOXL.obj: ScintillaFOX.cxx
	$(CC) $(INCLUDEDIRS) $(CXXFLAGS) -DSCI_LEXER -c $(NAMEFLAG)$@ ScintillaFOX.cxx

$(DIR_O)\ScintillaFOXS.obj: ScintillaFOX.cxx
	$(CC) $(INCLUDEDIRS) $(CXXFLAGS) -DSTATIC_BUILD -c $(NAMEFLAG)$@ ScintillaFOX.cxx

# Dependencies

# All lexers depend on this set of headers
LEX_HEADERS=..\include\Platform.h ..\include\PropSet.h \
 ..\include\SString.h ..\include\Accessor.h ..\include\KeyWords.h \
 ..\include\Scintilla.h ..\include\SciLexer.h ..\src\StyleContext.h
 
$(DIR_O)\AutoComplete.obj: ..\src\AutoComplete.cxx ..\include\Platform.h ..\src\AutoComplete.h

$(DIR_O)\CallTip.obj: ..\src\CallTip.cxx ..\include\Platform.h ..\src\CallTip.h

$(DIR_O)\CellBuffer.obj: ..\src\CellBuffer.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\CellBuffer.h

$(DIR_O)\ContractionState.obj: ..\src\ContractionState.cxx ..\include\Platform.h ..\src\ContractionState.h

$(DIR_O)\Document.obj: ..\src\Document.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\RESearch.h \
 ..\src\CellBuffer.h ..\src\Document.h

$(DIR_O)\DocumentAccessor.obj: ..\src\DocumentAccessor.cxx ..\include\Platform.h ..\include\PropSet.h ..\include\Accessor.h ..\src\DocumentAccessor.h ..\include\Scintilla.h

$(DIR_O)\Editor.obj: ..\src\Editor.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\ContractionState.h \
 ..\src\CellBuffer.h ..\src\KeyMap.h ..\src\Indicator.h ..\src\LineMarker.h ..\src\Style.h ..\src\ViewStyle.h \
 ..\src\Document.h ..\src\Editor.h

$(DIR_O)\Indicator.obj: ..\src\Indicator.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\Indicator.h

$(DIR_O)\KeyMap.obj: ..\src\KeyMap.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\KeyMap.h

$(DIR_O)\KeyWords.obj: ..\src\KeyWords.cxx ..\include\Platform.h ..\include\PropSet.h ..\include\Accessor.h ..\include\KeyWords.h \
 ..\include\Scintilla.h ..\include\SciLexer.h 

$(DIR_O)\LexAda.obj: ..\src\LexAda.cxx $(LEX_HEADERS)

$(DIR_O)\LexAVE.obj: ..\src\LexAVE.cxx $(LEX_HEADERS)

$(DIR_O)\LexBaan.obj: ..\src\LexBaan.cxx $(LEX_HEADERS)

$(DIR_O)\LexBullant.obj: ..\src\LexBullant.cxx $(LEX_HEADERS)

$(DIR_O)\LexConf.obj: ..\src\LexConf.cxx $(LEX_HEADERS)

$(DIR_O)\LexCPP.obj: ..\src\LexCPP.cxx $(LEX_HEADERS)

$(DIR_O)\LexCrontab.obj: ..\src\LexCrontab.cxx $(LEX_HEADERS)

$(DIR_O)\LexHTML.obj: ..\src\LexHTML.cxx $(LEX_HEADERS)

$(DIR_O)\LexLisp.obj: ..\src\LexLisp.cxx $(LEX_HEADERS)

$(DIR_O)\LexLua.obj: ..\src\LexLua.cxx $(LEX_HEADERS)

$(DIR_O)\LexOthers.obj: ..\src\LexOthers.cxx $(LEX_HEADERS)

$(DIR_O)\LexPascal.obj: ..\src\LexPascal.cxx $(LEX_HEADERS)

$(DIR_O)\LexPerl.obj: ..\src\LexPerl.cxx $(LEX_HEADERS)

$(DIR_O)\LexPython.obj: ..\src\LexPython.cxx $(LEX_HEADERS)

$(DIR_O)\LexSQL.obj: ..\src\LexSQL.cxx $(LEX_HEADERS)

$(DIR_O)\LexVB.obj: ..\src\LexVB.cxx $(LEX_HEADERS)

$(DIR_O)\LineMarker.obj: ..\src\LineMarker.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\LineMarker.h

$(DIR_O)\PlatWin.obj: PlatWin.cxx ..\include\Platform.h PlatformRes.h ..\src\UniConversion.h

$(DIR_O)\PropSet.obj: ..\src\PropSet.cxx ..\include\Platform.h ..\include\PropSet.h \
 ..\include\SString.h

$(DIR_O)\RESearch.obj: ..\src\RESearch.cxx ..\src\RESearch.h

$(DIR_O)\ScintillaBase.obj: ..\src\ScintillaBase.cxx ..\include\Platform.h ..\include\Scintilla.h \
 ..\src\ContractionState.h ..\src\CellBuffer.h ..\src\CallTip.h ..\src\KeyMap.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\ViewStyle.h ..\src\AutoComplete.h ..\src\Document.h ..\src\Editor.h \
 ..\src\ScintillaBase.h

$(DIR_O)\ScintillaBaseL.obj: ..\src\ScintillaBase.cxx ..\include\Platform.h ..\include\Scintilla.h ..\include\SciLexer.h \
 ..\src\ContractionState.h ..\src\CellBuffer.h ..\src\CallTip.h ..\src\KeyMap.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\AutoComplete.h ..\src\ViewStyle.h ..\src\Document.h ..\src\Editor.h \
 ..\src\ScintillaBase.h ..\include\PropSet.h ..\include\Accessor.h ..\src\DocumentAccessor.h ..\include\KeyWords.h

$(DIR_O)\ScintillaWin.obj: ScintillaWin.cxx ..\include\Platform.h ..\include\Scintilla.h \
 ..\src\ContractionState.h ..\src\CellBuffer.h ..\src\CallTip.h ..\src\KeyMap.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\AutoComplete.h ..\src\ViewStyle.h ..\src\Document.h ..\src\Editor.h \
 ..\src\ScintillaBase.h ..\src\UniConversion.h

$(DIR_O)\ScintillaWinL.obj: ScintillaWin.cxx ..\include\Platform.h ..\include\Scintilla.h ..\include\SciLexer.h \
 ..\src\ContractionState.h ..\src\CellBuffer.h ..\src\CallTip.h ..\src\KeyMap.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\AutoComplete.h ..\src\ViewStyle.h ..\src\Document.h ..\src\Editor.h \
 ..\src\ScintillaBase.h ..\include\PropSet.h ..\include\Accessor.h ..\include\KeyWords.h ..\src\UniConversion.h

$(DIR_O)\ScintillaWinS.obj: ScintillaWin.cxx ..\include\Platform.h ..\include\Scintilla.h \
 ..\src\ContractionState.h ..\src\CellBuffer.h ..\src\CallTip.h ..\src\KeyMap.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\AutoComplete.h ..\src\ViewStyle.h ..\src\Document.h ..\src\Editor.h \
 ..\src\ScintillaBase.h ..\src\UniConversion.h

$(DIR_O)\Style.obj: ..\src\Style.cxx ..\include\Platform.h ..\src\Style.h

$(DIR_O)\StyleContext.obj: ..\src\StyleContext.cxx ..\include\Platform.h ..\include\Accessor.h ..\include\PropSet.h ..\src\StyleContext.h

$(DIR_O)\ViewStyle.obj: ..\src\ViewStyle.cxx ..\include\Platform.h ..\include\Scintilla.h ..\src\Indicator.h \
 ..\src\LineMarker.h ..\src\Style.h ..\src\ViewStyle.h

$(DIR_O)\UniConversion.obj: ..\src\UniConversion.cxx ..\src\UniConversion.h

$(DIR_O)\WindowAccessor.obj: ..\src\WindowAccessor.cxx ..\include\Platform.h ..\include\PropSet.h \
 ..\include\Accessor.h ..\include\WindowAccessor.h ..\include\Scintilla.h

# FXScintilla source code edit control
#
# makefile - Make file for FXScintilla on Linux or compatible OS
#
# Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
#
# Adapted from the Scintilla GTK makefile 
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


FOX_prefix=/usr/local

.SUFFIXES: .cxx .o .h .a
CC = g++
AR = ar
RANLIB = touch

ifeq ($(OSTYPE),darwin)
CC = c++
AR = ar
RANLIB = ranlib
endif

COMPLIB=../bin/scintilla.a

vpath %.h ../src ../include
vpath %.cxx ../src

INCLUDEDIRS=-I ../include -I ../src
CXXBASEFLAGS= -W -Wall -DFOX -DSCI_LEXER $(INCLUDEDIRS)

ifdef DEBUG
CXXFLAGS=-DDEBUG -g $(CXXBASEFLAGS)
else
CXXFLAGS=-DNDEBUG $(CXXBASEFLAGS)
endif

.cxx.o:
	$(CC) -I$(FOX_prefix)/include $(CXXFLAGS) -c $<

LEXOBJS=LexAda.o LexAVE.o LexBaan.o LexBullant.o LexConf.o LexCPP.o \
	LexCrontab.o LexEiffel.o LexHTML.o LexLisp.o LexLua.o LexMatlab.o LexOthers.o \
	LexPascal.o LexPerl.o LexPython.o LexRuby.o LexSQL.o LexVB.o

# The LEXOBJS have to be treated specially as the functions in them are not called from external code

all: $(COMPLIB) $(LEXOBJS)

clean:
	rm -f *.o $(COMPLIB)

deps:
	$(CC) -MM -I$(FOX_prefix)/include $(CXXFLAGS) *.cxx ../src/*.cxx > deps.mak

$(COMPLIB): DocumentAccessor.o WindowAccessor.o KeyWords.o StyleContext.o Document.o CallTip.o \
	ScintillaBase.o ContractionState.o Editor.o PropSet.o PlatFOX.o \
	KeyMap.o LineMarker.o ScintillaFOX.o CellBuffer.o ViewStyle.o \
	RESearch.o Style.o Indicator.o AutoComplete.o
	$(RM) $@
	$(AR) rc $@ $^
	$(RANLIB) $@

# Automatically generate header dependencies with "make deps"
include deps.mak

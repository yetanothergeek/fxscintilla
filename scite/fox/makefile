# FXSciTE - FXScintilla based Text Editor
#
# makefile - Make file for FXSciTE on Linux or compatible OS
#
# Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
#
# Adapted from the SciTE GTK makefile 
# Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
#
# ======================================================================
#
# This file is part of FXSciTE.
# 
# FXSciTE is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# FXSciTE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FXSciTE; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

FOX_prefix=/usr/local

.SUFFIXES: .cxx .o .h .a
CC = g++
AR = ar

# For the Gnome desktop stuff to work, prefix must point to where Gnome thinks it is.
# "h@h@" is printed here when gnome-config unavailable. Seems harmless.
gnomeprefix:=$(shell gnome-config --prefix 2>/dev/null)
ifdef gnomeprefix
  prefix=$(gnomeprefix)
  datadir=$(gnomeprefix)/share
  pixmapdir=$(datadir)/pixmaps
else
  prefix=/usr/local
endif
bindir=$(prefix)/bin
SYSCONF_PATH=$(prefix)/share/scite

PROG	= ../bin/SciTE

vpath %.h ../src ../../scintilla/include
vpath %.cxx ../src

#CXXFLAGS= -g -DFOX -DSCI_LEXER -DSYSCONF_PATH=\"$(SYSCONF_PATH)\" -Wwrite-strings
INCLUDEDIRS=-I ../../scintilla/fox -I ../../scintilla/include -I ../src
CXXBASEFLAGS= -W -Wall -DFOX -DSCI_LEXER -DSYSCONF_PATH=\"$(SYSCONF_PATH)\" $(INCLUDEDIRS)

ifdef DEBUG
CXXFLAGS=-DDEBUG -g $(CXXBASEFLAGS)
else
CXXFLAGS=-DNDEBUG $(CXXBASEFLAGS)
endif

.cxx.o:
	$(CC) -I$(FOX_prefix)/include $(CXXFLAGS) -c $< -o $@

all: $(PROG)

clean:
	rm -f *.o $(PROG)

deps:
	$(CC) -MM -I$(FOX_prefix)/include $(CXXFLAGS) *.cxx ../src/*.cxx >deps.mak

# make should be run in ../..scintilla/fox to compile all the lexers.
LEXEROBJS=$(wildcard ../../scintilla/fox/Lex*.o)

$(PROG): SciTEFOX.o SciTEBase.o SciTEBuffers.o SciTEIO.o Exporters.o DirectorExtension.o \
SciTEProps.o $(LEXEROBJS) ../../scintilla/bin/scintilla.a
	$(CC) -g -DFOX $^ -L$(FOX_prefix)/lib -lFOX -o $@

# SciTE-static no longer builds. Kept here in case of later need.
SciTE-static: SciTEFOX.o SciTEBase.o SciTEBuffers.o SciTEIO.o Exporters.o SciTEProps.o \
KeyWords.o StyleContext.o Document.o Scintilla.o PropSet.o DirectorExtension.o
	$(CC) -L/usr/X11R6/lib -lXext -lX11 -lm -DFOX -DSCI_LEXER SciTE.o KeyWords.o StyeContext.o Document.o Scintilla.o PropSet.o $(FOX_prefix)/lib/libFOX.a -o SciTE-static

# Automatically generate header dependencies with "make dep"
include deps.mak

# The two last install commands will fail if Gnome is not installed or is not at $(prefix).
# This is OK - just means no SciTE in the Gnome Applications menu
# Dead:	install -D SciTEFOX.properties $(SYSCONF_PATH)/SciTEGlobal.properties
install:
	install -s $(PROG) $(bindir)
	install -d $(SYSCONF_PATH)
	install -D ../src/*.properties $(SYSCONF_PATH)
	install ../doc/*.html $(SYSCONF_PATH)
	install ../doc/SciTEIco.png $(SYSCONF_PATH)
	install ../doc/PrintHi.png $(SYSCONF_PATH)
ifdef gnomeprefix
	install SciTE.desktop $(datadir)/gnome/apps/Applications
	install Sci48M.png $(pixmapdir)
endif

uninstall:
	rm -f $(bindir)/SciTE
	rm -rf $(SYSCONF_PATH)
ifdef gnomeprefix
	rm -f $(datadir)/gnome/apps/Applications/SciTE.desktop
	rm -f $(pixmapdir)/Sci48M.png
endif

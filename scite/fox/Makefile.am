scitedir = $(datadir)/scite

INCLUDES = -I../../scintilla/include -I../../scintilla/fox -I../src
AM_CXXFLAGS = -W -Wall -DFOX -DSCI_LEXER -DSYSCONF_PATH=\"$(scitedir)\"

bin_PROGRAMS = fxscite

fxscite_SOURCES = \
	SciTEFOX.cxx \
	DirectorExtension.cxx

fxscite_LDADD = \
	../src/libscite.la \
	../../scintilla/src/libscilexers.la \
	../../scintilla/fox/libfxscintilla.la \
	-L/usr/local/lib -lFOX
all:
	(cd scintilla/fox; make all DEBUG=1)
	(cd scite/fox; make all DEBUG=1)

clean:
	(cd scintilla/fox; make clean)
	(cd scite/fox; make clean)

deps:
	(cd scintilla/fox; make deps)
	(cd scite/fox; make deps)

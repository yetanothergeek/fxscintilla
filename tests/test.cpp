#include <fx.h>
#include <Scintilla.h>
#include <FXScintilla.h>

int main(int argc, char ** argv)
{
	FXApp application("test");
	application.init(argc, argv);
	FXMainWindow win(&application, "Test");
	FXScintilla scint(&win, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
	scint.sendMessage(SCI_SETLEXERLANGUAGE, 0, (long)"ruby");
	application.create();
	win.show(PLACEMENT_SCREEN);
	application.run();
	return 0;
}

#if !defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
# if HAVE_FOX_1_1
#  include <fox-1.1/fx.h>
# else
#  include <fox/fx.h>
# endif
#else
# include <fx.h>
#endif // !defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include <Scintilla.h>
#include <SciLexer.h>
#include <FXScintilla.h>

// Initial text
static const char sInitialText[] = "\
#! /usr/bin/env ruby\n\
\n\
require \"fox\"\n\
require \"fox/responder\"\n\
require \"MainWindow\"\n\
\n\
include Fox\n\
\n\
def main\n\
  # Make application\n\
  application = FXApp.new(\"rbCVS\", \"pini\")\n\
\n\
  # Current threads implementation causes problems for this example, so disable\n\
  application.threadsEnabled = false\n\
\n\
  # Open display\n\
  application.init(ARGV)\n\
\n\
  # Set normal font regular\n\
  font = application.normalFont\n\
  fd = font.fontDesc\n\
  fd.weight = FONTWEIGHT_REGULAR\n\
  font.fontDesc = fd\n\
\n\
  # Make window\n\
  MainWindow.new(application)\n\
\n\
  # Create app\n\
  application.create()\n\
  # Run\n\
  application.run()\n\
end\n\
\n\
main\n\
";

// The lexer needs a keywords list
static const char sRubyKeyWords[] =
	"__FILE__ and def end in or self unless __LINE__ "
	"begin defined? ensure module redo super until BEGIN "
	"break do false next rescue then when END case "
	"else for nil retry true while alias class elsif "
	"if not return undef yield";

class Test : public FXMainWindow
{
	FXDECLARE(Test)
protected:
	FXScintilla * scint;
protected:
	Test() {}
public:
	enum {
		ID_SCINTILLA = FXMainWindow::ID_LAST,
		ID_FONT,
		ID_LAST
	};
public:
	Test(FXApp * app) : FXMainWindow(app, "Test", NULL, NULL, DECOR_ALL, 0, 0, 600, 400)
	{
		// Font selector button
		new FXButton(this, "Change Font", NULL, this, ID_FONT);
		
		// FXScintilla widget
		scint = new FXScintilla(this, this, ID_SCINTILLA, LAYOUT_FILL_X|LAYOUT_FILL_Y);	
		// Preparing the widget to do syntax coloring for Ruby
		scint->sendMessage(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<long>("ruby"));
		scint->sendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<long>(sRubyKeyWords));
		setFont("courier", 12);
	}
	virtual void create()
	{
		FXMainWindow::create();
		// Feeding the widget with some initial text
		scint->sendMessage(SCI_INSERTTEXT, 0, reinterpret_cast<long>(sInitialText));
		scint->setFocus();
	}
	void setFont(const FXString & font, int size)
	{
		scint->sendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<long>(font.text()));
		scint->sendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, size);
		scint->sendMessage(SCI_STYLECLEARALL, 0, 0);
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_DEFAULT, FXRGB(0x80, 0x80, 0x80));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTLINE, FXRGB(0x00, 0x7f, 0x00));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_NUMBER, FXRGB(0x00, 0x7f, 0x7f));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_STRING, FXRGB(0x7f, 0x00, 0x7f));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_CHARACTER, FXRGB(0x7f, 0x00, 0x7f));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_WORD, FXRGB(0x00, 0x00, 0x7f));
		scint->sendMessage(SCI_STYLESETBOLD, SCE_P_WORD, 1);
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLE, FXRGB(0x7f, 0x00, 0x00));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLEDOUBLE, FXRGB(0x7f, 0x00, 0x00));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_CLASSNAME, FXRGB(0x00, 0x00, 0xff));
		scint->sendMessage(SCI_STYLESETBOLD, SCE_P_CLASSNAME, 1);
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_DEFNAME, FXRGB(0x00, 0x7f, 0x7f));
		scint->sendMessage(SCI_STYLESETBOLD, SCE_P_DEFNAME, 1);
		scint->sendMessage(SCI_STYLESETBOLD, SCE_P_OPERATOR, 1);
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_IDENTIFIER, FXRGB(0x7f, 0x7f, 0x7f));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTBLOCK, FXRGB(0x7f, 0x7f, 0x7f));
		scint->sendMessage(SCI_STYLESETFORE, SCE_P_STRINGEOL, FXRGB(0x00, 0x00, 0x00));
		scint->sendMessage(SCI_STYLESETBACK, SCE_P_STRINGEOL, FXRGB(0xe0, 0xc0, 0xe0));
		scint->sendMessage(SCI_STYLESETEOLFILLED, SCE_P_STRINGEOL, 1);
		scint->sendMessage(SCI_STYLESETFORE, 34, FXRGB(0x00, 0x00, 0xff));
		scint->sendMessage(SCI_STYLESETBOLD, 34, 1);
		scint->sendMessage(SCI_STYLESETFORE, 35, FXRGB(0xff, 0x00, 0x00));
		scint->sendMessage(SCI_STYLESETBOLD, 35, 1);
	}
	long onCmdFont(FXObject *, FXSelector, void *)
	{
		FXFontDialog diag(this, 0);
		if (diag.execute()) {
			FXFontDesc desc;
			diag.getFontSelection(desc);
			setFont(desc.face, desc.size / 10);
		}
		return 1;
	}
	long onCmdScintilla(FXObject *, FXSelector, void * ptr)
	{
		SCNotification * scn = static_cast<SCNotification *>(ptr);
		
		// Add here stuff to handle Scintilla events
		
		return 1;
	}
};

FXDEFMAP(Test) TestMap[]={
  FXMAPFUNC(SEL_COMMAND, Test::ID_SCINTILLA, Test::onCmdScintilla),
  FXMAPFUNC(SEL_COMMAND, Test::ID_FONT, Test::onCmdFont),
};

FXIMPLEMENT(Test,FXMainWindow,TestMap,ARRAYNUMBER(TestMap))


int main(int argc, char ** argv)
{
	// Fox stuff
	FXApp application(FXString("test"));
	application.init(argc, argv);
	Test * win = new Test(&application);
	application.create();
	win->show(PLACEMENT_SCREEN);
	application.run();
	return 0;
}

#include <fx.h>
#include <Scintilla.h>
#include <SciLexer.h>
#include <FXScintilla.h>

// Initial text
static const char sInitialText[] = "\
#! /usr/bin/env ruby

require \"fox\"
require \"fox/responder\"
require \"MainWindow\"

include Fox

def main
  # Make application
  application = FXApp.new(\"rbCVS\", \"pini\")

  # Current threads implementation causes problems for this example, so disable
  application.threadsEnabled = false
  
  # Open display
  application.init(ARGV)

  # Set normal font regular
  font = application.normalFont
  fd = font.fontDesc
  fd.weight = FONTWEIGHT_REGULAR
  font.fontDesc = fd

  # Make window
  MainWindow.new(application)
  
  # Create app  
  application.create()
  # Run
  application.run()
end

main
";

// The lexer needs a keywords list
static const char sRubyKeyWords[] =
	"__FILE__ and def end in or self unless __LINE__"
	"begin defined? ensure module redo super until BEGIN"
	"break do false next rescue then when END case"
	"else for nil retry true while alias class elsif"
	"if not return undef yield";

int main(int argc, char ** argv)
{
	// Fox stuff
	FXApp application("test");
	application.init(argc, argv);
	FXMainWindow win(&application, "Test", NULL, NULL, DECOR_ALL, 0, 0, 600, 400);

	// The FXScintilla widget
	FXScintilla scint(&win, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);

	// Preparing the widget to do syntax coloring for Ruby
	scint.sendMessage(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<long>("ruby"));
	scint.sendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<long>(sRubyKeyWords));
	scint.sendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<long>("fixed"));
	scint.sendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
	scint.sendMessage(SCI_STYLECLEARALL, 0, 0);
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_DEFAULT, FXRGB(0x80, 0x80, 0x80));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTLINE, FXRGB(0x00, 0x7f, 0x00));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_NUMBER, FXRGB(0x00, 0x7f, 0x7f));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_STRING, FXRGB(0x7f, 0x00, 0x7f));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_CHARACTER, FXRGB(0x7f, 0x00, 0x7f));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_WORD, FXRGB(0x00, 0x00, 0x7f));
	scint.sendMessage(SCI_STYLESETBOLD, SCE_P_WORD, 1);
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLE, FXRGB(0x7f, 0x00, 0x00));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_TRIPLEDOUBLE, FXRGB(0x7f, 0x00, 0x00));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_CLASSNAME, FXRGB(0x00, 0x00, 0xff));
	scint.sendMessage(SCI_STYLESETBOLD, SCE_P_CLASSNAME, 1);
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_DEFNAME, FXRGB(0x00, 0x7f, 0x7f));
	scint.sendMessage(SCI_STYLESETBOLD, SCE_P_DEFNAME, 1);
	scint.sendMessage(SCI_STYLESETBOLD, SCE_P_OPERATOR, 1);
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_IDENTIFIER, FXRGB(0x7f, 0x7f, 0x7f));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_COMMENTBLOCK, FXRGB(0x7f, 0x7f, 0x7f));
	scint.sendMessage(SCI_STYLESETFORE, SCE_P_STRINGEOL, FXRGB(0x00, 0x00, 0x00));
	scint.sendMessage(SCI_STYLESETBACK, SCE_P_STRINGEOL, FXRGB(0xe0, 0xc0, 0xe0));
	scint.sendMessage(SCI_STYLESETEOLFILLED, SCE_P_STRINGEOL, 1);
	scint.sendMessage(SCI_STYLESETFORE, 34, FXRGB(0x00, 0x00, 0xff));
	scint.sendMessage(SCI_STYLESETBOLD, 34, 1);
	scint.sendMessage(SCI_STYLESETFORE, 35, FXRGB(0xff, 0x00, 0x00));
	scint.sendMessage(SCI_STYLESETBOLD, 35, 1);

	// Feeding the widget with some initial text
	scint.sendMessage(SCI_INSERTTEXT, 0, reinterpret_cast<long>(sInitialText));

	// Fox stuff
	application.create();
	win.show(PLACEMENT_SCREEN);
	application.run();
	return 0;
}

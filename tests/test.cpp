#include <fx.h>
#include <Scintilla.h>
#include <SciLexer.h>
#include <FXScintilla.h>

// Initial text
static const char initialText[] = "\
/* A simple 'hello world' program. */\n\
\n\
#include <stdio.h>\n\
\n\
int main(int argc, char *argv[]) {\n\
  printf(\"hello world\\n\");\n\
  return 0;\n\
}\n\
";

// The lexer needs a keywords list
static const char cppKeyWords[] =
  "asm auto break case char const continue default do double "
  "else enum extern float for goto if inline int long register "
  "return short signed sizeof static struct switch typedef "
  "union unsigned void volatile while";


class Test: public FXMainWindow
{
  FXDECLARE(Test)
protected:
  FXScintilla * scint;
  Test() {}
public:
  enum {
    ID_SCINTILLA = FXMainWindow::ID_LAST,
    ID_FONT,
    ID_LAST
  };
  Test(FXApp *app);
  long onCmdScintilla(FXObject *obj, FXSelector sel, void *ptr);
  virtual void create();
  void setFont(const FXString &font, int size);
  long onCmdFont(FXObject *obj, FXSelector sel, void *ptr);
};


FXColor rgb(const char *rrggbb) {
  unsigned int r,g,b;
  sscanf((char*)(rrggbb+1), "%2x%2x%2x", &r,&g, &b);
  return FXRGB(r,g,b);
}


Test::Test(FXApp *app): FXMainWindow(app, "Test", NULL, NULL, DECOR_ALL, 0, 0, 600, 400)
{
  // Font selector button
  new FXButton(this, " Change Font ", NULL, this, ID_FONT);

  // FXScintilla widget
  scint = new FXScintilla(this, this, ID_SCINTILLA, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  // Preparing the widget to do syntax coloring
  scint->sendMessage(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<long>("cpp"));
  scint->sendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<long>(cppKeyWords));
  setFont("courier", 12);
}


void Test::create()
{
  FXMainWindow::create();
  scint->sendMessage(SCI_INSERTTEXT, 0, reinterpret_cast<long>(initialText));
}


void Test::setFont(const FXString &font, int size)
{
  scint->sendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<long>(font.text()));
  scint->sendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, size);
  scint->sendMessage(SCI_STYLECLEARALL, 0, 0);
  scint->sendMessage(SCI_STYLESETBOLD, SCE_C_WORD, true);
  scint->sendMessage(SCI_STYLESETBOLD, SCE_C_OPERATOR, true);
  scint->sendMessage(SCI_STYLESETITALIC, SCE_C_COMMENT, true);
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_DEFAULT, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENT, rgb("#ff00ff"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINE, rgb("#ff00ff"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOC, rgb("#ff00ff"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_NUMBER, rgb("#0000ff"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_WORD, rgb("#a06000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_STRING, rgb("#00dd00"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_CHARACTER, rgb("#00dd00"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_UUID, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, rgb("#6060e0"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_OPERATOR, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_IDENTIFIER, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_STRINGEOL, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_VERBATIM, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_REGEX, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINEDOC, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_WORD2, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORD, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORDERROR, rgb("#000000"));
  scint->sendMessage(SCI_STYLESETFORE, SCE_C_GLOBALCLASS, rgb("#000000"));
}


long Test::onCmdFont(FXObject *obj, FXSelector sel, void *ptr)
{
  FXFontDialog diag(this, "Toto", 0);
  if (diag.execute()) {
#ifdef FOX_1_6
    FXFontDesc desc;
    diag.getFontSelection(desc);
#else
    FXFontDesc desc=diag.getFontDesc();
#endif
    setFont(desc.face, desc.size / 10);
  }
  return 1;
}


long Test::onCmdScintilla(FXObject *obj, FXSelector sel, void *ptr)
{
  SCNotification * scn = static_cast<SCNotification *>(ptr);
  // Add stuff here to handle Scintilla events
  return 1;
}


FXDEFMAP(Test) TestMap[]={
  FXMAPFUNC(SEL_COMMAND, Test::ID_SCINTILLA, Test::onCmdScintilla),
  FXMAPFUNC(SEL_COMMAND, Test::ID_FONT, Test::onCmdFont),
};

FXIMPLEMENT(Test,FXMainWindow,TestMap,ARRAYNUMBER(TestMap))


int main(int argc, char *argv[])
{
  FXApp application(FXString("test"));
  application.init(argc, argv);
  Test * win = new Test(&application);
  application.create();
  win->setTitle(FXString("FXScintilla-")+FXScintilla::version());
  win->show(PLACEMENT_SCREEN);
  application.run();
  return 0;
}


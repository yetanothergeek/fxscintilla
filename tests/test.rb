# This example require Ruby 1.6 + FXRuby 1.0 compiled with FXScintilla enabled

require 'fox'
require 'fox/scintilla'

include Fox

InitialText = """#! /usr/bin/env ruby\n

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
"""

# The lexer needs a keywords list
RubyKeyWords = """__FILE__ and def end in or self unless __LINE__ 
	begin defined? ensure module redo super until BEGIN 
	break do false next rescue then when END case 
	else for nil retry true while alias class elsif 
	if not return undef yield"""

application = FXApp.new("test")
application.init(ARGV)
win = FXMainWindow.new(application, "Test", nil, nil, DECOR_ALL, 0, 0, 600, 400)

# The FXScintilla widget
scint = FXScintilla.new(win, nil, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y)

# Preparing the widget to do syntax coloring for Ruby
scint.setLexerLanguage("ruby")
scint.setKeyWords(0, RubyKeyWords)
scint.styleSetFont(FXScintilla::STYLE_DEFAULT, "fixed")
scint.styleSetSize(FXScintilla::STYLE_DEFAULT, 12)
scint.styleClearAll
scint.styleSetFore(FXScintilla::SCE_P_DEFAULT, FXRGB(0x80, 0x80, 0x80) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_COMMENTLINE, FXRGB(0x00, 0x7f, 0x00) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_NUMBER, FXRGB(0x00, 0x7f, 0x7f) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_STRING, FXRGB(0x7f, 0x00, 0x7f) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_CHARACTER, FXRGB(0x7f, 0x00, 0x7f) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_WORD, FXRGB(0x00, 0x00, 0x7f) & 0xffffff)
scint.styleSetBold(FXScintilla::SCE_P_WORD, true)
scint.styleSetFore(FXScintilla::SCE_P_TRIPLE, FXRGB(0x7f, 0x00, 0x00) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_TRIPLEDOUBLE, FXRGB(0x7f, 0x00, 0x00) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_CLASSNAME, FXRGB(0x00, 0x00, 0xff) & 0xffffff)
scint.styleSetBold(FXScintilla::SCE_P_CLASSNAME, true)
scint.styleSetFore(FXScintilla::SCE_P_DEFNAME, FXRGB(0x00, 0x7f, 0x7f) & 0xffffff)
scint.styleSetBold(FXScintilla::SCE_P_DEFNAME, true)
scint.styleSetBold(FXScintilla::SCE_P_OPERATOR, true)
scint.styleSetFore(FXScintilla::SCE_P_IDENTIFIER, FXRGB(0x7f, 0x7f, 0x7f) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_COMMENTBLOCK, FXRGB(0x7f, 0x7f, 0x7f) & 0xffffff)
scint.styleSetFore(FXScintilla::SCE_P_STRINGEOL, FXRGB(0x00, 0x00, 0x00) & 0xffffff)
scint.styleSetBack(FXScintilla::SCE_P_STRINGEOL, FXRGB(0xe0, 0xc0, 0xe0) & 0xffffff)
scint.styleSetEOLFilled(FXScintilla::SCE_P_STRINGEOL, true)
scint.styleSetFore(34, FXRGB(0x00, 0x00, 0xff) & 0xffffff)
scint.styleSetBold(34, true)
scint.styleSetFore(35, FXRGB(0xff, 0x00, 0x00) & 0xffffff)
scint.styleSetBold(35, true)

# Feeding the widget with some initial text
scint.insertText(0, InitialText)

# Fox stuff
application.create
win.show(PLACEMENT_SCREEN)
application.run

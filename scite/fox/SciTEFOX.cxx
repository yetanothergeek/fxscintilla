/** FXSciTE - FXScintilla based Text Editor
 *
 *  SciTEFOX.cxx - main code for the FOX toolkit version of the editor
 *
 *  Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
 *
 *  Adapted from the SciTE GTK source SciTEGTK.cxx 
 *  Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
 *
 *  ====================================================================
 *
 *  This file is part of FXSciTE.
 * 
 *  FXSciTE is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  FXSciTE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FXSciTE; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>

#include <fox/fx.h>
#include <fox/fxkeys.h>

#include "Platform.h"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#include "SciTE.h"
#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "Extender.h"
#include "SciTEBase.h"
#ifdef LUA_SCRIPTING
#include "LuaExtension.h"
#endif
#ifndef NO_FILER
#include "DirectorExtension.h"
#endif
#include "pixmapsGNOME.h"
#include "SciIcon.h"

#include "FXScintilla.h"
#include <vector>
using namespace std;

#define MB_ABOUTBOX	0x100000L

const char appName[] = "SciTE";

#ifdef __vms
char g_modulePath[MAX_PATH];
#endif

#define TRANSLATED(s) (static_cast<SciTEFOX *>(FXApp::instance()))->Translated(s).c_str()

static void FillComboFromMemory(FXComboBox *combo, const ComboMemory &mem, bool useTop = false) {
	for (int i = 0; i < mem.Length(); i++) {
		combo->appendItem(mem.At(i).c_str());
	}
	if (useTop) {
		combo->setText(mem.At(0).c_str());
	}
}

// ====================================================================
// MessageBox (declaration)
// ====================================================================

// <FIXME> Voir la gestion des raccourcis dans la version GTK </FIXME>
class SCMessageBox : public FXDialogBox
{
	FXDECLARE(SCMessageBox);
protected:
	SCMessageBox() {}
protected:
	FXint escapeResult;
public:
	long onIDOK(FXObject *, FXSelector, void *);
	long onIDYES(FXObject *, FXSelector, void *);
	long onIDNO(FXObject *, FXSelector, void *);
	long onIDCANCEL(FXObject *, FXSelector, void *);
	long onCANCEL(FXObject *, FXSelector, void *);
public:
	enum {
		ID_IDOK=FXDialogBox::ID_LAST,
		ID_IDYES,
		ID_IDNO,
		ID_IDCANCEL,
		ID_LAST
	};
public:
	SCMessageBox(FXWindow *wParent, const char *m, const char *t, int style);
};

// ====================================================================
// FindReplace (declaration)
// ====================================================================

class SCFindReplace : public FXDialogBox
{
	FXDECLARE(SCFindReplace);
protected:
	SCFindReplace() {}
protected:
	FXComboBox * comboFind;
	FXComboBox * comboReplace;
	FXCheckButton * csButton;
	FXCheckButton * wwButton;
	FXCheckButton * reButton;
	FXCheckButton * wpButton;
	FXCheckButton * usButton;
	FXCheckButton * rdButton;
	bool _replacing;
public:
	enum {
		SCFR_CASESENSITIVE = 1,
		SCFR_WHOLEWORD = 2,
		SCFR_REVERSEDIRECTION = 4,
		SCFR_REGEXP = 8,
		SCFR_WRAP = 16,
		SCFR_UNSLASH = 32
	};
public:
	SCFindReplace(FXWindow *wParent, bool replace, const char * findWhat,
								const ComboMemory & memFinds, const ComboMemory & memReplaces,
								FXint settings);
	FXString getFind() const;
	FXString getReplace() const;
	bool replacing() const { return _replacing; }
	FXint getSettings() const;
};

// ====================================================================
// SciTEFOX (declaration)
// ====================================================================

#define ID(id) (FXApp::ID_LAST+id)

class SciTEFOX : public SciTEBase, public FXApp {
FXDECLARE(SciTEFOX)
protected:
	FXMainWindow * mainWindow;
	FXMenubar * menuBar;
	FXMenuPane * fileMenu;
	FXMenuPane * fileExportMenu;
	FXMenuPane * editMenu;
	FXMenuPane * searchMenu;
	FXMenuPane * viewMenu;
	FXMenuPane * toolsMenu;
	FXMenuPane * optionsMenu;
	FXMenuPane * buffersMenu;
	FXMenuPane * helpMenu;
	FXMenuPane * lineEndCharactersMenu;
	FXMenuPane * languageMenu;
	FXMenuPane * editPropsMenu;
	FXToolbar * toolBar;
	FXTextField *entryParam[maxParam];
	FXWindow * compile_btn;
	FXWindow * build_btn;
	FXWindow * stop_btn;
	FXStatusbar * statusBar;
	SCFindReplace * frDialog;
	FXWindow * focus;
protected:
	SciTEFOX() {}
public:
	long onSciTECommand(FXObject *, FXSelector, void*);
	long onPipeSignal(FXObject*, FXSelector, void*);
	long onIOSignal(FXObject*, FXSelector, void*);
	long onCmdNotify(FXObject*, FXSelector, void*);
	long onCmdQuit(FXObject *, FXSelector, void*);
	long onFRFind(FXObject *, FXSelector, void*);
	long onFRReplace(FXObject *, FXSelector, void*);
	long onFRReplaceAll(FXObject *, FXSelector, void*);
	long onFRReplaceInSelection(FXObject *, FXSelector, void*);
	long onFRCancel(FXObject *, FXSelector, void*);
	long onFIFFind(FXObject *, FXSelector, void *);
	long onGTGoto(FXObject *, FXSelector, void *);
	long onSCFocusIn(FXObject *, FXSelector, void*);
	long onSCChanged(FXObject *, FXSelector, void*);
	long onKeyPress(FXObject *, FXSelector, void*);
	long onParamOK(FXObject *, FXSelector, void*);
	long onParamCancel(FXObject *, FXSelector, void*);
	long onRightBtnPress(FXObject *, FXSelector, void *);
public:
	enum {
		IDM_LAST = ID(1000),
		ID_PIPESIGNAL,
		ID_IOSIGNAL,
		ID_NOTIFY,
		// FindReplace notifications
		ID_FRFIND,
		ID_FRREPLACE,
		ID_FRREPLACEALL,
		ID_FRREPLACEINSELECTION,
		ID_FRCANCEL,
		// FindInFiles notifications
		ID_FIFFIND,
		// GoTo notifications
		ID_GTGOTO,
		// Target notifications
		ID_MAINWINDOW,
		ID_EDITOR,
		ID_OUTPUT,
		// Parameters notofocations
		ID_PARAM,
		ID_PARAMOK,
		ID_PARAMCANCEL,

		ID_LAST
	};
public:
	virtual void create();
	int Run(int argc, char ** argv);

protected:
	// Fullscreen handling
	FXRectangle saved;
	
	// Control of sub process
	int icmd;
	int originalEnd;
	int fdFIFO;
	int pidShell;
	char resultsFile[MAX_PATH];
	ElapsedTime commandTime;

	// Command Pipe variables
	int fdPipe;
	char pipeName[MAX_PATH];

	bool dialogCanceled;

	FXDialogBox * findInFilesDialog;
	FXComboBox * fifComboFind;
	FXComboBox * fifComboFiles;
	FXComboBox * fifComboDir;
	FXCheckButton * fifToggleRec;
	FXDialogBox * gotoDialog;
	FXTextField * gotoEntry;

	FXint	fileSelectorWidth;
	FXint	fileSelectorHeight;

	void SetWindowName();
	void ShowFileInStatus();
	void SetIcon();

	virtual void ReadPropertiesInitial();
	virtual void ReadProperties();
	virtual void SizeContentWindows();
	virtual void SizeSubWindows();

	virtual void SetMenuItem(int menuNumber, int position, int itemID,
	                         const char *text, const char *mnemonic = 0);
	virtual void DestroyMenuItem(int menuNumber, int itemID);
	virtual void CheckAMenuItem(int wIDCheckItem, bool val);
	virtual void EnableAMenuItem(int wIDCheckItem, bool val);
	FXWindow * findMenuItem(int wIDCheckItem);
	virtual void CheckMenus();
	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true);
	virtual void ExecuteNext();

	virtual void OpenUriList(const char *list);
	virtual void AbsolutePath(char *absPath, const char *relativePath, int size);
	virtual bool OpenDialog();
	virtual bool SaveAsDialog();
	virtual void SaveAsHTML();
	virtual void SaveAsRTF();
	virtual void SaveAsPDF();
	virtual void Print();
	virtual void PrintSetup();

	virtual int WindowMessageBox(Window &w, const SString &msg, int style);
	virtual void AboutDialog();
	virtual void QuitProgram();
	void FindReplaceGrabFields();
	virtual void Find();
	SString TranslatedMenuLabel(const char *original);
	void TranslatedSetTitle(FXTopWindow *w, const char *original);
	FXLabel *TranslatedLabel(FXComposite *p, const char *original);
	FXButton *TranslatedCommand(FXComposite *p, const char *original, FXObject * tgt, FXSelector sel,
															FXint opt = FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|ICON_BEFORE_TEXT);
	FXCheckButton *TranslatedToggle(FXComposite *p, const char *original, bool active);
	virtual void FindInFiles();
	virtual void Replace();
	virtual void FindReplace(bool replace);
	virtual void DestroyFindReplace();
	virtual void GoLineDialog();
	virtual void TabSizeDialog();
	virtual bool ParametersDialog(bool modal);

	virtual void GetDefaultDirectory(char *directory, size_t size);
	virtual bool GetSciteDefaultHome(char *path, unsigned int lenPath);
	virtual bool GetSciteUserHome(char *path, unsigned int lenPath);
	virtual bool GetDefaultPropertiesFileName(char *pathDefaultProps,
	        char *pathDefaultDir, unsigned int lenPath);
	virtual bool GetUserPropertiesFileName(char *pathUserProps,
	                                       char *pathUserDir, unsigned int lenPath);
	virtual bool GetAbbrevPropertiesFileName(char *pathAbbrevProps,
	        char *pathDefaultDir, unsigned int lenPath);

	virtual void SetStatusBarText(const char *s);
	virtual void ShowToolBar();
	virtual void ShowTabBar();
	virtual void ShowStatusBar();
	void ContinueExecute();

	bool SendPipeCommand(const char *pipeCommand);
	bool CreatePipe(bool forceNew = false);
	void CheckAlreadyOpen(const char *cmdLine);

	virtual void ParamGrab();
public:
	SciTEFOX(Extension *ext);
	~SciTEFOX();

	SString Translated(const char * original);
	void WarnUser(int warnID);
	FXWindow *AddToolButton(const char *text, int cmd, char *icon[]);
	void AddToolBar();
	void CreateMenu();
	void CreateUI();

	virtual void Execute();
	virtual void StopExecute();

	void SetAboutMessage(WindowID wsci, const char *appTitle)
	{	SciTEBase::SetAboutMessage(wsci, appTitle); }
};

// ====================================================================
// SCMessageBox (implementation)
// ====================================================================

FXDEFMAP(SCMessageBox) SCMessageBoxMap[]={
  FXMAPFUNC(SEL_COMMAND,  SCMessageBox::ID_IDOK,	SCMessageBox::onIDOK),
  FXMAPFUNC(SEL_COMMAND,  SCMessageBox::ID_IDYES,	SCMessageBox::onIDYES),
  FXMAPFUNC(SEL_COMMAND,  SCMessageBox::ID_IDNO,	SCMessageBox::onIDNO),
  FXMAPFUNC(SEL_COMMAND,  SCMessageBox::ID_IDCANCEL,	SCMessageBox::onIDCANCEL),
  FXMAPFUNC(SEL_COMMAND,  FXDialogBox::ID_CANCEL,	SCMessageBox::onCANCEL),
  };

FXIMPLEMENT(SCMessageBox,FXDialogBox,SCMessageBoxMap,ARRAYNUMBER(SCMessageBoxMap))

SCMessageBox::SCMessageBox(FXWindow *wParent, const char *m, const char *t, int style) :
	FXDialogBox(static_cast<FXComposite *>(wParent), t),
  	escapeResult(ID_IDOK)
{
	FXVerticalFrame * vFrame = new FXVerticalFrame(this, LAYOUT_FILL_X|LAYOUT_FILL_Y);
	FXHorizontalFrame * hFrame = new FXHorizontalFrame(
		vFrame, LAYOUT_BOTTOM|LAYOUT_CENTER_X|LAYOUT_CENTER_X);
	FXButton * button;
	if ((style & 0xf) == MB_OK) {
		button = new FXButton(hFrame, "&OK", NULL, this, ID_IDOK);
		button->setDefault(true);
	} else {
		button = new FXButton(hFrame, "&Yes", NULL, this, ID_IDYES);
		button->setDefault(true);

		button = new FXButton(hFrame, "&No", NULL, this, ID_IDNO);
		escapeResult = ID_IDNO;
		if (style == MB_YESNOCANCEL) {
			button = new FXButton(hFrame, "&Cancel", NULL, this, ID_IDCANCEL);
			escapeResult = ID_IDCANCEL;
		}
	}
	if (style & MB_ABOUTBOX) {
		FXScintilla *explanation = new FXScintilla(vFrame, NULL, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
		explanation->setScintillaID(0);
		(static_cast<SciTEFOX *>(FXApp::instance()))->SetAboutMessage(explanation, "SciTE");
		resize(480, 380);
	} else {
		new FXLabel(vFrame, m);
	}
};

long SCMessageBox::onIDOK(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, IDOK);
  hide();
  return 1;
}

long SCMessageBox::onIDYES(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, IDYES);
  hide();
  return 1;
}

long SCMessageBox::onIDNO(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, IDNO);
  hide();
  return 1;
}

long SCMessageBox::onIDCANCEL(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, IDCANCEL);
  hide();
  return 1;
}

long SCMessageBox::onCANCEL(FXObject * obj, FXSelector, void * ptr)
{
	return handle(obj, MKUINT(escapeResult, SEL_COMMAND), ptr);
}

// ====================================================================
// FindReplace (implementation)
// ====================================================================

FXIMPLEMENT(SCFindReplace,FXDialogBox,NULL,0)

SCFindReplace::SCFindReplace(FXWindow *wParent, bool replace,
														 const char * findWhat,
														 const ComboMemory & memFinds,
														 const ComboMemory & memReplaces,
														 FXint settings) :
	FXDialogBox(static_cast<FXComposite *>(wParent), TRANSLATED(replace ? "Replace" : "Find")),
	_replacing(replace)
{
	FXMatrix * matrix = new FXMatrix(this, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X);
	new FXLabel(matrix, TRANSLATED("Find what:"), NULL);
	comboFind = new FXComboBox(matrix, 25, 10, NULL, 0, FRAME_SUNKEN);
	FillComboFromMemory(comboFind, memFinds);
	comboFind->setText(findWhat);
	if (_replacing) {
		new FXLabel(matrix, TRANSLATED("Replace with:"));
		comboReplace = new FXComboBox(matrix, 25, 10, NULL, 0, FRAME_SUNKEN);
		FillComboFromMemory(comboReplace, memReplaces);
	}

	wwButton = new FXCheckButton(this, TRANSLATED("Match whole word &only"));
	wwButton->setCheck(settings & SCFR_WHOLEWORD);
	csButton = new FXCheckButton(this, TRANSLATED("&Match case"));
	csButton->setCheck(settings & SCFR_CASESENSITIVE);
	reButton = new FXCheckButton(this, TRANSLATED("Regular E&xpression"));
	reButton->setCheck(settings & SCFR_REGEXP);
	wpButton = new FXCheckButton(this, TRANSLATED("&Wrap around"));
	wpButton->setCheck(settings & SCFR_WRAP);
	usButton = new FXCheckButton(this, TRANSLATED("&Transform backslash expressions"));
	usButton->setCheck(settings & SCFR_UNSLASH);
	rdButton = new FXCheckButton(this, TRANSLATED("Re&verse Direction"));
	rdButton->setCheck(settings & SCFR_REVERSEDIRECTION);

	new FXHorizontalSeparator(this);

	FXHorizontalFrame * hFrame = new FXHorizontalFrame(this, LAYOUT_FILL_X);
	new FXButton(hFrame, TRANSLATED("F&ind"), NULL, FXApp::instance(), SciTEFOX::ID_FRFIND, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|BUTTON_DEFAULT|LAYOUT_CENTER_X);
	if (_replacing) {
		new FXButton(hFrame, TRANSLATED("&Replace"), NULL, FXApp::instance(), SciTEFOX::ID_FRREPLACE, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);
		new FXButton(hFrame, TRANSLATED("Replace &All"), NULL, FXApp::instance(), SciTEFOX::ID_FRREPLACEALL, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);
		new FXButton(hFrame, TRANSLATED("Replace in &Selection"), NULL, FXApp::instance(), SciTEFOX::ID_FRREPLACEINSELECTION, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);
	}
	new FXButton(hFrame, TRANSLATED("&Cancel"), NULL, FXApp::instance(), SciTEFOX::ID_FRCANCEL, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);
}

FXint SCFindReplace::getSettings() const
{
	FXint settings = 0;
	if (csButton->getCheck())
		settings |= SCFR_CASESENSITIVE;
	if (wwButton->getCheck())
		settings |= SCFR_WHOLEWORD;
	if (reButton->getCheck())
		settings |= SCFR_REGEXP;
	if (wpButton->getCheck())
		settings |= SCFR_WRAP;
	if (usButton->getCheck())
		settings |= SCFR_UNSLASH;
	if (rdButton->getCheck())
		settings |= SCFR_REVERSEDIRECTION;
	return settings;
}

FXString SCFindReplace::getFind() const
{
	return comboFind->getText();
}

FXString SCFindReplace::getReplace() const
{
	return comboReplace->getText();
}

// ====================================================================
// SciTEFOX (implementation)
// ====================================================================

FXDEFMAP(SciTEFOX) SciTEFOXMap[]={
  FXMAPFUNCS(SEL_COMMAND, FXApp::ID_LAST, SciTEFOX::IDM_LAST, SciTEFOX::onSciTECommand),
  FXMAPFUNC(SEL_IO_READ, SciTEFOX::ID_PIPESIGNAL, SciTEFOX::onPipeSignal),
  FXMAPFUNC(SEL_IO_READ, SciTEFOX::ID_IOSIGNAL, SciTEFOX::onIOSignal),
  FXMAPFUNC(SEL_COMMAND, FXApp::ID_QUIT, SciTEFOX::onCmdQuit),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FRFIND, SciTEFOX::onFRFind),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FRREPLACE, SciTEFOX::onFRReplace),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FRREPLACEALL, SciTEFOX::onFRReplaceAll),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FRREPLACEINSELECTION, SciTEFOX::onFRReplaceInSelection),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FRCANCEL, SciTEFOX::onFRCancel),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_FIFFIND, SciTEFOX::onFIFFind),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_GTGOTO, SciTEFOX::onGTGoto),
  FXMAPFUNC(SEL_CLOSE, SciTEFOX::ID_PARAM, SciTEFOX::onParamCancel),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_PARAMOK, SciTEFOX::onParamOK),
  FXMAPFUNC(SEL_COMMAND, SciTEFOX::ID_PARAMCANCEL, SciTEFOX::onParamCancel),
  FXMAPFUNCS(SEL_COMMAND, SciTEFOX::ID_EDITOR, SciTEFOX::ID_OUTPUT, SciTEFOX::onCmdNotify),
  FXMAPFUNCS(SEL_FOCUSIN, SciTEFOX::ID_EDITOR, SciTEFOX::ID_OUTPUT, SciTEFOX::onSCFocusIn),
  FXMAPFUNC(SEL_KEYPRESS, 0, SciTEFOX::onKeyPress),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,SciTEFOX::ID_EDITOR,SciTEFOX::onRightBtnPress),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,SciTEFOX::ID_OUTPUT,SciTEFOX::onRightBtnPress),
};

FXIMPLEMENT(SciTEFOX,FXApp,SciTEFOXMap,ARRAYNUMBER(SciTEFOXMap))

long SciTEFOX::onCmdNotify(FXObject * sender, FXSelector, void* ptr)
{
	if (ptr) {
		SCNotification * scn = static_cast<SCNotification *>(ptr);
/*		if (scn->nmhdr.code == SCN_KEY) {
			FXEvent event;
			event.state = 0;
			if (scn->modifiers & SCMOD_SHIFT)
				event.state |= SHIFTMASK;
			if (scn->modifiers & SCMOD_CTRL)
				event.state |= CONTROLMASK;
			event.code = scn->ch;
			return handle(sender, MKUINT(0, SEL_KEYPRESS), &event);
		}*/
		Notify(static_cast<SCNotification *>(ptr));
		return 1;
	}
	else
		return 0;
}

long SciTEFOX::onSciTECommand(FXObject *, FXSelector sel, void*)
{
	int id = SELID(sel) - ID(0);
	switch (id) {
	case IDM_FULLSCREEN:
		fullScreen = !fullScreen;
		if (fullScreen) {
			saved.x = mainWindow->getX();
			saved.y = mainWindow->getY();
			saved.w = mainWindow->getWidth();
			saved.h = mainWindow->getHeight();

			mainWindow->position(0, 0, getRoot()->getWidth(), getRoot()->getHeight());
			SizeSubWindows();
		} else {
			mainWindow->position(saved.x, saved.y, saved.w, saved.h);
			SizeSubWindows();
		}
		CheckMenus();
		break;

	default:
		MenuCommand(SELID(sel)-ID(0));
	}
	UpdateStatusBar(true);
	return 1;
}

long SciTEFOX::onCmdQuit(FXObject *, FXSelector, void*)
{
	QuitProgram();
	return 1;
}

long SciTEFOX::onSCFocusIn(FXObject * sender, FXSelector, void*)
{
	focus = static_cast<FXWindow *>(sender);
	CheckMenus();
	return 1;
}

SciTEFOX::SciTEFOX(Extension *ext) :
	SciTEBase(ext),
	FXApp("SciTE", "Scintilla"),
	buffersMenu(0),
	frDialog(NULL),
	focus(NULL),
	findInFilesDialog(NULL)
{
	// Control of sub process
	icmd = 0;
	originalEnd = 0;
	fdFIFO = 0;
	pidShell = 0;
	sprintf(resultsFile, "/tmp/SciTE%x.results",
	        static_cast<int>(getpid()));

	propsEmbed.Set("PLAT_GTK", "1");
	
	ReadGlobalPropFile();

	compile_btn = 0;
	build_btn = 0;
	stop_btn = 0;

	// Fullscreen handling
	fullScreen = false;
}

SciTEFOX::~SciTEFOX()
{
	delete fileExportMenu;
	delete fileMenu;
	delete editMenu;
	delete searchMenu;
	delete viewMenu;
	delete toolsMenu;
	delete lineEndCharactersMenu;
	delete languageMenu;
	delete editPropsMenu;
	delete optionsMenu;
	delete buffersMenu;
	delete helpMenu;
}

void SciTEFOX::WarnUser(int)
{
}

void SciTEFOX::GetDefaultDirectory(char *directory, size_t size) {
	directory[0] = '\0';
	char *where = getenv("SciTE_HOME");
#ifdef SYSCONF_PATH
	if (!where) {
		where = SYSCONF_PATH;
	}
#else
	if (!where) {
		where = getenv("HOME");
	}
#endif
	if (where)
#ifdef __vms
		strncpy(directory, VMSToUnixStyle(where), size);
#else
		strncpy(directory, where, size);
#endif
	directory[size - 1] = '\0';
}

bool SciTEFOX::GetSciteDefaultHome(char *path, unsigned int lenPath) {
	char *where = getenv("SciTE_HOME");
#ifdef __vms
	if (where == NULL) {
		where = g_modulePath;
	}
#endif
#ifdef SYSCONF_PATH
	if (!where) {
		where = SYSCONF_PATH;
	}
#else
	if (!where) {
		where = getenv("HOME");
	}
#endif
	if (where) {
#ifdef __vms
		strncpy(path, VMSToUnixStyle(where), lenPath);
#else
		strncpy(path, where, lenPath);
#endif
		return true;
	}
	return false;
}

bool SciTEFOX::GetDefaultPropertiesFileName(char *pathDefaultProps,
        char *pathDefaultDir, unsigned int lenPath) {
	if (!GetSciteDefaultHome(pathDefaultDir, lenPath))
		return false;
	if (strlen(pathDefaultProps) + 1 + strlen(propGlobalFileName) < lenPath) {
		strncpy(pathDefaultProps, pathDefaultDir, lenPath);
		strncat(pathDefaultProps, pathSepString, lenPath);
		strncat(pathDefaultProps, propGlobalFileName, lenPath);
		return true;
	}
	return false;
}

bool SciTEFOX::GetAbbrevPropertiesFileName(char *pathAbbrevProps,
        char *pathDefaultDir, unsigned int lenPath) {
	if (!GetSciteDefaultHome(pathDefaultDir, lenPath))
		return false;
	if (strlen(pathAbbrevProps) + 1 + strlen(propAbbrevFileName) < lenPath) {
		strncpy(pathAbbrevProps, pathDefaultDir, lenPath);
		strncat(pathAbbrevProps, pathSepString, lenPath);
		strncat(pathAbbrevProps, propAbbrevFileName, lenPath);
		return true;
	}
	return false;
}

bool SciTEFOX::GetSciteUserHome(char *path, unsigned int lenPath) {
	char *where = getenv("SciTE_HOME");
	if (!where) {
		where = getenv("HOME");
	}
	if (where) {
		strncpy(path, where, lenPath);
		return true;
	}
	return false;
}

bool SciTEFOX::GetUserPropertiesFileName(char *pathUserProps,
        char *pathUserDir, unsigned int lenPath) {
	if (!GetSciteUserHome(pathUserDir, lenPath))
		return false;
	if (strlen(pathUserProps) + 1 + strlen(propUserFileName) < lenPath) {
		strncpy(pathUserProps, pathUserDir, lenPath);
		strncat(pathUserProps, pathSepString, lenPath);
		strncat(pathUserProps, propUserFileName, lenPath);
		return true;
	}
	return false;
}

void SciTEFOX::ShowFileInStatus() {
	char sbText[1000];
	sprintf(sbText, " File: ");
	if (fileName[0] == '\0')
		strcat(sbText, "Untitled");
	else
		strcat(sbText, fullPath);
	SetStatusBarText(sbText);
}

void SciTEFOX::SetWindowName() {
	SciTEBase::SetWindowName();
	ShowFileInStatus();
}

void SciTEFOX::SetStatusBarText(const char *s) {
	statusBar->getStatusline()->setNormalText(s);
}

void SciTEFOX::ShowToolBar() {
	toolBar->handle(
		this,
		MKUINT((tbVisible) ? FXWindow::ID_SHOW : FXWindow::ID_HIDE, SEL_COMMAND),
		NULL);
}

void SciTEFOX::ShowTabBar() {
	SizeSubWindows();
}

void SciTEFOX::ShowStatusBar() {
	statusBar->handle(
		this,
		MKUINT((sbVisible) ? FXWindow::ID_SHOW : FXWindow::ID_HIDE, SEL_COMMAND),
		NULL);
}

void SciTEFOX::ReadPropertiesInitial() {
	SciTEBase::ReadPropertiesInitial();
	ShowToolBar();
	ShowStatusBar();
}

void SciTEFOX::ReadProperties() {
	SciTEBase::ReadProperties();
	CheckMenus();
}
#define CLIENT_3D_EFFECT
void SciTEFOX::SizeContentWindows() {
	PRectangle rcClient = GetClientRectangle();
	int w = rcClient.right - rcClient.left;
	int h = rcClient.bottom - rcClient.top;
	heightOutput = NormaliseSplit(heightOutput);
#ifdef CLIENT_3D_EFFECT
	wOutput.GetID()->getParent()->handle(
		this,
		MKUINT((heightOutput) ? FXWindow::ID_SHOW : FXWindow::ID_HIDE, SEL_COMMAND),
		NULL);
#else
	wOutput.GetID()->handle(
		this,
		MKUINT((heightOutput) ? FXWindow::ID_SHOW : FXWindow::ID_HIDE, SEL_COMMAND),
		NULL);
#endif
}

void SciTEFOX::SizeSubWindows() {
	SizeContentWindows();
}

FXWindow * SciTEFOX::findMenuItem(int wIDCheckItem)
{
	vector<FXMenuPane *> panes;
	panes.push_back(fileMenu);
	panes.push_back(fileExportMenu);
	panes.push_back(editMenu);
	panes.push_back(searchMenu);
	panes.push_back(viewMenu);
	panes.push_back(toolsMenu);
	panes.push_back(optionsMenu);
	panes.push_back(buffersMenu);
	panes.push_back(helpMenu);
	panes.push_back(lineEndCharactersMenu);
	panes.push_back(languageMenu);
	panes.push_back(editPropsMenu);

	for (vector<FXMenuPane *>::iterator it = panes.begin(); it != panes.end(); it++) {
		FXMenuPane * pane = (*it);
		if (pane)
			for (FXWindow * item = pane->getFirst(); item; item = item->getNext()) {
				if ((item->getTarget() == this) && item->getSelector() == ID(wIDCheckItem))
					return item;
			}
	}
	return NULL;
}


void SciTEFOX::SetMenuItem(int, int, int itemID, const char *text, const char *) {
	DestroyMenuItem(0, itemID);

	FXMenuCommand * item =
		static_cast<FXMenuCommand *>(findMenuItem(itemID));
	if (item) {
		FXString tip = item->getText().extract(1, '\t') ;
		item->setText(FXString(text) + tip);
		item->show();
	}
}

void SciTEFOX::DestroyMenuItem(int, int itemID) {
	if (itemID) {
		FXMenuCommand * item =
			static_cast<FXMenuCommand *>(findMenuItem(itemID));
		if (item) {
			item->hide();
		}
	}
}

void SciTEFOX::CheckAMenuItem(int wIDCheckItem, bool val) {
	FXMenuCommand * item =
		static_cast<FXMenuCommand *>(findMenuItem(wIDCheckItem));
	if (item)
		if (val)
			item->check();
		else
			item->uncheck();
}

void SciTEFOX::EnableAMenuItem(int wIDCheckItem, bool val) {
	FXMenuCommand * item =
		static_cast<FXMenuCommand *>(findMenuItem(wIDCheckItem));
	if (item) {
		//Platform::DebugPrintf("Set %d to %d\n", wIDCheckItem, val);
		item->handle(
			this,
			MKUINT((val) ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE, SEL_COMMAND),
			NULL);
	}
}

void SciTEFOX::CheckMenus() {
	SciTEBase::CheckMenus();

	CheckAMenuItem(IDM_EOL_CRLF, SendEditor(SCI_GETEOLMODE) == SC_EOL_CRLF);
	CheckAMenuItem(IDM_EOL_CR, SendEditor(SCI_GETEOLMODE) == SC_EOL_CR);
	CheckAMenuItem(IDM_EOL_LF, SendEditor(SCI_GETEOLMODE) == SC_EOL_LF);

	CheckAMenuItem(IDM_VIEWSTATUSBAR, sbVisible);

	if (build_btn) {
		build_btn->handle(
			this,
			MKUINT((!executing) ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE, SEL_COMMAND),
		NULL);
		compile_btn->handle(
			this,
			MKUINT((!executing) ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE, SEL_COMMAND),
			NULL);
		stop_btn->handle(
			this,
			MKUINT((executing) ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE, SEL_COMMAND),
			NULL);
	}
}

char *split(char*& s, char c) {
	char *t = s;
	if (s && (s = strchr(s, c))) * s++ = '\0';
	return t;
}

/**
 * Replace any %xx escapes by their single-character equivalent.
 */
static void unquote(char *s) {
	char *o = s;
	while (*s) {
		if ((*s == '%') && s[1] && s[2]) {
			*o = IntFromHexDigit(s[1]) * 16 + IntFromHexDigit(s[2]);
			s += 2;
		} else {
			*o = *s;
		}
		o++;
		s++;
	}
	*o = '\0';
}

/**
 * Open a list of URIs each terminated by "\r\n".
 * Only "file:" URIs currently understood.
 */
void SciTEFOX::OpenUriList(const char *list) {
	if (list) {
		char *uri = StringDup(list);
		if (uri) {
			char *enduri = strchr(uri, '\r');
			while (enduri) {
				*enduri = '\0';
				if (isprefix(uri, "file:")) {
					uri += strlen("file:");
					if (isprefix(uri, "///")) {
						uri += 2;	// There can be an optional // before the file path that starts with /
					}

					unquote(uri);
					Open(uri);
				} else {
					SString msg = LocaliseMessage("URI '^0' not understood.");
					WindowMessageBox(wSciTE, msg, MB_OK | MB_ICONWARNING);
				}

				uri = enduri + 1;
				if (*uri == '\n')
					uri++;
				enduri = strchr(uri, '\r');
			}
		}
	}
}

// on Linux return the shortest path equivalent to pathname (remove . and ..)
void SciTEFOX::AbsolutePath(char *absPath, const char *relativePath, int /*size*/) {
	char path[MAX_PATH + 1], *cur, *last, *part, *tmp;
	if (!absPath)
		return;
	if (!relativePath)
		return;
	strcpy(path, relativePath);
	cur = absPath;
	*cur = '\0';
	tmp = path;
	last = NULL;
	if (*tmp == pathSepChar) {
		*cur++ = pathSepChar;
		*cur = '\0';
		tmp++;
	}
	while ((part = split(tmp, pathSepChar))) {
		if (strcmp(part, ".") == 0)
			;
		else if (strcmp(part, "..") == 0 && (last = strrchr(absPath, pathSepChar))) {
			if (last > absPath)
				cur = last;
			else
				cur = last + 1;
			*cur = '\0';
		} else {
			if (cur > absPath && *(cur - 1) != pathSepChar) *cur++ = pathSepChar;
			strcpy(cur, part);
			cur += strlen(part);
		}
	}

	// Remove trailing backslash(es)
	while (absPath < cur - 1 && *(cur - 1) == pathSepChar && *(cur - 2) != ':') {
		*--cur = '\0';
	}
}

bool SciTEFOX::OpenDialog() {
	FXString filename = FXFileDialog::getOpenFilename(mainWindow, "Open File", "");
	if (filename != "")
		Open(filename.text());
	return (filename != "");
}

bool SciTEFOX::SaveAsDialog() {
	FXString filename = FXFileDialog::getSaveFilename(mainWindow, LocaliseString("Save File As").c_str(), "");
	if (filename != "")
		SaveAs(const_cast<char *>(filename.text()));
	return (filename != "");
}

void SciTEFOX::SaveAsHTML() {
	FXString filename = FXFileDialog::getSaveFilename(mainWindow, LocaliseString("Export File As HTML").c_str(), "", "*.html");
	if (filename != "")
		SaveToHTML(const_cast<char *>(filename.text()));
}

void SciTEFOX::SaveAsRTF() {
	FXString filename = FXFileDialog::getSaveFilename(mainWindow, LocaliseString("Export File As RTF").c_str(), "", "*.rtf");
	if (filename != "")
		SaveToRTF(const_cast<char *>(filename.text()));
}

void SciTEFOX::SaveAsPDF() {
	FXString filename = FXFileDialog::getSaveFilename(mainWindow, LocaliseString("Export File As PDF").c_str(), "", "*.pdf");
	if (filename != "")
		SaveToPDF(const_cast<char *>(filename.text()));
}

void SciTEFOX::Print() {
	WindowMessageBox(wSciTE, "Printing not yet supported on FOX", MB_OK);
}

void SciTEFOX::PrintSetup() {
	WindowMessageBox(wSciTE, "Printing not yet supported on FOX", MB_OK);
}

void SciTEFOX::Find() {
	if (focus)
		focus->setFocus();
	SelectionIntoFind();
	FindReplace(false);
}

static SString Padded(const SString &s) {
	SString ret(s);
	ret.insert(0, "  ");
	ret += "  ";
	return ret;
}

SString SciTEFOX::Translated(const char * original)
{
	return LocaliseString(original);
}

void SciTEFOX::TranslatedSetTitle(FXTopWindow *w, const char *original) {
	w->setTitle(Translated(original).c_str());
}

FXLabel *SciTEFOX::TranslatedLabel(FXComposite *p, const char *original) {
	SString text = Translated(original);
	return new FXLabel(p, text.c_str(), NULL);
}

FXButton *SciTEFOX::TranslatedCommand(FXComposite *p, const char *original, FXObject * tgt, FXSelector sel,
																			FXint opt) {
	return new FXButton(p, Padded(Translated(original)).c_str(), NULL, tgt, sel, opt);
}

FXCheckButton *SciTEFOX::TranslatedToggle(FXComposite *p, const char *original, bool active) {
	FXCheckButton * cb = new FXCheckButton(p, Translated(original).c_str(), NULL);
	if (active) {
		cb->setCheck();
	}
	return cb;
}

long SciTEFOX::onFIFFind(FXObject *, FXSelector, void *)
{
	FXString findEntry = fifComboFind->getText();
	props.Set("find.what", findEntry.text());
	memFinds.Insert(findEntry.text());

	FXString dirEntry = fifComboDir->getText();
	props.Set("find.directory", dirEntry.text());
	memDirectory.Insert(dirEntry.text());

#ifdef RECURSIVE_GREP_WORKING
	if (fifToggleRec->getCheck())
		props.Set("find.recursive", props.Get("find.recursive.recursive").c_str());
	else
		props.Set("find.recursive", props.Get("find.recursive.not").c_str());
#endif

	FXString filesEntry = fifComboFiles->getText();
	props.Set("find.files", filesEntry.text());
	memFiles.Insert(filesEntry.text());

	stopModal(findInFilesDialog, 0);
  findInFilesDialog->hide();

	//printf("Grepping for <%s> in <%s>\n",
	//	props.Get("find.what"),
	//	props.Get("find.files"));
	SelectionIntoProperties();
	AddCommand(props.GetNewExpand("find.command", ""), props.Get("find.directory"), jobCLI);
	if (commandCurrent > 0)
		Execute();
	return 1;
}

void SciTEFOX::FindInFiles() {
	if (focus)
		focus->setFocus();
	SelectionIntoFind();
	props.Set("find.what", findWhat.c_str());

	char findInDir[1024];
	getcwd(findInDir, sizeof(findInDir));
	props.Set("find.directory", findInDir);

	findInFilesDialog = new FXDialogBox(mainWindow, "Find In Files");
	TranslatedSetTitle(findInFilesDialog, "Find In Files");

	FXMatrix * matrix = new FXMatrix(findInFilesDialog, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X);

	TranslatedLabel(matrix, "Find what:");

	fifComboFind = new FXComboBox(matrix, 25, 10, NULL, 0, FRAME_SUNKEN);
	FillComboFromMemory(fifComboFind, memFinds);
	fifComboFind->setText(findWhat.c_str());

	TranslatedLabel(matrix, "Files:");

	fifComboFiles = new FXComboBox(matrix, 25, 10, NULL, 0, FRAME_SUNKEN);
	FillComboFromMemory(fifComboFiles, memFiles, true);

	TranslatedLabel(matrix, "Directory:");

	fifComboDir = new FXComboBox(matrix, 25, 10, NULL, 0, FRAME_SUNKEN);
	FillComboFromMemory(fifComboDir, memDirectory);
	fifComboDir->setText(findInDir);

#ifdef RECURSIVE_GREP_WORKING
	fifToggleRec = TranslatedToggle(findInFilesDialog, "Re&cursive Directories", false);
#endif

	new FXHorizontalSeparator(findInFilesDialog);

	FXHorizontalFrame * hFrame = new FXHorizontalFrame(findInFilesDialog, LAYOUT_FILL_X);
	TranslatedCommand(hFrame, "F&ind", this, SciTEFOX::ID_FIFFIND, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|BUTTON_DEFAULT|LAYOUT_CENTER_X);
	TranslatedCommand(hFrame, "&Cancel", this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);

	findInFilesDialog->execute(PLACEMENT_SCREEN);
	delete findInFilesDialog;
	findInFilesDialog = NULL;
}

void SciTEFOX::Replace() {
	if (focus)
		focus->setFocus();
	SelectionIntoFind();
	FindReplace(true);
}

void SciTEFOX::ExecuteNext() {
	icmd++;
	if (icmd < commandCurrent && icmd < commandMax) {
		Execute();
	} else {
		icmd = 0;
		executing = false;
		CheckReload();
		CheckMenus();
		ClearJobQueue();
	}
}

void SciTEFOX::ContinueExecute() {
	char buf[8192];
	int count = 0;
	count = read(fdFIFO, buf, sizeof(buf) - 1);
	if (count < 0) {
		OutputAppendString(">End Bad\n");
		return;
	}
	if (count == 0) {
		int exitcode;
		wait(&exitcode);
		SString sExitMessage(exitcode);
		sExitMessage.insert(0, ">Exit code: ");
		if (timeCommands) {
			sExitMessage += "    Time: ";
			sExitMessage += SString(commandTime.Duration(), 3);
		}
		sExitMessage.append("\n");
		OutputAppendString(sExitMessage.c_str());
		// Move selection back to beginning of this run so that F4 will go
		// to first error of this run.
		if (scrollOutput && returnOutputToCommand)
			SendOutput(SCI_GOTOPOS, originalEnd);
		returnOutputToCommand = true;
		FXApp::instance()->removeInput(fdFIFO, INPUT_READ);
		close(fdFIFO);
		unlink(resultsFile);
		ExecuteNext();
	}
	if (count > 0) {
		buf[count] = '\0';
		OutputAppendString(buf);
		if (count < 0) {
			OutputAppendString(">Continue no data\n");
		}
	}
}

long SciTEFOX::onIOSignal(FXObject *, FXSelector, void *) {
	ContinueExecute();
	return 1;
}

int xsystem(const char *s, const char *resultsFile) {
	int pid = 0;
	//printf("xsystem %s %s\n", s, resultsFile);
	if ((pid = fork()) == 0) {
		close(0);
		int fh = open(resultsFile, O_WRONLY);
		close(1);
		dup(fh);
		close(2);
		dup(fh);

		execlp("/bin/sh", "sh", "-c", s, 0);
		exit(127);
	}
	return pid;
}

void SciTEFOX::Execute() {
	SciTEBase::Execute();

	commandTime.Duration(true);
	if (scrollOutput)
		SendOutput(SCI_GOTOPOS, SendOutput(SCI_GETTEXTLENGTH));
	originalEnd = SendOutput(SCI_GETCURRENTPOS);

	OutputAppendString(">");
	OutputAppendString(jobQueue[icmd].command.c_str());
	OutputAppendString("\n");

	unlink(resultsFile);
	if (jobQueue[icmd].directory != "") {
		chdir(jobQueue[icmd].directory.c_str());
	}

	if (jobQueue[icmd].jobType == jobShell) {
		if (fork() == 0)
			execlp("/bin/sh", "sh", "-c", jobQueue[icmd].command.c_str(), 0);
		else
			ExecuteNext();
	} else if (jobQueue[icmd].jobType == jobExtension) {
		if (extender)
			extender->OnExecute(jobQueue[icmd].command.c_str());
	} else {
#ifndef __vms
		if (mkfifo(resultsFile, S_IRUSR | S_IWUSR) < 0) {
#else           // no mkfifo on OpenVMS!
                creat (resultsFile, 0777);
                if (jobQueue[icmd].jobType == jobShell) {   // Always false!
#endif
			OutputAppendString(">Failed to create FIFO\n");
			ExecuteNext();
			return;
		}

		pidShell = xsystem(jobQueue[icmd].command.c_str(), resultsFile);
		fdFIFO = open(resultsFile, O_RDONLY | O_NONBLOCK);
		if (fdFIFO < 0) {
			OutputAppendString(">Failed to open\n");
			return;
		}
		FXApp::instance()->addInput(fdFIFO, INPUT_READ, this, ID_IOSIGNAL);
	}
}

void SciTEFOX::StopExecute() {
	kill(pidShell, SIGKILL);
}

long SciTEFOX::onGTGoto(FXObject *, FXSelector, void *)
{
	FXString lineEntry = gotoEntry->getText();
	int lineNo = atoi(lineEntry.text());

	stopModal(gotoDialog, 0);
  gotoDialog->hide();

	GotoLineEnsureVisible(lineNo - 1);
	return 1;
}

void SciTEFOX::GoLineDialog() {
	gotoDialog = new FXDialogBox(mainWindow, "Go To");
	TranslatedSetTitle(gotoDialog, "Go To");

	FXMatrix * matrix = new FXMatrix(gotoDialog, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X);

	TranslatedLabel(matrix, "Go to line:");
	gotoEntry = new FXTextField(matrix, 10, NULL, 0, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);

	new FXHorizontalSeparator(gotoDialog);

	FXHorizontalFrame * hFrame = new FXHorizontalFrame(gotoDialog, LAYOUT_FILL_X);
	TranslatedCommand(hFrame, "Go To", this, SciTEFOX::ID_GTGOTO, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|BUTTON_DEFAULT|LAYOUT_CENTER_X);
	TranslatedCommand(hFrame, "Cancel", this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);

	gotoDialog->execute(PLACEMENT_SCREEN);
	delete gotoDialog;
	gotoDialog = NULL;
}

void SciTEFOX::TabSizeDialog() {}

void SciTEFOX::ParamGrab() {
	if (wParameters.Created()) {
		for (int param=0; param<maxParam; param++) {
			SString paramText(param+1);
			FXString paramVal = entryParam[param]->getText();
			props.Set(paramText.c_str(), paramVal.text());
		}
	}
}

long SciTEFOX::onParamCancel(FXObject *, FXSelector, void *)
{
	dialogCanceled = true;
	wParameters.Destroy();
	CheckMenus();
	return 1;
}

long SciTEFOX::onParamOK(FXObject *, FXSelector, void *)
{
	dialogCanceled = false;
	ParamGrab();
	wParameters.Destroy();
	CheckMenus();
	return 1;
}

bool SciTEFOX::ParametersDialog(bool modal) {
	if (wParameters.Created()) {
		ParamGrab();
		if (!modal) {
			wParameters.Destroy();
		}
		return true;
	}
	FXDialogBox * wp = new FXDialogBox(mainWindow, "Parameters");
	wp->setTarget(this);
	wp->setSelector(ID_PARAM);
	wParameters = wp;
	TranslatedSetTitle(wp, "Parameters");

	FXMatrix * matrix = new FXMatrix(wp, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X);

	if (modal) {
		new FXLabel(matrix, parameterisedCommand.c_str(), NULL);
		new FXLabel(matrix, "", NULL);
	}
	
	for (int param=0; param<maxParam; param++) {
		SString paramText(param+1);
		SString paramTextVal = props.Get(paramText.c_str());
		paramText.append(":");
		new FXLabel(matrix, paramText.c_str(), NULL);

		entryParam[param] = new FXTextField(matrix, 20, NULL, 0, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
		entryParam[param]->setText(paramTextVal.c_str());
	}

	new FXHorizontalSeparator(wp);

	FXHorizontalFrame * hFrame = new FXHorizontalFrame(wp, LAYOUT_FILL_X);
	TranslatedCommand(hFrame, modal ? "&Execute" : "&Set", this, SciTEFOX::ID_PARAMOK, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|BUTTON_DEFAULT|LAYOUT_CENTER_X);
	TranslatedCommand(hFrame, modal ? "_Cancel" : "_Close", this, SciTEFOX::ID_PARAMCANCEL, FRAME_RAISED|FRAME_THICK|JUSTIFY_NORMAL|LAYOUT_CENTER_X);

	if (modal) {
		wp->execute(PLACEMENT_SCREEN);
	}
	else {
		wp->create();
		wp->show();
	}
	return !dialogCanceled;
}

long SciTEFOX::onFRFind(FXObject *, FXSelector, void *)
{
	FindReplaceGrabFields();
	if (!frDialog->replacing())
		DestroyFindReplace();
	if (findWhat[0]) {
		FindNext(reverseFind);
	}
	return 1;
}

long SciTEFOX::onFRReplace(FXObject *, FXSelector, void *)
{
	FindReplaceGrabFields();
	if (findWhat[0]) {
		ReplaceOnce();
	}
	return 1;
}

long SciTEFOX::onFRReplaceAll(FXObject *, FXSelector, void *)
{
	FindReplaceGrabFields();
	if (findWhat[0]) {
		ReplaceAll(false);
		DestroyFindReplace();
	}
	return 1;
}

long SciTEFOX::onFRReplaceInSelection(FXObject *, FXSelector, void *)
{
	FindReplaceGrabFields();
	if (findWhat[0]) {
		ReplaceAll(true);
		DestroyFindReplace();
	}
	return 1;
}

long SciTEFOX::onFRCancel(FXObject *, FXSelector, void *)
{
	DestroyFindReplace();
	return 1;
}

void SciTEFOX::FindReplace(bool replace) {
	replacing = replace;
	if (frDialog && replacing != frDialog->replacing())
		DestroyFindReplace();
	if (!frDialog) {
		FXint settings = 0;
		if (matchCase)
			settings |= SCFindReplace::SCFR_CASESENSITIVE;
		if (wholeWord)
			settings |= SCFindReplace::SCFR_WHOLEWORD;
		if (regExp)
			settings |= SCFindReplace::SCFR_REGEXP;
		if (wrapFind)
			settings |= SCFindReplace::SCFR_WRAP;
		if (unSlash)
			settings |= SCFindReplace::SCFR_UNSLASH;
		if (reverseFind)
			settings |= SCFindReplace::SCFR_REVERSEDIRECTION;
		frDialog = new SCFindReplace(mainWindow, replacing, findWhat.c_str(), memFinds, memReplaces, settings);
	  frDialog->create();
	}
  frDialog->show(PLACEMENT_SCREEN);
}

void SciTEFOX::DestroyFindReplace() {
	delete frDialog;
	frDialog = NULL;
}

void SciTEFOX::FindReplaceGrabFields() {
	findWhat = frDialog->getFind().text();
	memFinds.Insert(findWhat);
	if (frDialog->replacing()) {
		replaceWhat = frDialog->getReplace().text();
		memReplaces.Insert(replaceWhat);
	}
	FXint settings = frDialog->getSettings();
	wholeWord = settings & SCFindReplace::SCFR_WHOLEWORD;
	matchCase = settings & SCFindReplace::SCFR_CASESENSITIVE;
	regExp = settings & SCFindReplace::SCFR_REGEXP;
	wrapFind = settings & SCFindReplace::SCFR_WRAP;
	unSlash = settings & SCFindReplace::SCFR_UNSLASH;
	reverseFind = settings & SCFindReplace::SCFR_REVERSEDIRECTION;
}

int SciTEFOX::WindowMessageBox(Window & wParent, const SString & m, int style) {
	SCMessageBox mb(wParent.GetID(), m.c_str(), appName, style);
	return mb.execute(PLACEMENT_SCREEN);
}

void SciTEFOX::AboutDialog() {
	WindowMessageBox(wSciTE, "SciTE\nby Neil Hodgson neilh@scintilla.org .",
	           MB_OK | MB_ABOUTBOX);
}

void SciTEFOX::QuitProgram() {
	if (SaveIfUnsureAll() != IDCANCEL) {

#ifndef NO_FILER
		int fdPipe = props.GetInt("scite.ipc_fdpipe");
		if( fdPipe != -1 ){
			close(fdPipe);
			unlink(props.Get("scite.ipc_name").c_str());
		}
#else
		//clean up any pipes that are ours
		if (fdPipe != -1) {
			//printf("Cleaning up pipe\n");
			close(fdPipe);
			unlink(pipeName);
		}
#endif
		popup.Destroy();
		exit(0);
	}
}

// <FIXME> Pas sûr d'avoir besoin de ces handler de raccourcis
// Translate key strokes that are not in a menu into commands
class KeyToCommand {
public:
	int modifiers;
	unsigned int key;	// For alphabetic keys has to match the shift modifier.
	int msg;
};

enum {
	m__ = 0,
	mS_ = SHIFTMASK,
	m_C = CONTROLMASK,
	mSC = SHIFTMASK | CONTROLMASK
};

static KeyToCommand kmap[] = {
	{m_C,	KEY_Tab,			IDM_NEXTFILE},
	{mSC,	KEY_ISO_Left_Tab,	IDM_PREVFILE},
	{m_C,	KEY_KP_Enter,		IDM_COMPLETEWORD},
	{m_C,	KEY_F3,			IDM_FINDNEXTSEL},
	{mSC,	KEY_F3,			IDM_FINDNEXTBACKSEL},
	{m_C,	'j',				IDM_PREVMATCHPPC},
	{mSC,	'J',				IDM_SELECTTOPREVMATCHPPC},
	{m_C,	'k',				IDM_NEXTMATCHPPC},
	{mSC,	'K',				IDM_SELECTTONEXTMATCHPPC},
	{0,0,0},
};

long SciTEFOX::onKeyPress(FXObject *, FXSelector, void * ptr)
{
	FXEvent * event = (FXEvent *)ptr;
	//printf("S-key: %d %x %x %x %x\n",event->keyval, event->state, GDK_SHIFT_MASK, GDK_CONTROL_MASK, GDK_F3);
	int modifiers = event->state & (SHIFTMASK | CONTROLMASK);
	int commandID = 0;
	for (int i=0; kmap[i].msg; i++) {
		if ((event->code == kmap[i].key) && (modifiers == kmap[i].modifiers)) {
			commandID = kmap[i].msg;
		}
	}
	if (commandID) {
		onSciTECommand(this, MKUINT(ID(commandID), SEL_COMMAND), NULL);
		return 1;
	}
	return 0;
}
// </FIXME>

void SetFocus(FXWindow *hwnd) {
	hwnd->setFocus();
}

void SciTEFOX::AddToPopUp(const char *label, int cmd, bool enabled) {
	FXMenuPane * mp = static_cast<FXMenuPane *>(popup.GetID());
	if (!label[0]) {
		new FXMenuSeparator(mp);
	}
	else {
		SString localised = LocaliseString(label);
		FXMenuCommand * mc = new FXMenuCommand(mp, localised.c_str(), NULL, cmd ? this : NULL, cmd ? ID(cmd) : 0);
		if (!enabled)
			mc->disable();
	}
}

long SciTEFOX::onRightBtnPress(FXObject * sender, FXSelector sel, void * ptr) {
	FXEvent * event = static_cast<FXEvent *>(ptr);
	FXWindow * win = static_cast<FXWindow *>(sender);
	Window w;
	if (win == wEditor.GetID()) {
		w = wEditor;
	}
	else if (win == wOutput.GetID()) {
		w = wOutput;
	}
	else
		return 0;

	ContextMenu(w, Point(event->rootclick_x, event->rootclick_y), wSciTE);
	return 1;
}

FXWindow *SciTEFOX::AddToolButton(const char *text, int cmd, char *icon[]) {
	FXString tip = "\t";
	tip += text;
	return new FXButton(
		toolBar, tip,
		new FXXPMIcon(this, (const FXchar **)icon),
		this, ID(cmd),
		ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
}

void SciTEFOX::AddToolBar() {
	AddToolButton("New", IDM_NEW, filenew_xpm);
	AddToolButton("Open", IDM_OPEN, fileopen_xpm);
	AddToolButton("Save", IDM_SAVE, filesave_xpm);
	AddToolButton("Close", IDM_CLOSE, close_xpm);

	new FXFrame(toolBar,FRAME_SUNKEN|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,3,15);
	AddToolButton("Undo", IDM_UNDO, undo_xpm);
	AddToolButton("Redo", IDM_REDO, redo_xpm);
	AddToolButton("Cut", IDM_CUT, editcut_xpm);
	AddToolButton("Copy", IDM_COPY, editcopy_xpm);
	AddToolButton("Paste", IDM_PASTE, editpaste_xpm);

	new FXFrame(toolBar,FRAME_SUNKEN|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,3,15);
	AddToolButton("Find in Files", IDM_FINDINFILES, findinfiles_xpm);
	AddToolButton("Find", IDM_FIND, search_xpm);
	AddToolButton("Find Next", IDM_FINDNEXT, findnext_xpm);
	AddToolButton("Replace", IDM_REPLACE, replace_xpm);

	new FXFrame(toolBar,FRAME_SUNKEN|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,3,15);
	compile_btn = AddToolButton("Compile", IDM_COMPILE, compile_xpm);
	build_btn = AddToolButton("Build", IDM_BUILD, build_xpm);
	stop_btn = AddToolButton("Stop", IDM_STOPEXECUTE, stop_xpm);

	new FXFrame(toolBar,FRAME_SUNKEN|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,3,15);
	AddToolButton("Previous Buffer", IDM_PREVFILE, prev_xpm);
	AddToolButton("Next Buffer", IDM_NEXTFILE, next_xpm);
}

SString SciTEFOX::TranslatedMenuLabel(const char *original)
{
	SString label(original);
	int sep = label.search("\t");
	SString title(label.c_str(), 0, sep);
	SString ttitle = LocaliseString(title.c_str());
	return (sep >= 0) ? ttitle += SString(label.c_str() + sep) : ttitle;
}
	
void SciTEFOX::CreateMenu() {
  FXAccelTable * table = mainWindow->getAccelTable();
	FXMenuCommand * mc;
	FXToolbarShell * tbshell = new FXToolbarShell(mainWindow, FRAME_RAISED|FRAME_THICK);
	
	menuBar = new FXMenubar(mainWindow, tbshell, FRAME_RAISED|FRAME_THICK|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
	
	fileMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar,TranslatedMenuLabel("&File").c_str(),NULL,fileMenu);
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("&New\tCtl-N").c_str(),NULL,this,ID(IDM_NEW));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("&Open...\tCtl-O").c_str(),NULL,this,ID(IDM_OPEN));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("Open Selected &Filename\tCtl-Shift-O").c_str(),NULL,this,ID(IDM_OPENSELECTED));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("&Revert\tCtl-R").c_str(),NULL,this,ID(IDM_REVERT));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("&Close\tCtl-W").c_str(),NULL,this,ID(IDM_CLOSE));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("&Save\tCtl-S").c_str(),NULL,this,ID(IDM_SAVE));
	new FXMenuCommand(fileMenu,TranslatedMenuLabel("Save &As...\tCtl-Shift-S").c_str(),NULL,this,ID(IDM_SAVEAS));
	fileExportMenu = new FXMenuPane(mainWindow);
	new FXMenuCascade(fileMenu, TranslatedMenuLabel("&Export").c_str(), NULL, fileExportMenu);
	new FXMenuCommand(fileExportMenu, TranslatedMenuLabel("As &HTML...").c_str(),NULL,this,ID(IDM_SAVEASHTML));
	new FXMenuCommand(fileExportMenu, TranslatedMenuLabel("As &RTF...").c_str(),NULL,this,ID(IDM_SAVEASRTF));
	// new FXMenuCommand(fileExportMenu, TranslatedMenuLabel("As &PDF...").c_str(),NULL,this,ID(IDM_SAVEASPDF));
	new FXMenuSeparator(fileMenu);
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File0").c_str(), NULL, this, ID(fileStackCmdID + 0));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File1").c_str(), NULL, this, ID(fileStackCmdID + 1));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File2").c_str(), NULL, this, ID(fileStackCmdID + 2));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File3").c_str(), NULL, this, ID(fileStackCmdID + 3));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File4").c_str(), NULL, this, ID(fileStackCmdID + 4));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File5").c_str(), NULL, this, ID(fileStackCmdID + 5));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File6").c_str(), NULL, this, ID(fileStackCmdID + 6));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File7").c_str(), NULL, this, ID(fileStackCmdID + 7));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File8").c_str(), NULL, this, ID(fileStackCmdID + 8));
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("File9").c_str(), NULL, this, ID(fileStackCmdID + 9));
	new FXMenuSeparator(fileMenu);
	new FXMenuCommand(fileMenu, TranslatedMenuLabel("E&xit").c_str(),NULL,this,ID(IDM_QUIT));

	editMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Edit").c_str(),NULL,editMenu);
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Undo\tCtl-Z").c_str(), NULL, this, ID(IDM_UNDO));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Redo\tCtl-Y").c_str(), NULL, this, ID(IDM_REDO));
	new FXMenuSeparator(editMenu);
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Cu&t\tCtl-X").c_str(), NULL, this, ID(IDM_CUT));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Copy\tCtl-C").c_str(), NULL, this, ID(IDM_COPY));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Paste\tCtl-V").c_str(), NULL, this, ID(IDM_PASTE));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Delete\tDel").c_str(), NULL, this, ID(IDM_CLEAR));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Select &All\tCtl-A").c_str(), NULL, this, ID(IDM_SELECTALL));
	new FXMenuSeparator(editMenu);
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Match &Brace\tCtl-E").c_str(), NULL, this, ID(IDM_MATCHBRACE));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Select t&o Brace\tCtl-Shift-E").c_str(), NULL, this, ID(IDM_SELECTTOBRACE));
	mc = new FXMenuCommand(editMenu, TranslatedMenuLabel("S&how Calltip").c_str(), NULL, this, ID(IDM_SHOWCALLTIP));
	mc->setAccelText("Ctl-Shift-Space");
	table->addAccel(MKUINT(KEY_space,CONTROLMASK|SHIFTMASK),mc,MKUINT(FXWindow::ID_ACCEL,SEL_COMMAND));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Complete S&ymbol\tCtl-I").c_str(), NULL, this, ID(IDM_COMPLETE));
	mc = new FXMenuCommand(editMenu, TranslatedMenuLabel("Complete &Word\tCtl-Return").c_str(), NULL, this, ID(IDM_COMPLETEWORD));
	mc->setAccelText("Ctl-Return");
	new FXMenuCommand(editMenu, TranslatedMenuLabel("&Expand abbreviation\tCtl-B").c_str(), NULL, this, ID(IDM_ABBREV));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Co_mment or Uncomment\tCtl-Q").c_str(), NULL, this, ID(IDM_BLOCK_COMMENT));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Bo&x Comment\tCtl-Shift-B").c_str(), NULL, this, ID(IDM_BOX_COMMENT));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Stream Comme&nt\tCtl-Shift-Q").c_str(), NULL, this, ID(IDM_STREAM_COMMENT));
	table->addAccel(MKUINT(KEY_Return,CONTROLMASK),mc,MKUINT(FXWindow::ID_ACCEL,SEL_COMMAND));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Make &Selection Uppercase\tCtl-Shift-U").c_str(), NULL, this, ID(IDM_UPRCASE));
	new FXMenuCommand(editMenu, TranslatedMenuLabel("Make Selection &Lowercase\tCtl-U").c_str(), NULL, this, ID(IDM_LWRCASE));

	searchMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Search").c_str(),NULL,searchMenu);
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("&Find...\tCtl-F").c_str(), NULL, this, ID(IDM_FIND));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("Find &Next\tF3").c_str(), NULL, this, ID(IDM_FINDNEXT));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("Find Previou&s\tShift-F3").c_str(), NULL, this, ID(IDM_FINDNEXTBACK));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("F&ind in Files...\tCtl-Shift-F").c_str(), NULL, this, ID(IDM_FINDINFILES));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("R&eplace...\tCtl-H").c_str(), NULL, this, ID(IDM_REPLACE));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("&Go To...\tCtl-G").c_str(), NULL, this, ID(IDM_GOTO));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("Next Book&mark\tF2").c_str(), NULL, this, ID(IDM_BOOKMARK_NEXT));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("Pre&vious Bookmark\tshift-F2").c_str(), NULL, this, ID(IDM_BOOKMARK_PREV));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("Toggle Bookmar&k\tCtl-F2").c_str(), NULL, this, ID(IDM_BOOKMARK_TOGGLE));
	new FXMenuCommand(searchMenu, TranslatedMenuLabel("&Clear All Bookmarks").c_str(), NULL, this, ID(IDM_BOOKMARK_CLEARALL));
	new FXMenuSeparator(searchMenu);

	viewMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&View").c_str(),NULL,viewMenu);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("Toggle &current fold").c_str(), NULL, this, ID(IDM_EXPAND));
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("Toggle &all folds").c_str(), NULL, this, ID(IDM_TOGGLE_FOLDALL));
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("Full Scree&n\tF11").c_str(), NULL, this, ID(IDM_FULLSCREEN));
	new FXMenuSeparator(viewMenu);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Tool Bar").c_str(), NULL, this, ID(IDM_VIEWTOOLBAR), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Status Bar").c_str(), NULL, this, ID(IDM_VIEWSTATUSBAR), MENUSTATE_CHECKED);
	new FXMenuSeparator(viewMenu);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Whitespace\tCtl-Shift-A").c_str(), NULL, this, ID(IDM_VIEWSPACE), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&End of Line\tCtl-Shift-B").c_str(), NULL, this, ID(IDM_VIEWEOL), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Indentation Guides").c_str(), NULL, this, ID(IDM_VIEWGUIDES), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Line Numbers").c_str(), NULL, this, ID(IDM_LINENUMBERMARGIN), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Margin").c_str(), NULL, this, ID(IDM_SELMARGIN), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Fold Margin").c_str(), NULL, this, ID(IDM_FOLDMARGIN), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Output\tF8").c_str(), NULL, this, ID(IDM_TOGGLEOUTPUT), MENUSTATE_CHECKED);
	new FXMenuCommand(viewMenu, TranslatedMenuLabel("&Parameters").c_str(), NULL, this, ID(IDM_TOGGLEPARAMETERS), MENUSTATE_CHECKED);

	toolsMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Tools").c_str(),NULL,toolsMenu);
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Compile\tCtl-F7").c_str(), NULL, this, ID(IDM_COMPILE));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Build\tF7").c_str(), NULL, this, ID(IDM_BUILD));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Go\tF5").c_str(), NULL, this, ID(IDM_GO));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool0\tCtl-0").c_str(), NULL, this, ID(IDM_TOOLS + 0));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool1\tCtl-1").c_str(), NULL, this, ID(IDM_TOOLS + 1));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool2\tCtl-2").c_str(), NULL, this, ID(IDM_TOOLS + 2));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool3\tCtl-3").c_str(), NULL, this, ID(IDM_TOOLS + 3));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool4\tCtl-4").c_str(), NULL, this, ID(IDM_TOOLS + 4));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool5\tCtl-5").c_str(), NULL, this, ID(IDM_TOOLS + 5));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool6\tCtl-6").c_str(), NULL, this, ID(IDM_TOOLS + 6));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool7\tCtl-7").c_str(), NULL, this, ID(IDM_TOOLS + 7));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool8\tCtl-8").c_str(), NULL, this, ID(IDM_TOOLS + 8));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("Tool9\tCtl-9").c_str(), NULL, this, ID(IDM_TOOLS + 9));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Stop Executing\tCtl-.").c_str(), NULL, this, ID(IDM_STOPEXECUTE));
	new FXMenuSeparator(toolsMenu);
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Next Message\tF4").c_str(), NULL, this, ID(IDM_NEXTMSG));
	new FXMenuCommand(toolsMenu, TranslatedMenuLabel("&Previous Message\tShift-F4").c_str(), NULL, this, ID(IDM_PREVMSG));

	optionsMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Options").c_str(),NULL,optionsMenu);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Vertical &Split").c_str(), NULL, this, ID(IDM_SPLITVERTICAL), MENUSTATE_CHECKED);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("&Wrap").c_str(), NULL, this, ID(IDM_WRAP), MENUSTATE_CHECKED);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Wrap Out&put").c_str(), NULL, this, ID(IDM_WRAPOUTPUT), MENUSTATE_CHECKED);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Clear &Output\tShift-F5").c_str(), NULL, this, ID(IDM_CLEAROUTPUT));
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("&Switch Pane\tCtl-F6").c_str(), NULL, this, ID(IDM_SWITCHPANE));
	new FXMenuSeparator(optionsMenu);
	lineEndCharactersMenu = new FXMenuPane(mainWindow);
	new FXMenuCascade(optionsMenu, TranslatedMenuLabel("&Line End Characters").c_str(), NULL, lineEndCharactersMenu);
	new FXMenuCommand(lineEndCharactersMenu, TranslatedMenuLabel("CR &+ LF").c_str(), NULL, this, ID(IDM_EOL_CRLF), MENUSTATE_CHECKED);
	new FXMenuCommand(lineEndCharactersMenu, TranslatedMenuLabel("&CR").c_str(), NULL, this, ID(IDM_EOL_CR), MENUSTATE_CHECKED);
	new FXMenuCommand(lineEndCharactersMenu, TranslatedMenuLabel("&LF").c_str(), NULL, this, ID(IDM_EOL_LF), MENUSTATE_CHECKED);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("&Convert Lien End Characters").c_str(), NULL, this, ID(IDM_EOL_CONVERT));
	new FXMenuSeparator(optionsMenu);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Use &monospaced font\tCtl-F11").c_str(), NULL, this, ID(IDM_MONOFONT), MENUSTATE_CHECKED);
	new FXMenuSeparator(optionsMenu);
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Open Local &Options File").c_str(), NULL, this, ID(IDM_OPENLOCALPROPERTIES));
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Open &User Options File").c_str(), NULL, this, ID(IDM_OPENUSERPROPERTIES));
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Open &Global Options File").c_str(), NULL, this, ID(IDM_OPENGLOBALPROPERTIES));
	new FXMenuCommand(optionsMenu, TranslatedMenuLabel("Open A&bbreviations File").c_str(), NULL, this, ID(IDM_OPENABBREVPROPERTIES));
	new FXMenuSeparator(optionsMenu);
	editPropsMenu = new FXMenuPane(mainWindow);
	new FXMenuCascade(optionsMenu, TranslatedMenuLabel("Edit Properties").c_str(), NULL, editPropsMenu);
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props0").c_str(), NULL, this, ID(IDM_IMPORT + 0));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props1").c_str(), NULL, this, ID(IDM_IMPORT + 1));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props2").c_str(), NULL, this, ID(IDM_IMPORT + 2));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props3").c_str(), NULL, this, ID(IDM_IMPORT + 3));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props4").c_str(), NULL, this, ID(IDM_IMPORT + 4));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props5").c_str(), NULL, this, ID(IDM_IMPORT + 5));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props6").c_str(), NULL, this, ID(IDM_IMPORT + 6));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props7").c_str(), NULL, this, ID(IDM_IMPORT + 7));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props8").c_str(), NULL, this, ID(IDM_IMPORT + 8));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props9").c_str(), NULL, this, ID(IDM_IMPORT + 9));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props10").c_str(), NULL, this, ID(IDM_IMPORT + 10));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props11").c_str(), NULL, this, ID(IDM_IMPORT + 11));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props12").c_str(), NULL, this, ID(IDM_IMPORT + 12));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props13").c_str(), NULL, this, ID(IDM_IMPORT + 13));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props14").c_str(), NULL, this, ID(IDM_IMPORT + 14));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props15").c_str(), NULL, this, ID(IDM_IMPORT + 15));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props16").c_str(), NULL, this, ID(IDM_IMPORT + 16));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props17").c_str(), NULL, this, ID(IDM_IMPORT + 17));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props18").c_str(), NULL, this, ID(IDM_IMPORT + 18));
	new FXMenuCommand(editPropsMenu, TranslatedMenuLabel("Props19").c_str(), NULL, this, ID(IDM_IMPORT + 19));

	languageMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Language").c_str(), NULL, languageMenu);
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language00").c_str(), NULL, this, ID(IDM_LANGUAGE + 0));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language01").c_str(), NULL, this, ID(IDM_LANGUAGE + 1));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language02").c_str(), NULL, this, ID(IDM_LANGUAGE + 2));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language03").c_str(), NULL, this, ID(IDM_LANGUAGE + 3));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language04").c_str(), NULL, this, ID(IDM_LANGUAGE + 4));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language05").c_str(), NULL, this, ID(IDM_LANGUAGE + 5));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language06").c_str(), NULL, this, ID(IDM_LANGUAGE + 6));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language07").c_str(), NULL, this, ID(IDM_LANGUAGE + 7));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language08").c_str(), NULL, this, ID(IDM_LANGUAGE + 8));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language09").c_str(), NULL, this, ID(IDM_LANGUAGE + 9));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language10").c_str(), NULL, this, ID(IDM_LANGUAGE + 10));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language11").c_str(), NULL, this, ID(IDM_LANGUAGE + 11));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language12").c_str(), NULL, this, ID(IDM_LANGUAGE + 12));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language13").c_str(), NULL, this, ID(IDM_LANGUAGE + 13));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language14").c_str(), NULL, this, ID(IDM_LANGUAGE + 14));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language15").c_str(), NULL, this, ID(IDM_LANGUAGE + 15));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language16").c_str(), NULL, this, ID(IDM_LANGUAGE + 16));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language17").c_str(), NULL, this, ID(IDM_LANGUAGE + 17));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language18").c_str(), NULL, this, ID(IDM_LANGUAGE + 18));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language19").c_str(), NULL, this, ID(IDM_LANGUAGE + 19));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language20").c_str(), NULL, this, ID(IDM_LANGUAGE + 20));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language21").c_str(), NULL, this, ID(IDM_LANGUAGE + 21));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language22").c_str(), NULL, this, ID(IDM_LANGUAGE + 22));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language23").c_str(), NULL, this, ID(IDM_LANGUAGE + 23));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language24").c_str(), NULL, this, ID(IDM_LANGUAGE + 24));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language25").c_str(), NULL, this, ID(IDM_LANGUAGE + 25));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language26").c_str(), NULL, this, ID(IDM_LANGUAGE + 26));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language27").c_str(), NULL, this, ID(IDM_LANGUAGE + 27));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language28").c_str(), NULL, this, ID(IDM_LANGUAGE + 28));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language29").c_str(), NULL, this, ID(IDM_LANGUAGE + 29));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language30").c_str(), NULL, this, ID(IDM_LANGUAGE + 30));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language31").c_str(), NULL, this, ID(IDM_LANGUAGE + 31));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language32").c_str(), NULL, this, ID(IDM_LANGUAGE + 32));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language33").c_str(), NULL, this, ID(IDM_LANGUAGE + 33));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language34").c_str(), NULL, this, ID(IDM_LANGUAGE + 34));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language35").c_str(), NULL, this, ID(IDM_LANGUAGE + 35));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language36").c_str(), NULL, this, ID(IDM_LANGUAGE + 36));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language37").c_str(), NULL, this, ID(IDM_LANGUAGE + 37));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language38").c_str(), NULL, this, ID(IDM_LANGUAGE + 38));
	new FXMenuCommand(languageMenu, TranslatedMenuLabel("Language39").c_str(), NULL, this, ID(IDM_LANGUAGE + 39));

	if (props.GetInt("buffers") > 1) {
		buffersMenu = new FXMenuPane(mainWindow);
		new FXMenuTitle(menuBar, TranslatedMenuLabel("&Buffers").c_str(),NULL,buffersMenu);
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("&Previous\tShift-F6").c_str(), NULL, this, ID(IDM_PREVFILE));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("&Next\tShift-F6").c_str(), NULL, this, ID(IDM_NEXTFILE));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("&Close All\tShift-F6").c_str(), NULL, this, ID(IDM_CLOSEALL));
		new FXMenuSeparator(buffersMenu);
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer0\tAlt-1").c_str(), NULL, this, ID(bufferCmdID + 0));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer1\tAlt-2").c_str(), NULL, this, ID(bufferCmdID + 1));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer2\tAlt-3").c_str(), NULL, this, ID(bufferCmdID + 2));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer3\tAlt-4").c_str(), NULL, this, ID(bufferCmdID + 3));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer4\tAlt-5").c_str(), NULL, this, ID(bufferCmdID + 4));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer5\tAlt-6").c_str(), NULL, this, ID(bufferCmdID + 5));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer6\tAlt-7").c_str(), NULL, this, ID(bufferCmdID + 6));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer7\tAlt-8").c_str(), NULL, this, ID(bufferCmdID + 7));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer8\tAlt-9").c_str(), NULL, this, ID(bufferCmdID + 8));
		new FXMenuCommand(buffersMenu, TranslatedMenuLabel("Buffer9\tAlt-0").c_str(), NULL, this, ID(bufferCmdID + 9));
	}

	helpMenu = new FXMenuPane(mainWindow);
	new FXMenuTitle(menuBar, TranslatedMenuLabel("&Help").c_str(),NULL,helpMenu);
	new FXMenuCommand(helpMenu, TranslatedMenuLabel("&Help\tF1").c_str(), NULL, this, ID(IDM_HELP));
	new FXMenuCommand(helpMenu, TranslatedMenuLabel("&SciTE Help").c_str(), NULL, this, ID(IDM_HELP_SCITE));
	new FXMenuCommand(helpMenu, TranslatedMenuLabel("&About SciTE").c_str(), NULL, this, ID(IDM_ABOUT));
}

void SciTEFOX::CreateUI() {
	wSciTE = mainWindow = new FXMainWindow(this, appName, NULL, NULL, DECOR_ALL);
	mainWindow->setTarget(this);
	mainWindow->setSelector(ID_MAINWINDOW);

	CreateMenu();

	FXToolbarShell * tbshell = new FXToolbarShell(mainWindow, FRAME_RAISED|FRAME_THICK);
	toolBar = new FXToolbar(mainWindow, tbshell, FRAME_RAISED|FRAME_THICK|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
	new FXToolbarGrip(menuBar, menuBar, FXToolbar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);
	new FXToolbarGrip(toolBar, toolBar, FXToolbar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);
	
	statusBar = new FXStatusbar(mainWindow, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);

	fileSelectorWidth = props.GetInt("fileselector.width", fileSelectorWidth);
	fileSelectorHeight = props.GetInt("fileselector.height", fileSelectorHeight);

	FXVerticalFrame * vFrame =
		new FXVerticalFrame(
			mainWindow, LAYOUT_FILL_X|LAYOUT_FILL_Y,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	FXSplitter * splitter =
		new FXSplitter(
			vFrame,
			SPLITTER_HORIZONTAL|SPLITTER_REVERSED|LAYOUT_FILL_X|LAYOUT_FILL_Y);
 	// splitter->setBarSize(heightBar);

	FXComposite * topParent;
#ifdef CLIENT_3D_EFFECT
	topParent = new FXVerticalFrame(
		splitter, FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#else
	topParent = splitter;
#endif
	FXScintilla * so = new FXScintilla(topParent, this, ID_EDITOR, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	wEditor = so;
	so->setScintillaID(IDM_SRCWIN);
	fnEditor = reinterpret_cast<SciFnDirect>(Platform::SendScintilla(
	               wEditor.GetID(), SCI_GETDIRECTFUNCTION, 0, 0));
	ptrEditor = Platform::SendScintilla(wEditor.GetID(),
	                                    SCI_GETDIRECTPOINTER, 0, 0);
	SendEditor(SCI_USEPOPUP, 0);
	FXComposite * outputParent;
#ifdef CLIENT_3D_EFFECT
	outputParent = new FXVerticalFrame(
		splitter, FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#else
	outputParent = splitter;
#endif
	so = new FXScintilla(outputParent, this, ID_OUTPUT, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	wOutput = so;
	so->setScintillaID(IDM_RUNWIN);
	fnOutput = reinterpret_cast<SciFnDirect>(Platform::SendScintilla(
	               wOutput.GetID(), SCI_GETDIRECTFUNCTION, 0, 0));
	ptrOutput = Platform::SendScintilla(wOutput.GetID(),
	                                    SCI_GETDIRECTPOINTER, 0, 0);
	SendOutput(SCI_USEPOPUP, 0);
	AddToolBar();
	SetIcon();

	SetFocus(wOutput.GetID());

	// Make a tool tip
	new FXTooltip(this,0);

	const int useDefault = 0x10000000;
	int left = props.GetInt("position.left", useDefault);
	int top = props.GetInt("position.top", useDefault);
	int width = props.GetInt("position.width", useDefault);
	int height = props.GetInt("position.height", useDefault);
	if (width == -1 || height == -1) {
		width = getRoot()->getDefaultWidth() - left - 10;
		height = getRoot()->getDefaultHeight() - top - 30;
	}
	if ((width != useDefault) && (height != useDefault))
		mainWindow->resize(width, height);

// <FIXME> Je n'ai pas trouvé l'équivalent de drag_data_received en FOX
//	static const GtkTargetEntry dragtypes[] = { { "text/uri-list", 0, 0 } };
//	static const gint n_dragtypes = sizeof(dragtypes) / sizeof(dragtypes[0]);
//
//	gtk_drag_dest_set(wSciTE.GetID(), GTK_DEST_DEFAULT_ALL, dragtypes,
//	                  n_dragtypes, GDK_ACTION_COPY);
//	(void)gtk_signal_connect(GTK_OBJECT(wSciTE.GetID()), "drag_data_received",
//	                         GTK_SIGNAL_FUNC(DragDataReceived), this);
// </FIXME>

	UIAvailable();
	create();
}

void SciTEFOX::SetIcon() {
	mainWindow->setIcon(new FXXPMIcon(this, (const FXchar **)SciIcon_xpm));
}

bool SciTEFOX::CreatePipe(bool forceNew) {

	bool anotherPipe = false;
	bool tryStandardPipeCreation = false;
	SString pipeFilename = props.Get("ipc.scite.name");
	fdPipe = -1;

	//check we have been given a specific pipe name
	if (pipeFilename.size() > 0)
	{
		//printf("CreatePipe: if (pipeFilename.size() > 0): %s\n", pipeFilename.c_str());
		snprintf(pipeName, CHAR_MAX - 1, "%s", pipeFilename.c_str());

		fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);
		if (fdPipe == -1 && errno == EACCES) {
			//printf("CreatePipe: No access\n");
			tryStandardPipeCreation = true;
		}
		//there isn't one - so create one
		else if (fdPipe == -1) {
			SString fdPipeString;
			//printf("CreatePipe: Non found - making\n");
//WB++
#ifndef __vms
			mkfifo(pipeName, 0777);
#else           // no mkfifo on OpenVMS!
			creat(pipeName, 0777);
#endif
//WB--
			fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);
			
			fdPipeString = fdPipe;
			props.Set("ipc.scite.fdpipe",fdPipeString.c_str());
			tryStandardPipeCreation = false;
		}
		else
		{
			//printf("CreatePipe: Another one there - opening\n");

			fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);

			//there is already another pipe so set it to true for the return value
			anotherPipe = true;
			//I don;t think it is a good idea to be able to listen to our own pipes (yet) so just return
			//break;
			return anotherPipe;
		}
	}
	else
	{
		tryStandardPipeCreation = true;
	}
	
	if( tryStandardPipeCreation )
	{
	//possible bug here (eventually), can't have more than a 1000 SciTE's open - ajkc 20001112
	for (int i = 0; i < 1000; i++) {

		//create the pipe name - we use a number as well just incase multiple people have pipes open
		//or we are forceing a new instance of scite (even if there is already one)
		sprintf(pipeName, "/tmp/.SciTE.%d.ipc", i);

		//printf("Trying pipe %s\n", pipeName);
		//check to see if there is already one
		fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);

		//there is one but it isn't ours
		if (fdPipe == -1 && errno == EACCES) {
			//printf("No access\n");
			continue;
		}
		//there isn't one - so create one
		else if (fdPipe == -1) {
				SString fdPipeString;
			//printf("Non found - making\n");
//WB++
#ifndef __vms
			mkfifo(pipeName, 0755);
#else           // no mkfifo on OpenVMS!
			creat(pipeName, 0755);
#endif
//WB--
			fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);
				//store the file descriptor of the pipe so we can write to it again. (mainly for the director interface)
				fdPipeString = fdPipe;
				props.Set("ipc.scite.fdpipe",fdPipeString.c_str());
			break;
		}
		//there is so just open it (and we don't want out own)
		else if (forceNew == false) {
			//printf("Another one there - opening\n");

			fdPipe = open(pipeName, O_RDWR | O_NONBLOCK);

			//there is already another pipe so set it to true for the return value
			anotherPipe = true;
			//I don;t think it is a good idea to be able to listen to our own pipes (yet) so just return
			//break;
			return anotherPipe;
		}
		//we must want another one
	}
	}

	if (fdPipe != -1) {
		//store the inputwatcher so we can remove it.
		addInput(fdPipe, INPUT_READ, this, ID_PIPESIGNAL);
		//store the file descriptor of the pipe so we can write to it again.

		return anotherPipe;
	}

	//we must have failed or something so there definately isn't "another pipe"
	return false;
}

//use to send a command through a pipe.  (there is no checking to see whos's pipe it is. Probably not a good
//idea to send commands to  our selves.
bool SciTEFOX::SendPipeCommand(const char *pipeCommand) {
	//check that there is actually a pipe
	int size = 0;

	if (fdPipe != -1) {
		size = write(fdPipe, pipeCommand, strlen(pipeCommand) + 1);
		//printf("dd: Send pipecommand: %s %d bytes\n", pipeCommand, size);
		if (size != -1)
			return true;
	}
	return false;
}

//signal handler for gdk_input_add for the pipe listener.
long SciTEFOX::onPipeSignal(FXObject*, FXSelector, void*) {
	int readLength;
	char pipeData[8192];
	PropSetFile pipeProps;

	//printf("Pipe read signal\n");

	//use a propset so we don't have to fuss (it's already done!)
	while ((readLength = read(fdPipe, pipeData, sizeof(pipeData))) > 0) {
		//printf("Read: >%s< from pipedata\n", pipeData);
		//fill the propset with the data from the pipe
		pipeProps.ReadFromMemory(pipeData, readLength, 0);

		//get filename from open command
		SString fileName = pipeProps.Get("open");

		//printf("filename from pipe: %s\n", fileName.c_str());

		//is filename zero length if propset.get fails?
		//if there is a file name, open it.
		if (fileName.size() > 0) {
			//printf("opening >%s< from pipecommand\n", fileName.c_str());
			Open(fileName.c_str());

			//grab the focus back to us.  (may not work) - any ideas?
			mainWindow->setFocus();
		}
	}
	return 1;
}

void SciTEFOX::CheckAlreadyOpen(const char *cmdLine) {
	if (!props.GetInt("check.if.already.open"))
		return;

	// Create a pipe and see if it finds another one already there

	//printf("CheckAlreadyOpen: cmdLine: %s\n", cmdLine);
	// If we are not given a command line filename, assume that we just want to load ourselves.
	if ((strlen(cmdLine) > 0) && (CreatePipe() == true)) {
		char currentPath[MAX_PATH];
		getcwd(currentPath, MAX_PATH);

		//create the command to send thru the pipe
		char pipeCommand[CHAR_MAX];

		//check to see if path is already absolute
		if (cmdLine[0] == '/')
			sprintf(pipeCommand, "open:%s", cmdLine);
		//if it isn't then add the absolute path to the from of the command to send.
		else
			sprintf(pipeCommand, "open:%s/%s", currentPath, cmdLine);

		//printf("Sending %s through pipe\n", pipeCommand);
		//send it
		bool piperet = SendPipeCommand(pipeCommand);
		//printf("Sent.\n");

		//if it was OK then quit (we should really get an anwser back just in case
		if (piperet == true) {
			//printf("Sent OK -> Quitting\n");
			exit(0);
		}
	}
	//create our own pipe.
	else if (fdPipe == -1) {
		CreatePipe(true);
	}
}

int SciTEFOX::Run(int argc, char ** argv) {
	// Collect the argv into one string with each argument separated by '\n'
	SString args;
	int arg;
	for (arg=1; arg < argc; arg++) {
		args.appendwithseparator(argv[arg], '\n');
	}

	// Process any initial switches
	ProcessCommandLine(args, 0);

	if( props.Get("ipc.director.name").size() == 0 ){
	// If a file name argument, check if already open in another SciTE
	for (arg=1; arg < argc; arg++) {
		if (argv[arg][0] != '-') {
			CheckAlreadyOpen(argv[arg]);
		}
	}
	}

	CreateUI();

	// Process remaining switches and files
	ProcessCommandLine(args, 1);

	CheckMenus();
	SizeSubWindows();
	SetFocus(wEditor.GetID());

	return run();
}

void SciTEFOX::create()
{
  FXApp::create();
  mainWindow->show();
}

int main(int argc, char *argv[]) {
#ifdef LUA_SCRIPTING
	LuaExtension luaExtender;
	Extension *extender = &luaExtender;
#else
#ifndef NO_FILER
	DirectorExtension director;
	Extension *extender = &director;
#else
	Extension *extender = 0;
#endif
#endif

#ifdef __vms
	// Store the path part of the module name
	strcpy(g_modulePath, argv[0]);
	char *p = strstr(g_modulePath, "][");
	if (p != NULL) {
		strcpy (p, p + 2);
	}
	p = strchr(g_modulePath, ']');
	if (p == NULL) {
		p = strchr(g_modulePath, '>');
	}
	if (p == NULL) {
		p = strchr(g_modulePath, ':');
	}
	if (p != NULL) {
		*(p + 1) = '\0';
	}
	strcpy(g_modulePath, VMSToUnixStyle(g_modulePath));
	g_modulePath[strlen(g_modulePath) - 1] = '\0';  // remove trailing "/"
#endif
	SciTEFOX scite(extender);
	
	scite.init(argc, argv);
	
	// Make application
	scite.Run(argc, argv);
}

/** FXScintilla source code edit control
 *
 *  ScintillaFOX.cxx - FOX toolkit specific subclass of ScintillaBase
 *
 *  Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
 *
 *  Adapted from the Scintilla source ScintillaGTK.cxx 
 *  Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
 *
 *  ====================================================================
 *
 *  This file is part of FXScintilla.
 * 
 *  FXScintilla is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  FXScintilla is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with FXScintilla; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#if !defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
# if defined(__CYGWIN__) || defined(__MINGW32__)
#  include <windows.h>
#  ifdef PIC
#   define FOXDLL
#  endif
# endif
# include <fox/fx.h>
# include <fox/fxkeys.h>
#else
# include <fx.h>
# include <fxkeys.h>
#endif	// !defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

#include "Platform.h"

#include "XPM.h"

#include "Scintilla.h"
#include "ScintillaWidget.h"
#ifdef SCI_LEXER
#include "SciLexer.h"
#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#endif
#include "ContractionState.h"
#include "SVector.h"
#include "CellBuffer.h"
#include "CallTip.h"
#include "KeyMap.h"
#include "Indicator.h"
#include "LineMarker.h"
#include "Style.h"
#include "AutoComplete.h"
#include "ViewStyle.h"
#include "Document.h"
#include "Editor.h"
#include "SString.h"
#include "ScintillaBase.h"

#ifdef WIN32
#ifdef FOXDLL
#undef FXAPI
#define FXAPI __declspec(dllexport)
#endif
#endif
#include "FXScintilla.h"

#ifdef _MSC_VER
// Constant conditional expressions are because of GTK+ headers
#pragma warning(disable: 4127)
#endif

#if HAVE_FOX_1_1
#define SELID FXSELID
#define SELTYPE FXSELTYPE
#endif

// ====================================================================
// ScintillaFOX
// ====================================================================
class ScintillaFOX : public ScintillaBase {
	friend class FXScintilla;
protected:
	FXScintilla & _fxsc;
	bool tryDrag;
	bool dragWasDropped;
	Point ptMouseLastBeforeDND;

	// Because clipboard access is asynchronous, copyText is created by Copy
	SelectionText copyText;
	SelectionText primary;
	
	// Vertical scrollbar handling
	int vsbMax;
	int vsbPage;
public:
	ScintillaFOX(FXScintilla & fxsc);
	virtual ~ScintillaFOX();
	virtual sptr_t WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
private:
	// Virtual methods from ScintillaBase
	virtual void Initialise();
	virtual void Finalise();
	virtual void SetVerticalScrollPos();
	virtual void SetHorizontalScrollPos();
	virtual void Copy();
	virtual void Paste();
	virtual void ClaimSelection();
	virtual void NotifyChange();
	virtual void NotifyParent(SCNotification scn);
	virtual void SetTicking(bool on);
	virtual void SetMouseCapture(bool on);
	virtual bool HaveMouseCapture();
	virtual sptr_t DefWndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam);
	virtual void CreateCallTipWindow(PRectangle rc);
	virtual void AddToPopUp(const char * label, int cmd = 0, bool enabled = true);
	virtual void StartDrag();
	//
	static sptr_t DirectFunction(ScintillaFOX *sciThis, 
		unsigned int iMessage, uptr_t wParam, sptr_t lParam);
	// Paint
	void SyncPaint(PRectangle rc);
	void FullPaint();
	void UnclaimSelection();
	void ReceivedSelection(FXDNDOrigin origin);
	void NotifyKey(int key, int modifiers);
	void NotifyURIDropped(const char *list);
	
	// From ScintillaBase
	virtual bool ModifyScrollBars(int nMax, int nPage);
	virtual void ReconfigureScrollBars();
	virtual PRectangle GetClientRectangle();
	virtual int KeyDefault(int key, int modifiers);

	// Popup flag
	bool getDisplayPopupMenu() const { return displayPopupMenu; }
};

ScintillaFOX::ScintillaFOX(FXScintilla & fxsc) :
	_fxsc(fxsc),
	vsbMax(0),
	vsbPage(0),
	tryDrag(false),
	dragWasDropped(false)
{
	wMain = &_fxsc;
	Initialise();
}

ScintillaFOX::~ScintillaFOX()
{
	Finalise();
}

sptr_t ScintillaFOX::WndProc(unsigned int iMessage, uptr_t wParam, sptr_t lParam) {
	switch (iMessage) {

	case SCI_GRABFOCUS:
		_fxsc.setFocus();
		break;

	case SCI_GETDIRECTFUNCTION:
		return reinterpret_cast<sptr_t>(DirectFunction);
	
	case SCI_GETDIRECTPOINTER:
		return reinterpret_cast<sptr_t>(this);

	default:
		return ScintillaBase::WndProc(iMessage,wParam,lParam);
	}
	return 0l;
}

void ScintillaFOX::ClaimSelection()
{
  // Acquire selection
	if (currentPos != anchor) {
    _fxsc.acquireSelection(&FXWindow::stringType,1);
		primarySelection = true;
		primary.Set(0, 0);
	}
	else {
    _fxsc.releaseSelection();
	}
}

void ScintillaFOX::UnclaimSelection()
{
	//Platform::DebugPrintf("UnclaimPrimarySelection\n");
	if (!_fxsc.hasSelection()) {
		primary.Set(0, 0);
		primarySelection = false;
		FullPaint();
	}
}

void ScintillaFOX::ReceivedSelection(FXDNDOrigin origin)
{
  FXuchar *data; FXuint len;
	if (!pdoc->IsReadOnly()) {
    if(_fxsc.getDNDData(origin, FXWindow::stringType, data, len)) {
      FXRESIZE(&data,FXuchar,len+1); data[len]='\0';
			pdoc->BeginUndoAction();
			// Check for "\n\0" ending to string indicating that selection is rectangular
			int selStart = SelectionStart();
	    if(_fxsc.hasSelection() && (origin == FROM_CLIPBOARD)) {
				ClearSelection();
			}
// <FIXME/>
/*			bool isRectangular = ((len > 0) &&
				(data[len] == 0 && data[len - 1] == '\n'));
			if (isRectangular) {
				cout << data << flush;
				PasteRectangular(selStart, (const char *)data, len);
			} else */{
#ifdef WIN32
				len--;
#endif	// WIN32
				pdoc->InsertString(currentPos, (const char *)data, len);
				SetEmptySelection(currentPos + len);
			}
			pdoc->EndUndoAction();
      FXFREE(&data);
		}
	}
}

void ScintillaFOX::NotifyKey(int key, int modifiers) {
	SCNotification scn;
	scn.nmhdr.code = SCN_KEY;
	scn.ch = key;
	scn.modifiers = modifiers;

	NotifyParent(scn);
}

void ScintillaFOX::NotifyURIDropped(const char *list) {
	SCNotification scn;
	scn.nmhdr.code = SCN_URIDROPPED;
	scn.text = list;

	NotifyParent(scn);
}

int ScintillaFOX::KeyDefault(int key, int modifiers) {
	if (!(modifiers & SCI_CTRL) && !(modifiers & SCI_ALT) && (key < 256)) {
		AddChar(key);
		return 1;
	} else {
		// Pass up to container in case it is an accelerator
		NotifyKey(key, modifiers);
		return 0;
	}
	//Platform::DebugPrintf("SK-key: %d %x %x\n",key, modifiers);
}

// ********************************************************************

void ScintillaFOX::Initialise()
{
	SetTicking(true);
}

void ScintillaFOX::Finalise()
{
	SetTicking(false);
	ScintillaBase::Finalise();
}

void ScintillaFOX::SetVerticalScrollPos()
{
	// Update the internals about vertical pos. Should not use
	// _fxsc.setPosition(x, y) because it would trigger moveContents
	_fxsc.verticalScrollbar()->setPosition(topLine * vs.lineHeight);
	_fxsc.pos_y = -_fxsc.verticalScrollbar()->getPosition();
	_fxsc.update();
}

void ScintillaFOX::SetHorizontalScrollPos()
{
	// Update the internals about horizontal pos. Should not use
	// _fxsc.setPosition(x, y) because it would trigger moveContents
	_fxsc.horizontalScrollbar()->setPosition(xOffset);
	_fxsc.pos_x = -_fxsc.horizontalScrollbar()->getPosition();
	_fxsc.update();
}

void ScintillaFOX::Copy()
{
  if (_fxsc.hasSelection()) {
    if (_fxsc.acquireClipboard(&FXWindow::stringType, 1)) {
			CopySelectionRange(&copyText);
    }
	}
	else {
    _fxsc.releaseClipboard();
	}
}

void ScintillaFOX::Paste()
{
	ReceivedSelection(FROM_CLIPBOARD);
}

void ScintillaFOX::NotifyChange()
{
	_fxsc.handle(&_fxsc, MKUINT(0, SEL_CHANGED), NULL);
}

void ScintillaFOX::NotifyParent(SCNotification scn)
{
	scn.nmhdr.hwndFrom = wMain.GetID();
	scn.nmhdr.idFrom = GetCtrlID();
	_fxsc.handle(&_fxsc, MKUINT(0, SEL_COMMAND), &scn);
}

void ScintillaFOX::SetTicking(bool on)
{
	if (timer.ticking != on) {
		timer.ticking = on;
		if (timer.ticking) {
			timer.tickerID = FXApp::instance()->addTimeout(timer.tickSize, &_fxsc, _fxsc.ID_TICK);
		} else {
			FXApp::instance()->removeTimeout(static_cast<FXTimer *>(timer.tickerID));
		}
	}
	timer.ticksToWait = caret.period;
}

void ScintillaFOX::SetMouseCapture(bool on)
{
	if (mouseDownCaptures) {
		if (on) {
			_fxsc.grab();
		} else {
			_fxsc.ungrab();
		}
	}
}

bool ScintillaFOX::HaveMouseCapture()
{
	return _fxsc.grabbed();
}

void ScintillaFOX::StartDrag()
{
	tryDrag = true;
	dragWasDropped = false;
}

sptr_t ScintillaFOX::DefWndProc(unsigned int, uptr_t, sptr_t)
{
	return 0;
}

void ScintillaFOX::CreateCallTipWindow(PRectangle rc)
{
}

void ScintillaFOX::AddToPopUp(const char * label, int cmd, bool enabled)
{
	if (label[0]) {
		FXMenuCommand * item = new FXMenuCommand(popup.GetID(), label, NULL, &_fxsc, SCID(cmd));
		if (!enabled)
			item->disable();
	}
	else
		new FXMenuSeparator(popup.GetID());
}

sptr_t ScintillaFOX::DirectFunction(
    ScintillaFOX *sciThis, unsigned int iMessage, uptr_t wParam, sptr_t lParam) {
	return sciThis->WndProc(iMessage, wParam, lParam);
}

PRectangle ScintillaFOX::GetClientRectangle() {
	// Have to call FXScrollArea::getViewportXxxx instead of getViewportXxxx
	// to prevent infinite loop
	PRectangle rc(0, 0, _fxsc.FXScrollArea::getViewportWidth(), _fxsc.FXScrollArea::getViewportHeight());
	if (_fxsc.horizontalScrollbar()->shown())
		rc.bottom -= _fxsc.horizontalScrollbar()->getDefaultHeight();
	if (_fxsc.verticalScrollbar()->shown())
		rc.right -= _fxsc.verticalScrollbar()->getDefaultWidth();
	return rc;
}

void ScintillaFOX::ReconfigureScrollBars()
{
	if (horizontalScrollBarVisible)
		_fxsc.setScrollStyle(_fxsc.getScrollStyle() & ~HSCROLLER_NEVER);
	else
		_fxsc.setScrollStyle(_fxsc.getScrollStyle() | HSCROLLER_NEVER);
	_fxsc.recalc();
}

bool ScintillaFOX::ModifyScrollBars(int nMax, int nPage)
{
	bool modified = false;
	// There was no vertical scrollbar if
	//   !vsbMax || (vsbMax < vsbPage)
	bool noVScroll = !vsbMax || (vsbMax < vsbPage);
	// There won't be a vertical scrollbar if
	//   !nMax || (nMax < nPage)
	bool noVScrollNew = !nMax || (nMax < nPage);

	if ((nMax != vsbMax) || (nPage != vsbPage)) {
		vsbMax = nMax;
		vsbPage = nPage;
		// Layout if vertical scrollbar should appear or change
		if (noVScroll && !noVScrollNew || !noVScroll) {
			_fxsc.layout();
			modified = true;
		}
	}
	// Vertical scrollbar
	int line = vs.lineHeight;
	if (_fxsc.verticalScrollbar()->getLine() != line) {
		_fxsc.verticalScrollbar()->setLine(line);
		modified = true;
	}
	// Horizontal scrollbar
	PRectangle rcText = GetTextRectangle();
	unsigned int pageWidth = rcText.Width();
	if ((_fxsc.horizontalScrollbar()->getPage() != pageWidth) ||
			(_fxsc.horizontalScrollbar()->getLine() != 10)) {
		_fxsc.horizontalScrollbar()->setPage(pageWidth);
		_fxsc.horizontalScrollbar()->setLine(10);
		modified = true;
	}
	return modified;
}

// --------------------------------------------------------------------
// Synchronously paint a rectangle of the window.
void ScintillaFOX::SyncPaint(PRectangle rc) {
	paintState = painting;
	rcPaint = rc;
	PRectangle rcClient = GetClientRectangle();
	paintingAllText = rcPaint.Contains(rcClient);
	//Platform::DebugPrintf("ScintillaGTK::SyncPaint %0d,%0d %0d,%0d\n",
	//	rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
	Surface *sw = Surface::Allocate();
	if (sw) {
		sw->Init(wMain.GetID());
		Paint(sw, rcPaint);
		sw->Release();
		delete sw;
	}
	if (paintState == paintAbandoned) {
		// Painting area was insufficient to cover new styling or brace highlight positions
		FullPaint();
	}
	paintState = notPainting;
}

// Redraw all of text area. This paint will not be abandoned.
void ScintillaFOX::FullPaint() {
	paintState = painting;
	rcPaint = GetClientRectangle();
	//Platform::DebugPrintf("ScintillaGTK::FullPaint %0d,%0d %0d,%0d\n",
	//	rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
	paintingAllText = true;
	if (wMain.GetID()) {
		Surface *sw = Surface::Allocate();
		if (sw) {
			sw->Init(wMain.GetID());
			Paint(sw, rcPaint);
			sw->Release();
			delete sw;
		}
	}
	paintState = notPainting;
}

// ====================================================================
long Platform::SendScintilla(
		WindowID w, unsigned int msg, unsigned long wParam, long lParam) {
	return static_cast<FXScintilla *>(w)->sendMessage(msg, wParam, lParam);
}

long Platform::SendScintillaPointer(WindowID w, unsigned int msg,
																		unsigned long wParam, void *lParam) {
	return static_cast<FXScintilla *>(w)->
		sendMessage(msg, wParam, reinterpret_cast<sptr_t>(lParam));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FXScintilla
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FXDEFMAP(FXScintilla) FXScintillaMap[]={
  FXMAPFUNCS(SEL_COMMAND, FXScrollArea::ID_LAST, FXScintilla::idLast, FXScintilla::onScintillaCommand),
  FXMAPFUNC(SEL_COMMAND, 0, FXScintilla::onCommand),
  FXMAPFUNC(SEL_CHANGED, 0, FXScintilla::onChanged),
  FXMAPFUNC(SEL_PAINT, 0, FXScintilla::onPaint),
  FXMAPFUNC(SEL_CONFIGURE,0,FXScintilla::onConfigure),
  FXMAPFUNC(SEL_TIMEOUT,FXScintilla::ID_TICK,FXScintilla::onTimeoutTicking),
  FXMAPFUNC(SEL_FOCUSIN,0,FXScintilla::onFocusIn),
  FXMAPFUNC(SEL_FOCUSOUT,0,FXScintilla::onFocusOut),
  FXMAPFUNC(SEL_MOTION,0,FXScintilla::onMotion),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,FXScintilla::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,FXScintilla::onLeftBtnRelease),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,0,FXScintilla::onRightBtnPress),
  FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,0,FXScintilla::onMiddleBtnPress),
  FXMAPFUNC(SEL_BEGINDRAG,0,FXScintilla::onBeginDrag),
  FXMAPFUNC(SEL_DRAGGED,0,FXScintilla::onDragged),
  FXMAPFUNC(SEL_ENDDRAG,0,FXScintilla::onEndDrag),
  FXMAPFUNC(SEL_DND_ENTER,0,FXScintilla::onDNDEnter),
  FXMAPFUNC(SEL_DND_LEAVE,0,FXScintilla::onDNDLeave),
  FXMAPFUNC(SEL_DND_DROP,0,FXScintilla::onDNDDrop),
  FXMAPFUNC(SEL_DND_MOTION,0,FXScintilla::onDNDMotion),
  FXMAPFUNC(SEL_DND_REQUEST,0,FXScintilla::onDNDRequest),
  FXMAPFUNC(SEL_SELECTION_LOST,0,FXScintilla::onSelectionLost),
//  FXMAPFUNC(SEL_SELECTION_GAINED,0,FXScintilla::onSelectionGained),
  FXMAPFUNC(SEL_SELECTION_REQUEST,0,FXScintilla::onSelectionRequest),
  FXMAPFUNC(SEL_CLIPBOARD_LOST,0,FXScintilla::onClipboardLost),
//  FXMAPFUNC(SEL_CLIPBOARD_GAINED,0,FXScintilla::onClipboardGained),
  FXMAPFUNC(SEL_CLIPBOARD_REQUEST,0,FXScintilla::onClipboardRequest),
  FXMAPFUNC(SEL_KEYPRESS,0,FXScintilla::onKeyPress),
};

FXIMPLEMENT(FXScintilla,FXScrollArea,FXScintillaMap,ARRAYNUMBER(FXScintillaMap))

FXScintilla::FXScintilla()
{
}

FXScintilla::FXScintilla(FXComposite * p, FXObject * tgt, FXSelector sel,
		FXuint opts, FXint x, FXint y, FXint w, FXint h) :
	FXScrollArea(p, opts, x, y, w, h)
{
  flags|=FLAG_ENABLED;
	target = tgt;
	message = sel;
	_scint = new ScintillaFOX(*this);
}

FXScintilla::~FXScintilla()
{
	delete _scint;
}

void FXScintilla::create()
{
  FXScrollArea::create();
  if(!textType){textType=getApp()->registerDragType(textTypeName);}
  if(!urilistType){urilistType=getApp()->registerDragType(urilistTypeName);}
	dropEnable();
}

FXbool FXScintilla::canFocus() const
{
	return true;
}

long FXScintilla::onScintillaCommand(FXObject * obj, FXSelector sel, void * ptr)
{
	_scint->Command(SELID(sel)-SCID(0));
	return 1;
}

long FXScintilla::onCommand(FXObject * obj, FXSelector sel, void * ptr)
{
	if (target)
		return target->handle(this, MKUINT(message, SELTYPE(sel)), ptr);
	return 0;
}

long FXScintilla::onChanged(FXObject * obj, FXSelector sel, void * ptr)
{
	if (target)
		return target->handle(this, MKUINT(message, SELTYPE(sel)), ptr);
	return 0;
}

long FXScintilla::onPaint(FXObject * obj, FXSelector sel, void * ptr)
{
	FXEvent   *ev=(FXEvent*)ptr;
	PRectangle rcPaint(ev->rect.x, ev->rect.y, ev->rect.x + ev->rect.w - 1, ev->rect.y + ev->rect.h - 1);
	_scint->SyncPaint(rcPaint);
	return 1;
}

long FXScintilla::onTimeoutTicking(FXObject *, FXSelector, void *)
{
	_scint->timer.tickerID = FXApp::instance()->addTimeout(_scint->timer.tickSize, this, ID_TICK);
	_scint->Tick();
	return 1;
}

long FXScintilla::onFocusIn(FXObject * sender, FXSelector sel, void * ptr)
{
	FXScrollArea::onFocusIn(sender, sel, ptr);
	_scint->SetFocusState(true);
	return 1;
}

long FXScintilla::onFocusOut(FXObject * sender, FXSelector sel, void * ptr)
{
	FXScrollArea::onFocusOut(sender, sel, ptr);
	_scint->SetFocusState(false);
	return 1;
}

long FXScintilla::onMotion(FXObject * sender, FXSelector sel, void * ptr)
{
	FXEvent * ev = static_cast<FXEvent *>(ptr);
	if (_scint->tryDrag) {
		_scint->tryDrag = false;
		handle(this, MKUINT(0, SEL_BEGINDRAG), 0);
	}
	if (isDragging()) {
		return handle(this, MKUINT(0, SEL_DRAGGED), ptr);
	}
	else {
		Point pt(ev->win_x, ev->win_y);
		_scint->ButtonMove(pt);
	}
	return 1;
}

long FXScintilla::onLeftBtnPress(FXObject * sender, FXSelector sel, void * ptr)
{
//	if (FXScrollArea::onLeftBtnPress(sender, sel, ptr))
//		return 1;
  handle(this,MKUINT(0,SEL_FOCUS_SELF),ptr);
	setFocus();
	FXEvent * ev = static_cast<FXEvent *>(ptr);
	Point pt;
	pt.x = ev->win_x;
	pt.y = ev->win_y;
		//sciThis->ButtonDown(pt, event->time,
		//	event->state & GDK_SHIFT_MASK,
		//	event->state & GDK_CONTROL_MASK,
		//	event->state & GDK_MOD1_MASK);
		// Instead of sending literal modifiers use control instead of alt
		// This is because all the window managers seem to grab alt + click for moving
	_scint->ButtonDown(pt, ev->time,
			ev->state & SHIFTMASK,
			ev->state & CONTROLMASK,
			ev->state & CONTROLMASK);
	return 1;
}

long FXScintilla::onLeftBtnRelease(FXObject *, FXSelector, void * ptr)
{
	if (isDragging())
		return handle(this, MKUINT(0, SEL_ENDDRAG), ptr);
	FXEvent * ev = static_cast<FXEvent *>(ptr);
	Point pt(ev->win_x, ev->win_y);
	if (!_scint->HaveMouseCapture()) {
		if (_scint->tryDrag) {
			_scint->tryDrag = false;
			_scint->SetEmptySelection(_scint->PositionFromLocation(pt));
			_scint->SetDragPosition(invalidPosition);
		}
		return 1;
	}
	_scint->ButtonUp(pt, ev->time, (ev->state & CONTROLMASK) != 0);
	return 1;
}

long FXScintilla::onRightBtnPress(FXObject * sender, FXSelector sel, void * ptr)
{
//	if (FXScrollArea::onRightBtnPress(sender, sel, ptr))
//		return 1;
	if (target && target->handle(this, MKUINT(message, SELTYPE(sel)), ptr))
		return 1;
	if (!_scint->getDisplayPopupMenu())
		return 0;
	FXEvent * ev = static_cast<FXEvent *>(ptr);
	_scint->ContextMenu(Point(ev->root_x, ev->root_y));
	return 1;
}

long FXScintilla::onMiddleBtnPress(FXObject * sender, FXSelector sel, void * ptr)
{
//	if (FXScrollArea::onMiddleBtnPress(sender, sel, ptr))
//		return 1;

	Point pt;
	pt.x = ((FXEvent *)ptr)->win_x;
	pt.y = ((FXEvent *)ptr)->win_y;
	_scint->currentPos = _scint->PositionFromLocation(pt);
	_scint->ReceivedSelection(FROM_SELECTION);
  return 1;
}

// ********************************************************************
// Keyboard
// ********************************************************************

// Map the keypad keys to their equivalent functions
static int KeyTranslate(int keyIn) {
	switch (keyIn) {
		case KEY_ISO_Left_Tab:
			return SCK_TAB;
		case KEY_KP_Down:
			return SCK_DOWN;
		case KEY_KP_Up:
			return SCK_UP;
		case KEY_KP_Left:
			return SCK_LEFT;
		case KEY_KP_Right:
			return SCK_RIGHT;
		case KEY_KP_Home:
			return SCK_HOME;
		case KEY_KP_End:
			return SCK_END;
		case KEY_KP_Page_Up:
			return SCK_PRIOR;
		case KEY_KP_Page_Down:
			return SCK_NEXT;
		case KEY_KP_Delete:
			return SCK_DELETE;
		case KEY_KP_Insert:
			return SCK_INSERT;
		case KEY_KP_Enter:
			return SCK_RETURN;
			
		case KEY_Down:
			return SCK_DOWN;
		case KEY_Up:
			return SCK_UP;
		case KEY_Left:
			return SCK_LEFT;
		case KEY_Right:
			return SCK_RIGHT;
		case KEY_Home:
			return SCK_HOME;
		case KEY_End:
			return SCK_END;
		case KEY_Page_Up:
			return SCK_PRIOR;
		case KEY_Page_Down:
			return SCK_NEXT;
		case KEY_Delete:
			return SCK_DELETE;
		case KEY_Insert:
			return SCK_INSERT;
		case KEY_Escape:
			return SCK_ESCAPE;
		case KEY_BackSpace:
			return SCK_BACK;
		case KEY_Tab:
			return SCK_TAB;
		case KEY_Return:
			return SCK_RETURN;
		case KEY_KP_Add:
			return SCK_ADD;
		case KEY_KP_Subtract:
			return SCK_SUBTRACT;
		case KEY_KP_Divide:
			return SCK_DIVIDE;
		default:
			return keyIn;
	}
}

long FXScintilla::onKeyPress(FXObject* sender,FXSelector sel,void* ptr)
{
  // Try handling it in base class first
  if (FXScrollArea::onKeyPress(sender,sel,ptr)) return 1;

  FXEvent *event=(FXEvent*)ptr;
	bool shift = (event->state & SHIFTMASK) != 0;
	bool ctrl = (event->state & CONTROLMASK) != 0;
	bool alt = (event->state & ALTMASK) != 0;
	// <FIXME> Workaround for event->code doesn't hold the correct
  // KEY_xxx under WIN32
#ifndef WIN32
	int key = event->code;
#else
	int key = ((event->text[0] >= 32) && !ctrl && !alt) ?
		event->text[0] : event->code;
#endif
	// </FIXME>
	if (ctrl && (key < 128))
		key = toupper(key);
	else if (!ctrl && (key >= KEY_KP_Multiply && key <= KEY_KP_9))
		key &= 0x7F;
	// Hack for keys over 256 and below command keys but makes Hungarian work.
	// This will have to change for Unicode
	else if ((key >= 0x100) && (key < 0x1000))
		key &= 0xff;
	else	
		key = KeyTranslate(key);

	bool consumed = false;
	bool added = _scint->KeyDown(key, shift, ctrl, alt, &consumed) != 0;
	if (!consumed)
		consumed = added;
	//Platform::DebugPrintf("SK-key: %d %x %x\n",event->code, event->state, consumed);
	if (event->code == 0xffffff && event->text.length() > 0) {
		_scint->ClearSelection();
		if (_scint->pdoc->InsertString(_scint->CurrentPosition(), event->text.text())) {
			_scint->MovePositionTo(_scint->CurrentPosition() + event->text.length());
		}
		consumed = true;
	}
	return consumed ? 1 : 0;
}

// ********************************************************************
// Clipboard
// ********************************************************************

// We lost the clipboard somehow
long FXScintilla::onClipboardLost(FXObject* sender,FXSelector sel,void* ptr){
  FXScrollArea::onClipboardLost(sender,sel,ptr);
	//Platform::DebugPrintf("Clipboard lost\n");
	_scint->copyText.Set(0, 0);

  return 1;
}

// Somebody wants our clipboard
long FXScintilla::onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *data;

  // Try handling it in base class first
  if(FXScrollArea::onClipboardRequest(sender,sel,ptr)) return 1;

  // Requested data from clipboard
  if(event->target==stringType){
		// <FIXME>
		// Framework taken from FXTextField.cpp
		// Should have a look to FXText.cpp too
    size_t len=strlen(_scint->copyText.s);
    FXCALLOC(&data,FXuchar,len+1);
    memcpy(data,_scint->copyText.s,len);
#ifndef WIN32
    setDNDData(FROM_CLIPBOARD,stringType,data,len);
#else
    setDNDData(FROM_CLIPBOARD,stringType,data,len+1);
#endif
		// </FIXME>
    return 1;
	}

  return 0;
}

// ********************************************************************
// Drag'n drop
// ********************************************************************

// Start a drag operation
long FXScintilla::onBeginDrag(FXObject* sender,FXSelector sel,void* ptr){
  _scint->SetMouseCapture(true);
  if(FXScrollArea::onBeginDrag(sender,sel,ptr)) return 1;
  beginDrag(&textType,1);
  setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));
  return 1;
  }


// End drag operation
long FXScintilla::onEndDrag(FXObject* sender,FXSelector sel,void* ptr){
  _scint->SetMouseCapture(false);
  if(FXScrollArea::onEndDrag(sender,sel,ptr)) return 1;
  endDrag((didAccept()!=DRAG_REJECT));
  setDragCursor(getApp()->getDefaultCursor(DEF_TEXT_CURSOR));

	_scint->SetDragPosition(invalidPosition);
	return 1;
  }


// Dragged stuff around
long FXScintilla::onDragged(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  FXDragAction action;
  if(FXScrollArea::onDragged(sender,sel,ptr)) return 1;
  action=DRAG_COPY;
	if (!_scint->pdoc->IsReadOnly()) {
    if(isDropTarget()) action=DRAG_MOVE;
    if(event->state&CONTROLMASK) action=DRAG_COPY;
    if(event->state&SHIFTMASK) action=DRAG_MOVE;
    }
  handleDrag(event->root_x,event->root_y,action);
  if(didAccept()!=DRAG_REJECT){
    if(action==DRAG_MOVE)
      setDragCursor(getApp()->getDefaultCursor(DEF_DNDMOVE_CURSOR));
    else
      setDragCursor(getApp()->getDefaultCursor(DEF_DNDCOPY_CURSOR));
    }
  else{
    setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));
    }
  return 1;
  }


// Handle drag-and-drop enter
long FXScintilla::onDNDEnter(FXObject* sender,FXSelector sel,void* ptr){
  FXScrollArea::onDNDEnter(sender,sel,ptr);
  return 1;
  }


// Handle drag-and-drop leave
long FXScintilla::onDNDLeave(FXObject* sender,FXSelector sel,void* ptr){
  stopAutoScroll();
  FXScrollArea::onDNDLeave(sender,sel,ptr);
  return 1;
  }

// Handle drag-and-drop motion
long FXScintilla::onDNDMotion(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent* ev=(FXEvent*)ptr;
  FXDragAction action;
  FXint pos;

  // Scroll into view
  if(startAutoScroll(ev->win_x,ev->win_y,TRUE)) return 1;

  // Handled elsewhere
  if(FXScrollArea::onDNDMotion(sender,sel,ptr)) return 1;

  // Correct drop type
  if(offeredDNDType(FROM_DRAGNDROP,textType)){

    // Is target editable?
		if (!_scint->pdoc->IsReadOnly()) {
      action=inquireDNDAction();

      // Check for legal DND action
      if(action==DRAG_COPY || action==DRAG_MOVE){
				Point npt(ev->win_x, ev->win_y);
				pos = _scint->PositionFromLocation(npt);
				if (!_scint->inDragDrop) {
					_scint->inDragDrop = true;
					_scint->ptMouseLastBeforeDND = _scint->ptMouseLast;
				}
				_scint->ptMouseLast = npt;
				_scint->SetDragPosition(pos);
				if (_scint->PositionInSelection(pos) != 0) {
          acceptDrop(DRAG_ACCEPT);
        }
      }
    }
    return 1;
  }

  // Didn't handle it here
  return 0;
}


// Handle drag-and-drop drop
long FXScintilla::onDNDDrop(FXObject* sender,FXSelector sel,void* ptr){
  FXuchar *data,*junk; FXuint len,dum;

  // Stop scrolling
  stopAutoScroll();

  // Try handling it in base class first
  if(FXScrollArea::onDNDDrop(sender,sel,ptr)) return 1;

  // Should really not have gotten this if non-editable
	if (!_scint->pdoc->IsReadOnly()) {

	  // Try handle here
	  if(getDNDData(FROM_DRAGNDROP,textType,data,len)){
			_scint->dragWasDropped = true;
	    FXRESIZE(&data,FXchar,len+1); data[len]='\0';

	    // Need to ask the source to delete his copy
  	  if(inquireDNDAction()==DRAG_MOVE){
    	  getDNDData(FROM_DRAGNDROP,deleteType,junk,dum);
      	FXASSERT(!junk);
	    }

	    // Insert the new text
			bool isRectangular = ((len > 0) &&
					(data[len] == 0 && data[len-1] == '\n'));
			_scint->DropAt(_scint->posDrop, (const char *)(data), false, isRectangular);
  	
    	FXFREE(&data);
	  }
		else if (getDNDData(FROM_DRAGNDROP,urilistType,data,len)) {
			_scint->dragWasDropped = true;
	    FXRESIZE(&data,FXchar,len+1); data[len]='\0';
			_scint->NotifyURIDropped((FXchar *)data);
		}
  	return 1;
	}
  return 0;
}

// Service requested DND data
long FXScintilla::onDNDRequest(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *data;

  // Perhaps the target wants to supply its own data
  if(FXScrollArea::onDNDRequest(sender,sel,ptr)) return 1;

  // Return dragged text
  if(event->target==textType){
		if (_scint->primary.s == NULL) {
			_scint->CopySelectionRange(&_scint->primary);

		}

    setDNDData(FROM_DRAGNDROP,stringType,(FXuchar *)strdup(_scint->primary.s),strlen(_scint->primary.s));
    return 1;
    }

  // Delete dragged text
  if(event->target==deleteType){
		if (!_scint->pdoc->IsReadOnly()) {
			if (isDragging()) {
				int selStart = _scint->SelectionStart();
				int selEnd = _scint->SelectionEnd();
				if (_scint->posDrop > selStart) {
					if (_scint->posDrop > selEnd)
						_scint->posDrop = _scint->posDrop - (selEnd-selStart);
					else
						_scint->posDrop = selStart;
					_scint->posDrop = _scint->pdoc->ClampPositionIntoDocument(_scint->posDrop);
				}
			}
      _scint->ClearSelection();
      }
    return 1;
    }

  return 0;
  }

// ********************************************************************
// Selection
// ********************************************************************

// We lost the selection somehow
long FXScintilla::onSelectionLost(FXObject* sender,FXSelector sel,void* ptr){
  FXbool hadselection=hasSelection();
  FXScrollArea::onSelectionLost(sender,sel,ptr);
  if (hadselection) {
		_scint->UnclaimSelection();
	}
  return 1;
}


// Somebody wants our selection
long FXScintilla::onSelectionRequest(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent *event=(FXEvent*)ptr;

  // Perhaps the target wants to supply its own data for the selection
  if(FXScrollArea::onSelectionRequest(sender,sel,ptr)) return 1;

  // Return text of the selection
  if(event->target==stringType){
		if (_scint->primary.s == NULL) {
			_scint->CopySelectionRange(&_scint->primary);
		}

    setDNDData(FROM_SELECTION,stringType,(FXuchar *)strdup(_scint->primary.s),strlen(_scint->primary.s));
    return 1;
  }

  return 0;
}

// ********************************************************************
// Scrolling
// ********************************************************************

FXint FXScintilla::getViewportWidth()
{
	return _scint->GetTextRectangle().Width();
	//return FXScrollArea::getViewportWidth();
}

FXint FXScintilla::getViewportHeight()
{
	//return (_scint) ? _scint->GetTextRectangle().Height() : FXScrollArea::getViewportHeight();
	return FXScrollArea::getViewportHeight();
}

FXint FXScintilla::getContentWidth()
{
	return FXMAX(_scint->scrollWidth, 0);
}

FXint FXScintilla::getContentHeight()
{
	return
		_scint->cs.LinesDisplayed() * _scint->vs.lineHeight +
		// This part is a kind of magic, to have the correct scrollbar
		// behavior regarding the last line of the text
#ifndef WIN32
		(getViewportHeight() + 1) % _scint->vs.lineHeight - 1;
#else
		(getViewportHeight() - 2) % _scint->vs.lineHeight;
#endif
}

void FXScintilla::moveContents(FXint x,FXint y)
{
	bool moved = false;
	int line = (-y + _scint->vs.lineHeight / 2) / _scint->vs.lineHeight;
	if (line != getYPosition()/_scint->vs.lineHeight) {
		moved = true;
		_scint->ScrollTo(line);
	}
	if (x != getXPosition()) {
		moved = true;
		_scint->HorizontalScrollTo(-x);
	}
	if (moved) {
		FXScrollArea::moveContents(x, -line * _scint->vs.lineHeight);
	}
}

long FXScintilla::onConfigure(FXObject *sender, FXSelector sel, void * ptr)
{
	long ret = FXScrollArea::onConfigure(sender, sel, ptr);
	_scint->ChangeSize();
	return ret;
}

// ********************************************************************
// Messenging
// ********************************************************************

void FXScintilla::setScintillaID(int id)
{
	_scint->ctrlID = id;
}

sptr_t FXScintilla::sendMessage(unsigned int iMessage, uptr_t wParam, sptr_t lParam) {
	return _scint->WndProc(iMessage, wParam, lParam);
}

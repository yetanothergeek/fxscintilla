/** FXSciTE - FXScintilla based Text Editor
 *
 *  DirectorExtension.h - Extension for communicating with a director program
 *
 *  Copyright 2001-2002 by Gilles Filippini <gilles.filippini@free.fr>
 *
 *  Adapted from the SciTE GTK source DirectorExtension.h
 *  Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
 *
 *  ====================================================================
 *
 *  This file is part of FXSciTE.
 * 
 *  FXSciTE is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  FXSciTE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with FXSciTE; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **/

#include <fox/fx.h>

class DirectorExtension : public Extension, public FXObject {
	FXDECLARE(DirectorExtension);
public:
	enum {
		ID_PIPESIGNAL = 1,
		ID_LAST
	};
public:
	long onPipeSignal(FXObject * sender, FXSelector sel, void * ptr);
public:
	DirectorExtension();
	virtual ~DirectorExtension();

	// Implement the Extension interface
	virtual bool Initialise(ExtensionAPI *host_);
	virtual bool Finalise();
	virtual bool Clear();
	virtual bool Load(const char *filename);

	virtual bool OnOpen(const char *path);
	virtual bool OnSwitchFile(const char *path);
	virtual bool OnSave(const char *path);
	virtual bool OnChar(char ch);
	virtual bool OnExecute(const char *s);
	virtual bool OnSavePointReached();
	virtual bool OnSavePointLeft();
	virtual bool OnStyle(unsigned int startPos, int lengthDoc, int initStyle, Accessor *styler);
	virtual bool OnDoubleClick();
	virtual bool OnUpdateUI();
	virtual bool OnMarginClick();
	virtual bool OnMacro(const char *command, const char *params);
	
	virtual bool SendProperty(const char *prop);

	// Allow messages through to extension
	void HandleStringMessage(const char *message);
	
	bool CreatePipe(bool forceNew = false);
private:
	int fdReceiver;
};


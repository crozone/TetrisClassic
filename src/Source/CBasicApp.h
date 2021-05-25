// ===========================================================================
//	CBasicApp.h			�1994-2002 Metrowerks Corporation. All rights reserved.
// ===========================================================================

#ifndef _H_CBasicApp
#define _H_CBasicApp
#pragma once

#include <LApplication.h>
#include <LListener.h>

class CBasicApp : public LApplication, public LListener {

public:
							CBasicApp();
	virtual					~CBasicApp();

	// LCommander
	virtual Boolean			ObeyCommand(
								CommandT			inCommand,
								void*				ioParam = nil);	

	virtual void			FindCommandStatus(
								CommandT			inCommand,
								Boolean&			outEnabled,
								Boolean&			outUsesMark,
								UInt16&				outMark,
								Str255				outName);
								
	virtual Boolean			HandleKeyPress( const EventRecord& inKeyEvent );
								
	// LListener
	virtual void			ListenToMessage( MessageT inMessage, void *ioParam );

protected:
	virtual void			StartUp();
	
			void			RegisterClasses();
			
			LWindow*		mPrimaryWindow;
};

#endif // _H_CBasicApp
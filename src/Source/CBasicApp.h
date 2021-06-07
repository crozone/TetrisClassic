//
// CBasicApp is the entry point for the application.
//


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
			
			void			SwitchPrimaryWindowTo(LWindow* window);
			
			LWindow*		mPrimaryWindow;
			
			LWindow*		mSetupWindow;
			
			LWindow*		mGameWindow;
			
			// LWindow*		mHighScoresWindow;
};

#endif // _H_CBasicApp
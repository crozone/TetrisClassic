#ifndef _H_CTetrisPane
#define _H_CTetrisPane
#pragma once

#include <LPane.h>
#include <LCommander.h>
#include <LPeriodical.h>

#include "CTetrisGame.h"

///
/// A pane that renders Tetris game state, updates it periodically,
/// handles input commands from the keyboard, and broadcasts changes in game state.
///
class CTetrisPane :
	public LPane, 
	public LPeriodical,
	public LCommander
	//public LListener,
	//public LBroadcaster
	{

public:
	// Tetr = Tetris
	// Use at least one capital letter to avoid conflict with
	// CodeWarrior built in class ids
	enum { class_ID = FOUR_CHAR_CODE('Tetr') };

							CTetrisPane();
							CTetrisPane(const SPaneInfo	&inPaneInfo);
							CTetrisPane(LStream			*inStream);

	virtual					~CTetrisPane();
	
				void		NewGame(CTetrisGame* inTetrisGame);
				
				void		PauseGame();
				
				void		ResumeGame();
							
	// LPane
	virtual		void		DrawSelf();
	
	// LPeriodical
	virtual		void		SpendTime(const EventRecord& inMacEvent);
	
	// LCommander
	virtual		Boolean		HandleKeyPress( const EventRecord& inKeyEvent );
								
	// LListener
	//virtual void			ListenToMessage( MessageT inMessage, void *ioParam );

protected:
	Boolean mGameActive;
	SInt32 mLastUpdateTime;
	SInt32 mNextUpdateThreshold;
	CTetrisGame* mTetrisGame;
};

#endif // _H_CTetrisPane
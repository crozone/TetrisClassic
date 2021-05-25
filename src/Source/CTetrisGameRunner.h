#ifndef _H_CTetrisGameRunner
#define _H_CTetrisGameRunner
#pragma once

#include <LCommander.h>
#include <LPeriodical.h>

#include "CTetrisGame.h"

// This class runs the actual Tetris game, acting as a commander, periodical, and broadcaster.
// Commander:	Handles key presses and converts them to Tetris game calls
// Periodical:	Triggers Tetris game ticks after a certain amount of time has passed
// Broadcaster:	Notifies listeners of state updates. Listeners will be UI elements such as the game board,
// 				hold and piece preview panes, and score labels.
//
// This should be added as a subcommander to the main window and activated when the game is started.
//
class CTetrisGameRunner :
	public LPeriodical,
	public LCommander,
	//public LListener,
	public LBroadcaster
{

public:

							CTetrisGameRunner();

	virtual					~CTetrisGameRunner();
	
				void		NewGame();
				
				void		PauseGame();
				
				void		ResumeGame();
	
	// LPeriodical
	virtual		void		SpendTime(const EventRecord& inMacEvent);
	
	// LCommander
	virtual		Boolean		HandleKeyPress( const EventRecord& inKeyEvent );
								
	// LListener
	//virtual void			ListenToMessage( MessageT inMessage, void *ioParam );

protected:
				void		NotifyGameStateChanged();

	Boolean mGameActive;
	UInt32 mLastUpdateTime;
	SInt32 mTimeRemainingOnTick;
	CTetrisGame mTetrisGame;
};

#endif // _H_CTetrisGameRunner
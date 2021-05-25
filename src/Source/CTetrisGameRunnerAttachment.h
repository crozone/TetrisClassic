#ifndef _H_CTetrisGameRunnerAttachment
#define _H_CTetrisGameRunnerAttachment
#pragma once

#include <LAttachment.h>
#include "CTetrisGame.h"

class	CTetrisGameRunnerAttachment :
	public LAttachment,
	public LListener,
	public LPeriodical,
	public LBroadcaster
	{
public:
	enum { class_ID = FOUR_CHAR_CODE('TGRA') };

					CTetrisGameRunnerAttachment();
					CTetrisGameRunnerAttachment(LStream* inStream);
					
	// LPeriodical
	virtual		void		SpendTime(const EventRecord& inMacEvent);

protected:
	Boolean mInitialized;
	Boolean mGameActive;
	UInt32 mLastUpdateTime;
	SInt32 mTimeElapsedOnTick;
	CTetrisGame* mTetrisGame;
	
	Boolean	HandleKeyPress(const EventRecord& inKeyEvent);
	void	HandleGameCommandMessage(MessageT inMessage, void* ioParam);
	void	GameStateChanged();
	
	void	EnsureInitialized();
	void	NewGame();
	void	PauseGame();
	void	ResumeGame();
	void	AddRenderPanesAsListeners();
	void	AttachTetrisPanesRecursively(LPane* pane);
	
	// LListener
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);
	// LAttachment
	virtual void	ExecuteSelf(MessageT inMessage, void* ioParam);
};

#endif // _H_CTetrisGameRunnerAttachment
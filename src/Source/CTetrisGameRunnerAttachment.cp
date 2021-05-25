#include "CTetrisGameRunnerAttachment.h"
#include "CTetrisPane.h"

CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment()
: LAttachment(msg_KeyPress), LPeriodical(), LBroadcaster(),
	mInitialized(FALSE),
	mGameActive(FALSE),
	mLastUpdateTime(0),
	mTimeElapsedOnTick(0),
	mTetrisGame()
{
}


CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment(LStream*	inStream)
	: LAttachment(inStream), LPeriodical(), LBroadcaster(),
	mInitialized(FALSE),
	mGameActive(FALSE),
	mLastUpdateTime(0),
	mTimeElapsedOnTick(0),
	mTetrisGame()
{
	//mMessage = msg_KeyPress;
	mMessage = msg_AnyMessage;
}

void
CTetrisGameRunnerAttachment::EnsureInitialized() {
	if(!mInitialized) {
		AddRenderPanesAsListeners();
		mInitialized = true;
	}
}

// Assumes the attachable that this attachment is attached to
// is a View with subpanes and subviews that at some depth, contain
// Tetris Render Panes that will be used to render the Tetris game board.
//
// This method locates the TRPs and adds them as a listener to
// the game runner broadcaster.
void
CTetrisGameRunnerAttachment::AddRenderPanesAsListeners() {
	// The attachable is what this attachment is currently connected to
	LAttachable* attachable = this->GetOwnerHost();
	
	// The attachable is usually a Pane
	LPane* pane = dynamic_cast<LPane*>(attachable);
	
	if(pane == nil) {
		// If we have no LPane host, there's nothing to do here.
		return;
	}
	
	AttachTetrisPanesRecursively(pane);
}

void
CTetrisGameRunnerAttachment::AttachTetrisPanesRecursively(LPane* pane) {
	// This is the recursive algorithm for finding panes,
	// stolen from the LView FindPaneById code
	
	// Pane should not be nil at this point
	ThrowIfNil_(pane);
	
	// Check the current pane to see if it is a Tetris Render Pane
	CTetrisPane* tetrisPane = dynamic_cast<CTetrisPane*>(pane);
	if (tetrisPane != nil) {			// Check first if this is the one
		this->AddListener(tetrisPane);
	} else {
		// Check if this is a view, and if so recursively traverse 
		LView* view = dynamic_cast<LView*>(pane);
		if(view != nil) {
			// Search all subpanes
			TArray<LPane*>& subPanes = view->GetSubPanes();
			TArrayIterator<LPane*> iterator(subPanes);
			LPane	*theSub = nil;
			while (iterator.Next(theSub)) {
				AttachTetrisPanesRecursively(theSub);
			}
		}
	}
}

void
CTetrisGameRunnerAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	switch(inMessage) {
		case 1001:
			// New game command
			NewGame();
		break;
		case 1002:
			// Pause game command
			PauseGame();
		break;
		case 1003:
			// Resume game command
			ResumeGame();
		break;
		case msg_FinishCreate:
			EnsureInitialized();
			// The pane hierarchy has been created, we can initialize 
		case msg_KeyPress:
			EventRecord* inKeyEventPtr = static_cast<EventRecord*>(ioParam);
			HandleKeyPress(*inKeyEventPtr);
		break;
	}
}
	
void
CTetrisGameRunnerAttachment::NewGame() {
	if(!mInitialized) {
		Throw_(-1);
	}

	mTetrisGame = CTetrisGame::CTetrisGame(0);
	mTimeElapsedOnTick = 0;
	mGameActive = TRUE;
	StartRepeating();
	
	GameStateChanged();
}

void
CTetrisGameRunnerAttachment::PauseGame() {
	mGameActive = FALSE;
	StopRepeating();
	
	GameStateChanged();
}

void
CTetrisGameRunnerAttachment::ResumeGame() {
	mGameActive = TRUE;
	StartRepeating();
	
	GameStateChanged();
}

// LPeriodical
void
CTetrisGameRunnerAttachment::SpendTime(const EventRecord& inMacEvent) {
	UInt32 currentTime = inMacEvent.when;
	UInt32 timeDelta = mLastUpdateTime == 0 ? 0 : currentTime - mLastUpdateTime;
	mLastUpdateTime = currentTime;
	
	if(!mGameActive || !mInitialized) {
		return;
	}
	
	if(timeDelta == 0) {
		// Either the first time through SpendTime,
		// or no time has passed.
		return;
	}
	
	if(timeDelta > 3000) {
		// If the time delta was too large, assume the window was out of focus.
		// Pause the game.
		//PauseGame();
		
		// Just ignore this time delay
		return;
	}

	mTimeElapsedOnTick += timeDelta;
	
	while(TRUE) {
		SInt32 tickDelay = mTetrisGame.GetCurrentTickDelay();
	
		if(tickDelay < 0) {
			// Infinite tick
			// Reset elapsed so that if the tick delay changes,
			// we don't end up doing many game ticks in a row.
			mTimeElapsedOnTick = 0;
			break;
		}
		else if(mTimeElapsedOnTick >= tickDelay) {
			mTimeElapsedOnTick -= tickDelay;
		
			if(mTetrisGame.DoGameTick()) {
				GameStateChanged();
			}
			else {
				// Game over
				PauseGame();
			}
		}
		else {
			break;
		}
	}
}

void
CTetrisGameRunnerAttachment::GameStateChanged() {
	BroadcastMessage(2000, &mTetrisGame);
}
	
Boolean	
CTetrisGameRunnerAttachment::HandleKeyPress( const EventRecord& inKeyEvent ) {
	// what: Should be a keyDown event, or a key repeat event (autoKey)
	if(inKeyEvent.what != keyDown && inKeyEvent.what != autoKey) {
		return FALSE;
	}
	
	if(!mGameActive || !mInitialized) {
		return FALSE;
	}
	
	// message:
	// Get the Character code and virtual key code from the low-order word.
	// For Apple Desktop Bus keyboards, the low byte of the high-order word
	// contains the ADB address of the keyboard where the keyboard event occured.
	
	// Get the character code and virtual key code
	UInt8 charCode = inKeyEvent.message & charCodeMask;
	UInt8 keyCode = inKeyEvent.message & keyCodeMask;
	UInt8 adbAddr = inKeyEvent.message & adbAddrMask;
	Boolean keyRepeat = inKeyEvent.what == autoKey;
	
	// when:
	// The time when the event was posted, in ticks since system startup
	
	// where:
	// For low level events (like keydown), contains the location of the
	// mouse cursor in global coordinates.
	// For high level events, contains the event ID.
	
	// TODO: Make keys configurable
	switch(charCode) {
		case 'w':
		case 'W':
			if(mTetrisGame.DoPieceHardDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
			return TRUE;
		case 'a':
		case 'A':
			if(mTetrisGame.DoPieceLeft()) {
				GameStateChanged();
			}
			return TRUE;
		case 's':
		case 'S':
			if(mTetrisGame.DoPieceSoftDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
			return TRUE;
		case 'd':
		case 'D':
			if(mTetrisGame.DoPieceRight()) {
				GameStateChanged();
			}
			return TRUE;
		case 'q':
		case 'Q':
			if(mTetrisGame.DoPieceRotateCCW()) {
				GameStateChanged();
			}
			return TRUE;
		case 'e':
		case 'E':
			if(mTetrisGame.DoPieceRotateCW()) {
				GameStateChanged();
			}
			return TRUE;
		case 'r':
		case 'R':
			if(mTetrisGame.DoPieceHold()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
			return TRUE;
		case 'n':
		case 'N':
			// For testing: Do a game tick manually
			mTetrisGame.DoGameTick();
			GameStateChanged();
			return TRUE;
	}
	
	return FALSE;
}
#include "CTetrisGameRunnerAttachment.h"
#include "CTetrisPane.h"

CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment()
: LAttachment(msg_KeyPress)
{
	AddRenderPanesAsListeners();
}


CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment(LStream*	inStream)
	: LAttachment(inStream)
{
	mMessage = msg_KeyPress;
	AddRenderPanesAsListeners();
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
	
	// Try to cast the attachable to a View so the subpanes can be accessed
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
		LView* view = dynamic_cast<LView*>(pane);
	
		// Search all subpanes
		TArrayIterator<LPane*> iterator(view->GetSubPanes());
		LPane	*theSub;
		while (iterator.Next(theSub)) {
			AttachTetrisPanesRecursively(theSub);
		}
	}
}

void
CTetrisGameRunnerAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{	
	switch(inMessage) {
		case msg_KeyPress:
			EventRecord* inKeyEventPtr = static_cast<EventRecord*>(ioParam);
			HandleKeyPress(*inKeyEventPtr);
		break;
	}
}
	
void
CTetrisGameRunnerAttachment::NewGame() {
	mTetrisGame = CTetrisGame::CTetrisGame(0);
	mTimeRemainingOnTick = mTetrisGame.GetCurrentTickDelay();
	mGameActive = TRUE;
}

void
CTetrisGameRunnerAttachment::PauseGame() {
	mGameActive = FALSE;
}

void
CTetrisGameRunnerAttachment::ResumeGame() {
	mGameActive = TRUE;
}

// LPeriodical
void
CTetrisGameRunnerAttachment::SpendTime(const EventRecord& inMacEvent) {
	UInt32 currentTime = inMacEvent.when;
	UInt32 timeDelta = currentTime - mLastUpdateTime;
	mLastUpdateTime = currentTime;
	
	if(!mGameActive) {
		return;
	}
	
	if(timeDelta > 1000) {
		// If the time delta was too large, assume the window was out of focus.
		// Pause the game.
		PauseGame();
		return;
	}

	mTimeRemainingOnTick -= timeDelta;
	
	while(mTimeRemainingOnTick < 0) {
		SInt32 tickDelay = mTetrisGame.GetCurrentTickDelay();
		
		if(tickDelay >= 0) {
			mTimeRemainingOnTick += tickDelay;
			mTetrisGame.DoGameTick();
		}
		else {
			// Infinite time, we shouldn't call game tick
			mTimeRemainingOnTick = 0;
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
			mTetrisGame.DoPieceHardDrop();
			GameStateChanged();
			return TRUE;
		case 'a':
		case 'A':
			mTetrisGame.DoPieceLeft();
			GameStateChanged();
			return TRUE;
		case 's':
		case 'S':
			mTetrisGame.DoPieceSoftDrop();
			GameStateChanged();
			return TRUE;
		case 'd':
		case 'D':
			mTetrisGame.DoPieceRight();
			GameStateChanged();
			return TRUE;
		case 'q':
		case 'Q':
			mTetrisGame.DoPieceRotateCCW();
			GameStateChanged();
			return TRUE;
		case 'e':
		case 'E':
			mTetrisGame.DoPieceRotateCW();
			GameStateChanged();
			return TRUE;
		case 'r':
		case 'R':
			mTetrisGame.DoPieceHold();
			GameStateChanged();
			return TRUE;
	}
	
	return FALSE;
}
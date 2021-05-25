#include "CTetrisGameRunner.h"


CTetrisGameRunner::CTetrisGameRunner() {
}

CTetrisGameRunner::~CTetrisGameRunner() {
}
	
void
CTetrisGameRunner::NewGame() {
	mTetrisGame = CTetrisGame::CTetrisGame(0);
	mTimeRemainingOnTick = mTetrisGame.GetCurrentTickDelay();
	mGameActive = TRUE;
}
				
void
CTetrisGameRunner::PauseGame() {
	mGameActive = FALSE;
}
				
void
CTetrisGameRunner::ResumeGame() {
	mGameActive = TRUE;
}
	
// LPeriodical
void
CTetrisGameRunner::SpendTime(const EventRecord& inMacEvent) {
	UInt32 currentTime = inMacEvent.when;
	mLastUpdateTime = currentTime;
	UInt32 timeDelta = currentTime - mLastUpdateTime;
	
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
		mTimeRemainingOnTick += mTetrisGame.GetCurrentTickDelay();
		mTetrisGame.DoGameTick();
		
	}
}

void
CTetrisGameRunner::NotifyGameStateChanged() {
	// TODO: Make message id constant
	BroadcastMessage(2000, &mTetrisGame);
}
	
// LCommander
Boolean	
CTetrisGameRunner::HandleKeyPress( const EventRecord& inKeyEvent ) {
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
			NotifyGameStateChanged();
			return TRUE;
		case 'a':
		case 'A':
			mTetrisGame.DoPieceLeft();
			NotifyGameStateChanged();
			return TRUE;
		case 's':
		case 'S':
			mTetrisGame.DoPieceSoftDrop();
			NotifyGameStateChanged();
			return TRUE;
		case 'd':
		case 'D':
			mTetrisGame.DoPieceRight();
			NotifyGameStateChanged();
			return TRUE;
		case 'q':
		case 'Q':
			mTetrisGame.DoPieceRotateCCW();
			NotifyGameStateChanged();
			return TRUE;
		case 'e':
		case 'E':
			mTetrisGame.DoPieceRotateCW();
			NotifyGameStateChanged();
			return TRUE;
		case 'r':
		case 'R':
			mTetrisGame.DoPieceHold();
			NotifyGameStateChanged();
			return TRUE;
	}
	
	return FALSE;
}
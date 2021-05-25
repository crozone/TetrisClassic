// Tetris render pane implementation

#include "CTetrisPane.h"

CTetrisPane::CTetrisPane()
: LPane::LPane() {

}

CTetrisPane::CTetrisPane(const SPaneInfo &inPaneInfo)
: LPane::LPane(inPaneInfo) {

}

CTetrisPane::CTetrisPane(LStream *inStream)
: LPane::LPane(inStream) {
	StartRepeating();
}

CTetrisPane::~CTetrisPane() {

}

// CTetrisPane
void
CTetrisPane::NewGame(CTetrisGame* inTetrisGame) {
	mTetrisGame = inTetrisGame;
	ResumeGame();
}

void
CTetrisPane::PauseGame() {
	StopRepeating();
	mGameActive = FALSE;
}

void
CTetrisPane::ResumeGame() {
	//ThrowIfNil_(mTetrisGame);

	StartRepeating();
	mGameActive = TRUE;
}

// LPane
void
CTetrisPane::DrawSelf() {
	Rect frameRect;
	// CalcLocalFrameRect returns the paneÕs frame
	// as a QuickDraw rectangle in local coordinates
	CalcLocalFrameRect(frameRect);
	
	// Define the rectangle in which the piece squares will be drawn
	Rect gameRect = frameRect;
	MacInsetRect(&gameRect, 2, 2);
	
	// TODO:
	// * Learn macOS QuickDraw
	// * Get this method to draw the Tetris gameboard from state
	
	::PenNormal();
	
	UInt32 gameWidth = 10;
	UInt32 gameHeight = 20;
	
	UInt32 scaledSquareWidth = (gameRect.right - gameRect.left) / gameWidth;
	UInt32 scaledSquareHeight = (gameRect.bottom - gameRect.top) / gameHeight;
	
	UInt32 squareWidth = scaledSquareWidth > scaledSquareHeight
						? scaledSquareWidth : scaledSquareHeight;
		
	
	Rect basePieceRect;
	MacSetRect(&basePieceRect, 0, 0, squareWidth, squareWidth);
	MacInsetRect(&basePieceRect, 1, 1);
	MacOffsetRect(&basePieceRect, gameRect.left, gameRect.top);
	
	for(UInt8 j = 0; j < gameHeight; j++) {
		for(UInt8 i = 0; i < gameWidth; i++) {
		
			if(mLastUpdateTime % 3 == 0) {
				// Draw a single piece rectangle
				Rect currentPieceRect = basePieceRect;
				// Offset the piece by the current ammount
				MacOffsetRect(&currentPieceRect, i * squareWidth, j * squareWidth);
			
				// Paint the rectangle
				::PaintRect(&currentPieceRect);
			}
		}
	}
		
	// Draws boarder
	::MacFrameRect(&gameRect);
	
	// Draws boarder
	::MacFrameRect(&frameRect);
	
	// Draw temp standin cross
	::MoveTo(frameRect.left, frameRect.top);
	::LineTo(frameRect.right, frameRect.bottom);
	::MoveTo(frameRect.right, frameRect.top);
	::LineTo(frameRect.left, frameRect.bottom);
}

// LPeriodical
void
CTetrisPane::SpendTime(const EventRecord& inMacEvent) {
	if(!mGameActive) {
		return;
	}

	// TODO: Handle Tetris time based updates here and update state
	while(inMacEvent.when >= mNextUpdateThreshold) {
		// TODO: Get update timespan from tetris game state
		UInt32 timeDelay = 1000;
		mNextUpdateThreshold += timeDelay;
		
		// Drop game piece down by one
	}
	
	mLastUpdateTime = inMacEvent.when;
	Refresh();
}

// LCommander
Boolean
CTetrisPane::HandleKeyPress(const EventRecord& inKeyEvent) {
	// what: Should be a keyDown event, or a key repeat event (autoKey)
	if(inKeyEvent.what != keyDown && inKeyEvent.what != autoKey) {
		Throw_(-1);
	}

	if(!mGameActive) {
		return FALSE;
	}

	// TODO: Handle Tetris keypresses here and update state

	
	
	
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
	
	return FALSE;
}
//
// The purpose of CTetrisKeyHandlerAttachment is to receive key presses from the attached pane,
// look them up in a keybinds mapping, and transmit the appropriate broadcast messages to control
// the active Tetris game.
//
// Upon initialization, the attachment searches the host pane view hierarchy for any LListeners
// and adds them as a broadcast receiver.
//
// It is intended to be attached to a pane that contains somewhere in its view hierarchy
// a CTetrisGameRunnerAttachment, or any other LListener that cares about Tetris game control messages.
//
// Note: LPeriodical is implemented since we will probably want to do our own key repeat internally,
//       without relying on the operating system key repeat behaviour. However, it is not currently
//       used and we simply use the os key repeat.
//
// Note: LListener is implemented since we may want to pick up keymapping changes via broadcast
//       from whatever is setting up the game.
//

#include "CTetrisKeyHandlerAttachment.h"
#include "PaneHelpers.h"
#include "TetrisMessage.h"

using namespace PaneHelpers;

CTetrisKeyHandlerAttachment::CTetrisKeyHandlerAttachment()
: LAttachment(msg_AnyMessage), LPeriodical(), LBroadcaster(), LListener(),
	mInitialized(FALSE),
	mLastUpdateTime(0)
{
}


CTetrisKeyHandlerAttachment::CTetrisKeyHandlerAttachment(LStream*	inStream)
	: LAttachment(inStream), LPeriodical(), LBroadcaster(), LListener(),
	mInitialized(FALSE),
	mLastUpdateTime(0)
{
	mMessage = msg_AnyMessage;
}

void
CTetrisKeyHandlerAttachment::EnsureInitialized() {
	if(!mInitialized) {
		mInitialized = true;
	}
}

// LListener
void
CTetrisKeyHandlerAttachment::ListenToMessage(MessageT inMessage, void *ioParam) {
	
}

// LAttachment
void
CTetrisKeyHandlerAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	switch(inMessage) {
		// UI messages
		case msg_FinishCreate:
			EnsureInitialized();
			// The pane hierarchy has been created, we can initialize now.
		break;
		// Keypress
		case msg_KeyPress:
			EventRecord* inKeyEventPtr = static_cast<EventRecord*>(ioParam);
			HandleKeyPress(*inKeyEventPtr);
		break;
	}
}

Boolean	
CTetrisKeyHandlerAttachment::HandleKeyPress( const EventRecord& inKeyEvent ) {
	if(!mInitialized) {
		return FALSE;
	}
	
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
			// Hard drop
			BroadcastMessage(msg_TetrisHardDropPiece, NULL);
			return TRUE;
		case 'a':
		case 'A':
			// Piece left
			BroadcastMessage(msg_TetrisMovePieceLeft, NULL);
			return TRUE;
		case 's':
		case 'S':
			// Soft drop
			BroadcastMessage(msg_TetrisSoftDropPiece, NULL);
			return TRUE;
		case 'd':
		case 'D':
			// Piece right
			BroadcastMessage(msg_TetrisMovePieceRight, NULL);
			return TRUE;
		case 'q':
		case 'Q':
			// Piece rotate CCW
			BroadcastMessage(msg_TetrisRotatePieceCCW, NULL);
			return TRUE;
		case 'e':
		case 'E':
			// Piece rotate CW
			BroadcastMessage(msg_TetrisRotatePieceCW, NULL);
			return TRUE;
		case 'r':
		case 'R':
			// Hold piece
			BroadcastMessage(msg_TetrisHoldPiece, NULL);
			return TRUE;
		case 'n':
		case 'N':
			// For testing: Do a game tick manually
			BroadcastMessage(msg_TetrisGameTick, NULL);
			return TRUE;
	}
	
	return FALSE;
}

// LPeriodical
void
CTetrisKeyHandlerAttachment::SpendTime(const EventRecord& inMacEvent) {
	UInt32 currentTime = inMacEvent.when;
	UInt32 timeDelta = mLastUpdateTime == 0 ? 0 : currentTime - mLastUpdateTime;
	mLastUpdateTime = currentTime;
}
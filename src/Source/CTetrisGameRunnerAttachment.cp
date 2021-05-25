//
// The purpose of CTetrisGameRunnerAttachment is to coordinate the high level game logic.
// It is intended to be attached to a pane that contains subpains for the Tetris game elements.
//
// This attachment handles:
//
// * Key inputs for the game (TODO: Move this)
// * Game command messages (start, stop, resume, move piece, do tick)
// * Periodic game updates (by implementing LPeriodical)
// * Broadcasts game updates to interested Listeners
//    Any listeners that exist within subpains of the attached pane will be automatically and
//    recursively attached as a broadcast listener
//    (via AddRenderPanesAsListeners(), called in EnsureInitialized())
//

#include "CTetrisGameRunnerAttachment.h"
#include "CTetrisPane.h"

CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment()
: LAttachment(msg_AnyMessage), LPeriodical(), LBroadcaster(), LListener(),
	mInitialized(FALSE),
	mGameActive(FALSE),
	mLastUpdateTime(0),
	mTimeElapsedOnTick(0),
	mTetrisGame(NULL)
{
}


CTetrisGameRunnerAttachment::CTetrisGameRunnerAttachment(LStream*	inStream)
	: LAttachment(inStream), LPeriodical(), LBroadcaster(), LListener(),
	mInitialized(FALSE),
	mGameActive(FALSE),
	mLastUpdateTime(0),
	mTimeElapsedOnTick(0),
	mTetrisGame(NULL)
{
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
	// The attachable is the host to which this attachment is currently connected.
	LAttachable* attachable = this->GetOwnerHost();
	
	// The attachable is usually a Pane
	LPane* pane = dynamic_cast<LPane*>(attachable);
	
	if(pane == nil) {
		// If we have no LPane host, there's nothing to do here.
		return;
	}
	
	AttachTetrisPanesRecursively(pane);
	
	// Note: We currently rely on a side effect of the above call to AttachTetrisPanesRecursively
	//       as an interim measure.
	//
	//       Since this attachment implements LListener and is attached to the parent pane,
	//       AttachTetrisPanesRecursively will actually subscribe this attachment to itself.
	//       i.e. this attachment can send messages to itself via BroadcastMessage() like a loopback.
	//
	//       It is intended that all the code that needs to broadcast messages to this attachment
	//       will eventually be moved into their own attachment, for greater separation of concerns.
	//       Both the keypress handling, and even LPeriodic game updates, will be moved, however the code
	//       will not need to be updated much since the BroadcastMessage() mechanism will remain the same.
	
}

void
CTetrisGameRunnerAttachment::AttachTetrisPanesRecursively(LPane* pane) {
	// This is the recursive algorithm for finding panes,
	// stolen from the LView FindPaneById code
	
	// Pane should not be nil at this point
	ThrowIfNil_(pane);
	
	// Check the current pane to see if it is an LListener
	LListener* listener = dynamic_cast<LListener*>(pane);
	// If the pane is a listener, attach it.
	if (listener != nil) {
		this->AddListener(listener);
	}
	
	// Check if any of the current pane's attachments are LListeners too, and attach those.
	TArray<LAttachment*>* attachments = pane->GetAttachmentsList();
	if(attachments != nil) {
		TArrayIterator<LAttachment*> attachmentsIterator(*attachments);
		LAttachment	*theAttachment = nil;
		while (attachmentsIterator.Next(theAttachment)) {
			LListener* attachmentListener = dynamic_cast<LListener*>(theAttachment);
			if(attachmentListener != nil) {
				this->AddListener(attachmentListener);
			}
		}
	}
	
	
	// Check if this is a view, and if so recursively traverse its subpanes.
	LView* view = dynamic_cast<LView*>(pane);
	if(view != nil) {
		// Search all subpanes
		TArray<LPane*>& subPanes = view->GetSubPanes();
		TArrayIterator<LPane*> subPanesIterator(subPanes);
		LPane	*theSub = nil;
		while (subPanesIterator.Next(theSub)) {
			AttachTetrisPanesRecursively(theSub);
		}
	}
}


// LListener
void
CTetrisGameRunnerAttachment::ListenToMessage(MessageT inMessage, void *ioParam) {
	// All game commands fall between 1000 and 1999
	if(inMessage >= 1000 && inMessage < 2000) {
		HandleGameCommandMessage(inMessage, ioParam);
	}
}

// LAttachment
void
CTetrisGameRunnerAttachment::ExecuteSelf(
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
	
	// All game commands fall between 1000 and 1999
	if(inMessage >= 1000 && inMessage < 2000) {
		HandleGameCommandMessage(inMessage, ioParam);
	}
}

void	
CTetrisGameRunnerAttachment::HandleGameCommandMessage(MessageT inMessage, void *ioParam) {
	switch(inMessage) {
		// Game commands
		case 1001:
			// New game
			NewGame();
		break;
		case 1002:
			// Pause game
			PauseGame();
		break;
		case 1003:
			// Resume game
			ResumeGame();
		break;
		case 1010:
			// Game tick
			if(mTetrisGame->DoGameTick()) {
				GameStateChanged();
			}
			else {
				// Game over
				PauseGame();
			}
		break;
		case 1011:
			// Move piece left
			if(mTetrisGame->DoPieceLeft()) {
				GameStateChanged();
			}
		break;
		case 1012:
			// Move piece right
			if(mTetrisGame->DoPieceRight()) {
				GameStateChanged();
			}
		break;
		case 1013:
			// Rotate CCW
			if(mTetrisGame->DoPieceRotateCCW()) {
				GameStateChanged();
			}
		break;
		case 1014:
			// Rotate CW
			if(mTetrisGame->DoPieceRotateCW()) {
				GameStateChanged();
			}
		break;
		case 1015:
			// Soft Drop
			if(mTetrisGame->DoPieceSoftDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
		case 1016:
			// Hard Drop
			if(mTetrisGame->DoPieceHardDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
		case 1017:
			// Hold piece
			if(mTetrisGame->DoPieceHold()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
	}
}
	
void
CTetrisGameRunnerAttachment::NewGame() {
	if(!mInitialized) {
		Throw_(-1);
	}

	if(mTetrisGame != nil) {
		delete mTetrisGame;
	}

	mTetrisGame = new CTetrisGame();
	mTimeElapsedOnTick = 0;
	mGameActive = TRUE;
	StartRepeating();
	
	GameStateChanged();
}

void
CTetrisGameRunnerAttachment::PauseGame() {
	if(!mInitialized || mTetrisGame == NULL) {
		return;
	}
	
	StopRepeating();
	mGameActive = FALSE;
	mLastUpdateTime = 0;
	
	GameStateChanged();
}

void
CTetrisGameRunnerAttachment::ResumeGame() {
	if(!mInitialized || mTetrisGame == NULL) {
		return;
	}
	
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
	
	if(!mGameActive || !mInitialized || mTetrisGame == NULL) {
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
		SInt32 tickDelay = mTetrisGame->GetCurrentTickDelay();
	
		if(tickDelay < 0) {
			// Infinite tick
			// Reset elapsed so that if the tick delay changes,
			// we don't end up doing many game ticks in a row.
			mTimeElapsedOnTick = 0;
			break;
		}
		else if(mTimeElapsedOnTick >= tickDelay) {
			mTimeElapsedOnTick -= tickDelay;
		
			// Game tick
			BroadcastMessage(1010, NULL);
		}
		else {
			break;
		}
	}
}

void
CTetrisGameRunnerAttachment::GameStateChanged() {
	if(mTetrisGame != NULL) {
		BroadcastMessage(2000, mTetrisGame);
	}
}

// TODO: Move this into a Keypress handler attachment that is separate from this attachment.
Boolean	
CTetrisGameRunnerAttachment::HandleKeyPress( const EventRecord& inKeyEvent ) {
	// what: Should be a keyDown event, or a key repeat event (autoKey)
	if(inKeyEvent.what != keyDown && inKeyEvent.what != autoKey) {
		return FALSE;
	}
	
	if(!mGameActive || !mInitialized || mTetrisGame == NULL) {
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
			BroadcastMessage(1016, NULL);
			return TRUE;
		case 'a':
		case 'A':
			// Piece left
			BroadcastMessage(1011, NULL);
			return TRUE;
		case 's':
		case 'S':
			// Soft drop
			BroadcastMessage(1015, NULL);
			return TRUE;
		case 'd':
		case 'D':
			// Piece right
			BroadcastMessage(1012, NULL);
			return TRUE;
		case 'q':
		case 'Q':
			// Piece rotate CCW
			BroadcastMessage(1013, NULL);
			return TRUE;
		case 'e':
		case 'E':
			// Piece rotate CW
			BroadcastMessage(1014, NULL);
			return TRUE;
		case 'r':
		case 'R':
			// Hold piece
			BroadcastMessage(1017, NULL);
			return TRUE;
		case 'n':
		case 'N':
			// For testing: Do a game tick manually
			BroadcastMessage(1010, NULL);
			return TRUE;
	}
	
	return FALSE;
}
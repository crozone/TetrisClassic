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
#include "PaneHelpers.h"
#include "TetrisMessage.h"

using namespace PaneHelpers;

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
		mInitialized = true;
	}
}

// LListener
void
CTetrisGameRunnerAttachment::ListenToMessage(MessageT inMessage, void *ioParam) {
	// All game commands fall between 1000 and 1999
	if(inMessage >= 1000 && inMessage < 2000) {
		HandleGameControlMessage(inMessage, ioParam);
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
	}
	
	// Handle game control messages
	HandleGameControlMessage(inMessage, ioParam);
}

void	
CTetrisGameRunnerAttachment::HandleGameControlMessage(MessageT inMessage, void *ioParam) {
	if(!mInitialized) {
		return;
	}

	// Game state control commands
	switch(inMessage) {
		// Game commands
		case msg_TetrisSetupGame:
			// Setup a new game with the given ruleset
			CTetrisGameRuleset* rulesetPtr = static_cast<CTetrisGameRuleset*>(ioParam);
			ThrowIfNil_(rulesetPtr);
			SetupGame(*rulesetPtr);
		break;
		case msg_TetrisStartGame:
			// Start game
			StartGame();
		break;
		case msg_TetrisPauseGame:
			// Pause game
			PauseGame();
		break;
		case msg_TetrisResumeGame:
			// Resume game
			ResumeGame();
		break;
	}
	
	if(!mGameActive || mTetrisGame == NULL) {
		return;
	}
	
	// Game input control commands
	switch(inMessage) {
		case msg_TetrisGameTick:
			// Game tick
			if(mTetrisGame->DoGameTick()) {
				GameStateChanged();
			}
			else {
				// Game over
				PauseGame();
			}
		break;
		case msg_TetrisMovePieceLeft:
			// Move piece left
			if(mTetrisGame->DoPieceLeft()) {
				GameStateChanged();
			}
		break;
		case msg_TetrisMovePieceRight:
			// Move piece right
			if(mTetrisGame->DoPieceRight()) {
				GameStateChanged();
			}
		break;
		case msg_TetrisRotatePieceCCW:
			// Rotate CCW
			if(mTetrisGame->DoPieceRotateCCW()) {
				GameStateChanged();
			}
		break;
		case msg_TetrisRotatePieceCW:
			// Rotate CW
			if(mTetrisGame->DoPieceRotateCW()) {
				GameStateChanged();
			}
		break;
		case msg_TetrisSoftDropPiece:
			// Soft Drop
			if(mTetrisGame->DoPieceSoftDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
		case msg_TetrisHardDropPiece:
			// Hard Drop
			if(mTetrisGame->DoPieceHardDrop()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
		case msg_TetrisHoldPiece:
			// Hold piece
			if(mTetrisGame->DoPieceHold()) {
				mTimeElapsedOnTick = 0;
				GameStateChanged();
			}
		break;
	}
}

void
CTetrisGameRunnerAttachment::SetupGame(CTetrisGameRuleset ruleset) {
	if(!mInitialized) {
		Throw_(-1);
	}

	if(mTetrisGame != nil) {
		delete mTetrisGame;
	}

	mTetrisGame = new CTetrisGame(ruleset);
	mTimeElapsedOnTick = 0;
	mGameActive = FALSE;
	
	GameStateChanged();
}
	
void
CTetrisGameRunnerAttachment::StartGame() {
	if(!mInitialized) {
		Throw_(-1);
	}

	if(mTetrisGame == NULL) {
		return;
	}
	
	mTetrisGame->ResetState();

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
			BroadcastMessage(msg_TetrisGameTick, NULL);
		}
		else {
			break;
		}
	}
}

void
CTetrisGameRunnerAttachment::GameStateChanged() {
	if(mTetrisGame != NULL) {
		BroadcastMessage(msg_TetrisGameStateChanged, mTetrisGame);
	}
}
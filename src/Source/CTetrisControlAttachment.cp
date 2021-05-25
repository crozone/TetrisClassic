//
// The purpose of the CTetrisControlAttachment is to turn a pane into a
// text UI element for showing game status.
//
// The attached pane can select what role it wants with the User Constant field.
// 1: The pane will act as a "Game Over" message.
//     It will be hidden during play, and shown after the game has ended.
// 2: The pane will act as a score display.
// 3: The pane will show the lines cleared count.
// 4: The pane will show the current game level.
//
// The attachment listens for messages broadcast from the CTetrisGameRunnerAttachment.
// It is usually attached recursively when AttachTetrisPanesRecursively is called in CTetrisGameRunnerAttachment.
//

#include "CTetrisControlAttachment.h"


CTetrisControlAttachment::CTetrisControlAttachment()
: LAttachment(msg_KeyPress), LListener()
{
}


CTetrisControlAttachment::CTetrisControlAttachment(LStream*	inStream)
	: LAttachment(inStream), LListener()
{
	mMessage = msg_AnyMessage;
}

// LListener
void
CTetrisControlAttachment::ListenToMessage(MessageT inMessage, void *ioParam) {
	if(inMessage == 2000)
	{
		// ioParam is a TetrisGame
		CTetrisGame* game = static_cast<CTetrisGame*>(ioParam);
		ThrowIfNil_(game);
		
		// The attachable is what this attachment is currently connected to
		LAttachable* attachable = this->GetOwnerHost();
	
		// The attachable is usually a Pane
		LPane* pane = dynamic_cast<LPane*>(attachable);
	
		if(pane == nil) {
			// If we don't have an LPane host, there's nothing to do here.
			return;
		}
		
		// TODO: Make this a proper enum
		// The kind is what kind of message to display in the parent pane
		SInt32 kind = pane->GetUserCon();
		if(kind == 1) {
			// Game over
			if(game->IsGameOver()) {
				pane->SetVisible(TRUE);
			}
			else {
				pane->SetVisible(FALSE);
			}
		}
		else if(kind == 2) {
			// Score
			SInt32 score = game->GetState()->mScore;
			LStr255 scoreString("Score: ");
			// Append as a character
			scoreString.Append(static_cast<SInt32>(score));
			pane->SetDescriptor(scoreString);
		}
		else if(kind == 3) {
			// Lines
			SInt32 lines = game->GetState()->mLinesCleared;
			LStr255 linesString("Lines: ");
			// Append as a character
			linesString.Append(static_cast<SInt32>(lines));
			pane->SetDescriptor(linesString);
		}
		else if(kind == 4) {
			// Level
			SInt32 level = game->GetState()->mLevel;
			LStr255 levelString("Level: ");
			// Append as a character
			levelString.Append(static_cast<SInt32>(level));
			pane->SetDescriptor(levelString);
		}
	}
}

// LAttachment
void
CTetrisControlAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	switch(inMessage) {
		case msg_FinishCreate:
			// The pane hierarchy has been created, we can initialize 
		break;
	}
}
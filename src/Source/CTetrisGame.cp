// Tetris Game Implementation
//
// This class handles all Tetris game state and logic

#include "CTetrisGame.h"

CTetrisGame::CTetrisGame() {

}

CTetrisGame::~CTetrisGame() {

}
	
void
CTetrisGame::InitializeGame(UInt32 startLevel) {
	// Setup new state
	CTetrisGameState newState;
	newState.mLevel = startLevel;
	//newState.
	
}
	
// TODO: Maybe make this return Boolean?
void
CTetrisGame::DoPieceLeft() {
	// Move the piece left if it won't collide with anything
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition - 1,
		mState.mCurrentPieceYPosition,
		mState.mBoardState)) {
		
		mState.mCurrentPieceXPosition--;
	}
}
				
void
CTetrisGame::DoPieceRight() {
	// Move the piece right if it won't collide with anything
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition + 1,
		mState.mCurrentPieceYPosition,
		mState.mBoardState)) {
		
		mState.mCurrentPieceXPosition++;
	}
}
				
void
CTetrisGame::DoPieceSoftDrop() {
	// Move the piece down if it won't collide with anything
	if(!DoDropCheck()) {
		mState.mCurrentPieceYPosition--;
		
		// TODO: Add to soft-drop counter for scoring purposes.
		//       Many game rulesets reward points per soft-drop line.
		
		// softDropThisTurn++;
	}
}
				
void
CTetrisGame::DoPieceFastDrop() {
	// Move the piece down until it collides with something.
	
	while(!DoDropCheck()) {
		mState.mCurrentPieceYPosition--;
		
		// TODO: Add to hard-drop counter for scoring purposes.
		//       Many game rulesets reward points per hard-drop line
			
		// hardDropThisTurnn++;
	}
}


// Switches out the piece in play for the hold piece.
// Returns TRUE if successful, FALSE if hold was already used this turn.		
Boolean
CTetrisGame::DoPieceHold() {
	if(!mState.mHoldPieceTriggeredThisTurn) {
		mState.mHoldPieceTriggeredThisTurn = TRUE;
		
		PieceKind::Type currentHoldPiece = mState.mCurrentHoldPiece;
		mState.mCurrentHoldPiece = mState.mCurrentPiece;
		
		PieceKind::Type nextPiece;
		if(currentHoldPiece == PieceKind::None) {
			// Hold was empty, so new piece is from the top of the piece queue
			// TODO: Implement DequeueNextPiece() and fix this.
			
			// nextPiece = DequeueNextPiece();
			nextPiece = PieceKind::O;
		}
		else {
			// Hold was not empty, so new piece is the existing hold piece
			nextPiece = currentHoldPiece;
		}
		
		StartNewTurn(nextPiece);
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Returns TRUE if successful, FALSE if piece was on top of existing blocks.
// FALSE indicates GAME OVER.
Boolean
CTetrisGame::StartNewTurn(PieceKind::Type pieceKind) {
	mState.mCurrentPiece = pieceKind;
	mState.mCurrentPieceOrientation = PieceOrientation::Down;
	mState.mCurrentPieceXPosition = 3;
	mState.mCurrentPieceYPosition = 20;
	
	// TODO: Check if piece is colliding with current piece. If so, return FALSE
	return TRUE;
}

//
// Checks if a piece has a collision immediately under it and if so,
// stamps the piece to the board.
//
Boolean
CTetrisGame::DoDropCheck() {
	// Check if piece has collision immediately underneath it
	if(TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition - 1,
		mState.mBoardState)) {
		
		// Piece has collided
		
		// Stamp the piece onto the board
		TetrisPieces::StampPieceOntoBoard(
			mState.mCurrentPiece,
			mState.mCurrentPieceOrientation,
			mState.mCurrentPieceXPosition,
			mState.mCurrentPieceYPosition,
			TRUE,
			FALSE,
			mState.mBoardState);
		
		
		// TODO: Trigger scoring for placing a piece down
		// (Maybe do this externally to this function,
		// based upon the return value, for more control?)
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}
				
void
CTetrisGame::DoGameTick() {
	// TODO: Implement animation frames.
	// TODO: Disable the piece drop during animation frames.
	
	// Gravity
	if(!DoDropCheck()) {
		mState.mCurrentPieceYPosition--;
	}
	
	
}
				
UInt32
CTetrisGame::GetCurrentTickDelay() {
	// TODO: Make this change based on current level and animation frames
	
	
	
	return 1000;
}

void
CTetrisGame::RenderBoard(BlockKind::Type boardBuffer[20][10]) {
	// Copy current board state to buffer
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			boardBuffer[j][i] = mState.mBoardState[j][i];
		}
	}
		
	// Draw ghost piece onto the game board.
	
	// To do this, we move the piece downwards until it collides with something.
	// Then we draw the ghost piece in the last known non-colliding position.
	for(int i = mState.mCurrentPieceYPosition - 1; i >= 0; i--) {
		// Check for a collision below the current row
		if(TetrisPieces::CheckCollisionWithBoard(
			mState.mCurrentPiece,
			mState.mCurrentPieceOrientation,
			mState.mCurrentPieceXPosition,
			i - 1,
			boardBuffer)) {
			
			// Stamp ghost version of piece onto the current row
			TetrisPieces::StampPieceOntoBoard(
				mState.mCurrentPiece,
				mState.mCurrentPieceOrientation,
				mState.mCurrentPieceXPosition,
				i,
				FALSE,
				TRUE,
				boardBuffer);
		}
	}
	
	// Copy Tetris piece over the top of the current game board
	TetrisPieces::StampPieceOntoBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		TRUE,
		FALSE,
		boardBuffer);
}

CTetrisGameState*	
CTetrisGame::GetState() {
	return &mState;
}
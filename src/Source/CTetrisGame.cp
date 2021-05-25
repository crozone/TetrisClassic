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
	newState.mGameOver = FALSE;
	
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
CTetrisGame::DoPieceRotateCW() {
	// Rotate the piece clockwise if it won't collide with anything
	
	PieceOrientation::Type orientation = PieceOrientation::RotateCW(mState.mCurrentPieceOrientation);
	
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		orientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		mState.mBoardState)) {
		
		mState.mCurrentPieceOrientation = orientation;
	}
}
				
void
CTetrisGame::DoPieceRotateCCW() {
	// Rotate the piece counter-clockwise if it won't collide with anything
	
	PieceOrientation::Type orientation = PieceOrientation::RotateCCW(mState.mCurrentPieceOrientation);
	
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		orientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		mState.mBoardState)) {
		
		mState.mCurrentPieceOrientation = orientation;
	}
}
				
void
CTetrisGame::DoPieceSoftDrop() {
	if(!DoDrop()) {
		
		// TODO: Add to soft-drop counter for scoring purposes.
		//       Many game rulesets reward points per soft-drop line.
		
		// softDropThisTurnn++;
	}
}
				
void
CTetrisGame::DoPieceHardDrop() {
	// Move the piece down until it collides with something.
	
	while(!DoDrop()) {	
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

PieceKind::Type
CTetrisGame::GetNextPiece() {
	// If the bag is empty, fill it.
	if(mState.mCurrentPieceBagCount == 0) {
		UpdatePieceBag();
		
		if(mState.mCurrentPieceBagCount == 0) {
			// Sanity check
			Throw_(-1);
		}
	}
	
	// Get the top piece from the piece bag
	PieceKind::Type nextPiece = mState.mPieceBag[0];
	
	if(nextPiece == PieceKind::None) {
		// Sanity check
		Throw_(-1);
	}
	
	// Move all pieces down by 1
	for(UInt8 i = 0; i < mState.mCurrentPieceBagCount - 1; i++) {
		mState.mPieceBag[i] = mState.mPieceBag[i + 1];
	}
	
	// It is technically not necessary to clear the last index, 
	// since no piece beyond the bag count should ever be accessed,
	// but this adds a sanity check during debugging.
	mState.mPieceBag[mState.mCurrentPieceBagCount - 1] = PieceKind::None;
	
	mState.mCurrentPieceBagCount--;
	
	return nextPiece;
}

void
CTetrisGame::UpdatePieceBag() {

	// While the piece bag has space for another piece set,
	// generate random piece sets into the piece bag.
	
	while((CTetrisGameState::PieceBagBufferCount - mState.mCurrentPieceBagCount)
		>= PieceGenerationStrategy::GetPieceSetSize(mState.mPieceGenerationStrategy)) {
		
		UInt8 piecesAdded = PieceGenerationStrategy::GeneratePieceSet(
			mState.mPieceGenerationStrategy,
			&mState.mPieceBag[mState.mCurrentPieceBagCount]
			);
			
		mState.mCurrentPieceBagCount += piecesAdded;
	}
}


void
CTetrisGame::StartNewTurn(PieceKind::Type pieceKind) {
	// Set piece to initial position
	mState.mCurrentPiece = pieceKind;
	mState.mCurrentPieceOrientation = PieceOrientation::Down;
	mState.mCurrentPieceXPosition = 3;
	mState.mCurrentPieceYPosition = 20;
}

// Starts a new turn with the next piece from the piece queue
void
CTetrisGame::StartNextTurn() {
	// Get the next piece from the piece queue
	
	PieceKind::Type pieceKind = GetNextPiece();
	
	StartNewTurn(pieceKind);
}

//
// Checks if a piece has a collision immediately under it and if so,
// stamps the piece to the board.
//
// Returns TRUE if a collision occured, FALSE if no collision occured.
//
Boolean
CTetrisGame::DoDrop() {
	// Check if piece is already colliding with the board.
	// This indicates GAME OVER.
	
	if(TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
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
		
		// Set game over
		mState.mGameOver = TRUE;
		
		// Start the next turn.
		// (Not strictly necessary, but the GameBoy version does this
		// to show yet another piece over the top of the current piece)
		
		StartNextTurn();	
		
		return TRUE;
	}
	// Check if piece has collision immediately underneath it
	else if(TetrisPieces::CheckCollisionWithBoard(
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
		
		// Start the next turn.
		StartNextTurn();
		
		return TRUE;
	}
	else {
		// Move piece down by one row
		mState.mCurrentPieceYPosition--;
	
		return FALSE;
	}
}


// Performs a game tick.
//
// Returns TRUE if the game is still running, FALSE if GAME OVER.				
Boolean
CTetrisGame::DoGameTick() {
	// TODO: Implement animation frames.
	// TODO: Disable the piece drop during animation frames.
	
	if(mState.mGameOver) {
		return FALSE;
	}
	
	DoDrop();
	
	return TRUE;
}
				
UInt32
CTetrisGame::GetCurrentTickDelay() {
	// TODO: Make this change based on current level and animation frames
	
	
	
	return 1000;
}

void
CTetrisGame::RenderBoard(BlockKind::Type blockBuffer[20][10]) {
	// Copy current board state to buffer
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			blockBuffer[j][i] = mState.mBoardState[j][i];
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
			blockBuffer)) {
			
			// Stamp ghost version of piece onto the current row
			TetrisPieces::StampPieceOntoBoard(
				mState.mCurrentPiece,
				mState.mCurrentPieceOrientation,
				mState.mCurrentPieceXPosition,
				i,
				FALSE,
				TRUE,
				blockBuffer);
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
		blockBuffer);
}

CTetrisGameState*	
CTetrisGame::GetState() {
	return &mState;
}
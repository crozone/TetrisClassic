// Tetris Game Implementation
//
// This class handles all Tetris game state and logic

#include "CTetrisGame.h"

CTetrisGame::CTetrisGame() :
	mState()
{
	// Setup state
	mState.mLevel = 0;
	mState.mGameOver = FALSE;
	StartNextTurn();
};

CTetrisGame::CTetrisGame(UInt32 startLevel) :
	mState()
{
	// Setup state
	mState.mLevel = startLevel;
	mState.mGameOver = FALSE;
	StartNextTurn();
}

CTetrisGame::~CTetrisGame() {
}

Boolean
CTetrisGame::IsAcceptingInput() {
	return (!mState.mGameOver && !mState.mQueueNewTurn);
}

Boolean
CTetrisGame::DoPieceLeft() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	// Move the piece left if it won't collide with anything
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition - 1,
		mState.mCurrentPieceYPosition,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		mState.mCurrentPieceXPosition--;
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}
				
Boolean
CTetrisGame::DoPieceRight() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	// Move the piece right if it won't collide with anything
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition + 1,
		mState.mCurrentPieceYPosition,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		mState.mCurrentPieceXPosition++;
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Boolean
CTetrisGame::DoPieceRotateCW() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}
	
	// Rotate the piece clockwise if it won't collide with anything
	
	PieceOrientation::Type orientation = PieceOrientation::RotateCW(mState.mCurrentPieceOrientation);
	
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		orientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		mState.mCurrentPieceOrientation = orientation;
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}
				
Boolean
CTetrisGame::DoPieceRotateCCW() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	// Rotate the piece counter-clockwise if it won't collide with anything
	
	PieceOrientation::Type orientation = PieceOrientation::RotateCCW(mState.mCurrentPieceOrientation);
	
	if(!TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		orientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		mState.mCurrentPieceOrientation = orientation;
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Move the piece down one				
Boolean
CTetrisGame::DoPieceSoftDrop() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	if(!DoDrop()) {
		
		// TODO: Add to soft-drop counter for scoring purposes.
		//       Many game rulesets reward points per soft-drop line.
		
		// softDropThisTurn++;
	}
	
	return TRUE;
}

// Drop the piece until it hits the board
Boolean
CTetrisGame::DoPieceHardDrop() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	// Move the piece down until it collides with something.
	
	while(!DoDrop() && mState.mCurrentPieceYPosition >= 0) {	
		// TODO: Add to hard-drop counter for scoring purposes.
		//       Many game rulesets reward points per hard-drop line
			
		// hardDropThisTurnn++;
	}
	
	return TRUE;
}


// Switches out the piece in play for the hold piece.
// Returns TRUE if successful, FALSE if hold was already used this turn.		
Boolean
CTetrisGame::DoPieceHold() {
	if(!IsAcceptingInput()) {
		return FALSE;
	}

	if(!mState.mHoldPieceTriggeredThisTurn) {
		mState.mHoldPieceTriggeredThisTurn = TRUE;
		
		PieceKind::Type currentHoldPiece = mState.mCurrentHoldPiece;
		mState.mCurrentHoldPiece = mState.mCurrentPiece;
		
		if(currentHoldPiece == PieceKind::None) {
			// Hold was empty, so new piece is from the top of the piece queue
			// TODO: Implement DequeueNextPiece() and fix this.
			
			PieceKind::Type nextPiece = GetNextPiece();
			StartNewTurn(nextPiece, FALSE);
		}
		else {
			// Hold was not empty, so new piece is the existing hold piece
			PieceKind::Type holdPiece = currentHoldPiece;
			StartNewTurn(holdPiece, FALSE);
		}
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Boolean
CTetrisGame::IsGameOver() {
	return mState.mGameOver;
}


// Get the next piece from the piece bag,
// and regenerate the piece bag if necessary.
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
CTetrisGame::StartNewTurn(PieceKind::Type pieceKind, Boolean resetHold) {
	// Set piece to initial position
	mState.mCurrentPiece = pieceKind;
	mState.mCurrentPieceOrientation = PieceOrientation::Down;
	mState.mCurrentPieceXPosition = 3;
	mState.mCurrentPieceYPosition = 20;
	
	if(resetHold) {
		mState.mHoldPieceTriggeredThisTurn = FALSE;
	}
}

// Starts a new turn with the next piece from the piece queue
void
CTetrisGame::StartNextTurn() {
	// Get the next piece from the piece queue
	
	PieceKind::Type pieceKind = GetNextPiece();	
	StartNewTurn(pieceKind, TRUE);
}

//
// Checks if a piece has a collision immediately under it and if so,
// stamps the piece to the board.
//
// Returns TRUE if a collision occured, FALSE if no collision occured.
//
Boolean
CTetrisGame::DoDrop() {
	Boolean collision = DropAndStampPiece();
	UInt8 rowsCleared = DoRowClears();
	
	if(collision) {
		// TODO: Trigger scoring for placing a piece down
		
		if(rowsCleared == 0) {
			StartNextTurn();
		}
		else {
			// Clear the current piece while the rowsCleared animation triggers
			StartNewTurn(PieceKind::None, FALSE);
		
			// Queue a turn clear for after the line clearing animations are complete
			mState.mQueueNewTurn = TRUE;
		}
	}
	
	return collision;
}

Boolean
CTetrisGame::DropAndStampPiece() {
	// Check if piece is already colliding with the board.
	// This indicates GAME OVER.
	
	if(TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		// Piece has collided
		
		// Stamp the piece onto the board
		TetrisPieces::StampPieceOntoBoard(
			mState.mCurrentPiece,
			mState.mCurrentPieceOrientation,
			mState.mCurrentPieceXPosition,
			mState.mCurrentPieceYPosition,
			TRUE,
			FALSE,
			mState.mBoardState,
			mState.mBoardStateWidth,
			mState.mBoardStateHeight);
		
		// Set game over
		mState.mGameOver = TRUE;	
		
		return TRUE;
	}
	// Check if piece has collision immediately underneath it
	else if(TetrisPieces::CheckCollisionWithBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition - 1,
		mState.mBoardState,
		mState.mBoardStateWidth,
		mState.mBoardStateHeight)) {
		
		// Piece has collided
		
		// Stamp the piece onto the board
		TetrisPieces::StampPieceOntoBoard(
			mState.mCurrentPiece,
			mState.mCurrentPieceOrientation,
			mState.mCurrentPieceXPosition,
			mState.mCurrentPieceYPosition,
			TRUE,
			FALSE,
			mState.mBoardState,
			mState.mBoardStateWidth,
			mState.mBoardStateHeight);
		
		return TRUE;
	}
	else {
		// Move piece down by one row
		mState.mCurrentPieceYPosition--;
	
		return FALSE;
	}
}

UInt8
CTetrisGame::DoRowClears() {
	UInt8 rowsCleared = 0;

	for(int j = 0; j < mState.mBoardStateHeight; j++) {
		Boolean rowIsComplete = TRUE;
		for(int i = 0; i < mState.mBoardStateWidth; i++) {
			// If the entire row is collidable, it is considered cleared
			if(!TetrisPieces::IsBlockCollidable(mState.mBoardState[j * mState.mBoardStateWidth + i])) {
				rowIsComplete = FALSE;
				break;
			}
		}
		
		if(rowIsComplete) {
			// Clear the row
			for(int i = 0; i < mState.mBoardStateWidth; i++) {
				// Clear the collidable flag
				TetrisPieces::SetBlockCollidable(mState.mBoardState[j * mState.mBoardStateWidth + i], FALSE);
				
				// Set the clear flag and ticks
				TetrisPieces::SetFlaggedForClear(mState.mBoardState[j * mState.mBoardStateWidth + i], TRUE);
				
				// TODO: Play with this value depending on row/column
				// to achieve fancy clear animations
				TetrisPieces::SetClearCountdown(mState.mBoardState[j * mState.mBoardStateWidth + i], 7);
			}
			
			rowsCleared++;
		}
	}
	
	return rowsCleared;
}

Boolean
CTetrisGame::ProcessFlaggedToClearBlocks() {
	Boolean blocksProcessed = FALSE;
	
	for(int j = mState.mBoardStateHeight - 1; j >= 0; j--) {
		for(int i = 0; i < mState.mBoardStateWidth; i++) {
			// Process any blocks that are flagged for clear
			if(TetrisPieces::IsFlaggedForClear(mState.mBoardState[j * mState.mBoardStateWidth + i])) {
				blocksProcessed = TRUE;
				UInt8 clearTicksLeft = TetrisPieces::GetClearCountdown(mState.mBoardState[j * mState.mBoardStateWidth + i]);
				
				if(clearTicksLeft > 0) {
					TetrisPieces::SetClearCountdown(mState.mBoardState[j * mState.mBoardStateWidth + i], clearTicksLeft - 1);
				}
				else {
					// Clear the block. Move every block above this block down one.
					for(int k = j; k < (mState.mBoardStateHeight - 1); k++) {
						mState.mBoardState[k * mState.mBoardStateWidth + i] = mState.mBoardState[(k + 1) * mState.mBoardStateWidth + i];
					}
					mState.mBoardState[(mState.mBoardStateHeight - 1) * mState.mBoardStateWidth + i] = BlockKind::None;
				}
			}
		}
	}
	
	return blocksProcessed;
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
	
	Boolean skipDrop = ProcessFlaggedToClearBlocks();
	
	if(!skipDrop) {
		if(mState.mQueueNewTurn) {
			mState.mQueueNewTurn = FALSE;
			StartNextTurn();
		}
		else {
			DoDrop();
		}
	}
	
	return TRUE;
}
				
SInt32
CTetrisGame::GetCurrentTickDelay() {
	// TODO: Move this into an array and actually finish it
	
	if(mState.mGameOver) {
		return -1;
	}
	
	// If a new turn is queued, it means we are in a line-clear animation
	if(mState.mQueueNewTurn) {
		return 10;
	}
	
	if(mState.mLevel < 0) {
		// Infinite timeout
		return -1;
	}
	else if(mState.mLevel == 0) {
		return 53 * 100 / 60;
	}
	else if(mState.mLevel == 1) {
		return 49 * 100 / 60;
	}
	else if(mState.mLevel == 2) {
		return 45 * 100 / 60;
	}
	else if(mState.mLevel == 3) {
		return 41 * 100 / 60;
	}
	else if(mState.mLevel == 4) {
		return 37 * 100 / 60;
	}
	else if(mState.mLevel == 5) {
		return 33 * 100 / 60;
	}
	else if(mState.mLevel == 6) {
		return 28 * 100 / 60;
	}
	else if(mState.mLevel == 7) {
		
	}
	else if(mState.mLevel == 8) {
	
	}
	else if(mState.mLevel == 9) {
	
	}
	else {
	
	}
	
	
	return 100;
}

UInt8
CTetrisGame::GetBoardWidth() {
	return mState.mBoardStateWidth;
}

UInt8
CTetrisGame::GetBoardHeight() {
	return mState.mBoardStateHeight;
}

void
CTetrisGame::RenderBoard(BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight) {
	// Sanity check that the incoming buffer is large enough to render the board
	if(bufferWidth < mState.mBoardStateWidth || bufferHeight < mState.mBoardStateHeight) {
		Throw_(-1);
	}
	
	// Copy current board state to buffer
	for(int j = 0; j < mState.mBoardStateHeight; j++) {
		for(int i = 0; i < mState.mBoardStateWidth; i++) {
			blockBuffer[j * bufferWidth + i] = mState.mBoardState[j * mState.mBoardStateWidth + i];
		}
	}
		
	// Draw ghost piece onto the game board.
	
	// To do this, we move the piece downwards until it collides with something.
	// Then we draw the ghost piece in the last known non-colliding position.
	for(int i = mState.mCurrentPieceYPosition; i >= 0; i--) {
		// Check for a collision below the current row
		if(TetrisPieces::CheckCollisionWithBoard(
			mState.mCurrentPiece,
			mState.mCurrentPieceOrientation,
			mState.mCurrentPieceXPosition,
			i - 1,
			mState.mBoardState,
			mState.mBoardStateWidth,
			mState.mBoardStateHeight)) {
			
			// Stamp ghost version of piece onto the current row
			TetrisPieces::StampPieceOntoBoard(
				mState.mCurrentPiece,
				mState.mCurrentPieceOrientation,
				mState.mCurrentPieceXPosition,
				i,
				FALSE,
				TRUE,
				blockBuffer,
				bufferWidth,
				bufferHeight);
				
			// Break out of loop (we only want one ghost piece drawn)
			break;
		}
	}
	
	// Stamp the falling Tetris piece over the top of the current game board
	TetrisPieces::StampPieceOntoBoard(
		mState.mCurrentPiece,
		mState.mCurrentPieceOrientation,
		mState.mCurrentPieceXPosition,
		mState.mCurrentPieceYPosition,
		TRUE,
		FALSE,
		blockBuffer,
		bufferWidth,
		bufferHeight);
}

void
CTetrisGame::RenderHoldPiece(BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight) {
	if(bufferWidth < 4 || bufferHeight < 4) {
		Throw_(-1);
	}
	
	TetrisPieces::StampPieceOntoBoard(
		mState.mCurrentHoldPiece,
		PieceOrientation::Down,
		0,
		0,
		TRUE,
		FALSE,
		blockBuffer,
		bufferWidth,
		bufferHeight);
}

void
CTetrisGame::RenderBagPiece(UInt8 index, BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight) {
	if(bufferWidth < mState.mBoardStateWidth || bufferHeight < mState.mBoardStateHeight) {
		Throw_(-1);
	}
	
	PieceKind::Type piece = PieceKind::None;
	
	if(index < mState.mCurrentPieceBagCount) {
		piece = mState.mPieceBag[index];
	}
	
	TetrisPieces::StampPieceOntoBoard(
		piece,
		PieceOrientation::Down,
		0,
		0,
		TRUE,
		FALSE,
		blockBuffer,
		bufferWidth,
		bufferHeight);
}

CTetrisGameState*	
CTetrisGame::GetState() {
	return &mState;
}
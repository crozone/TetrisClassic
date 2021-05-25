// Tetris Game State Implementation

#include "CTetrisGameState.h"

CTetrisGameState::CTetrisGameState() :
	mScore(0),
	mLevel(0),
	mLinesCleared(0),
	mGameOver(FALSE),
	mQueueNewTurn(FALSE),
	mCurrentPiece(PieceKind::None),
	mCurrentPieceOrientation(PieceOrientation::Down),
	mCurrentPieceXPosition(0),
	mCurrentPieceYPosition(0),
	mCurrentHoldPiece(PieceKind::None),
	mHoldPieceTriggeredThisTurn(FALSE),
	mPieceGenerationStrategy(PieceGenerationStrategy::ClassicRandom),
	mCurrentPieceBagCount(0),
	mBoardStateWidth(0),
	mBoardStateHeight(0),
	mBoardState(NULL)
	
{
	// TODO: Different sized Tetris boards based on ruleset?
	InitializeBoardBuffer(10, 20);
	
	for(int i = 0; i < PieceBagBufferCount; i++) {
		mPieceBag[i] = PieceKind::None;
	}
}

void
CTetrisGameState::InitializeBoardBuffer(UInt8 width, UInt8 height) {
	mBoardStateWidth = width;
	mBoardStateHeight = height;
	
	mBoardState = new BlockKind::Type[width * height];
	ThrowIfNil_(mBoardState);
	
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			mBoardState[j * width + i] = BlockKind::None;
		}
	}
}

CTetrisGameState::~CTetrisGameState() {
	delete[] mBoardState;
}

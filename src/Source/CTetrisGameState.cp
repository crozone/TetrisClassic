// Tetris Game State Implementation

#include "CTetrisGameState.h"

CTetrisGameState::CTetrisGameState() :
	mScore(0),
	mLevel(0),
	mLinesCleared(0),
	mGameOver(FALSE),
	mCurrentPiece(PieceKind::None),
	mCurrentPieceOrientation(PieceOrientation::Down),
	mCurrentPieceXPosition(0),
	mCurrentPieceYPosition(0),
	mCurrentHoldPiece(PieceKind::None),
	mHoldPieceTriggeredThisTurn(FALSE),
	mPieceGenerationStrategy(PieceGenerationStrategy::ClassicRandom),
	mCurrentPieceBagCount(0)
	
{
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			mBoardState[j][i] = BlockKind::None;
		}
	}
	for(int i = 0; i < PieceBagBufferCount; i++) {
		mPieceBag[i] = PieceKind::None;
	}
}

CTetrisGameState::~CTetrisGameState() {

}

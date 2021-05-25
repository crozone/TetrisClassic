#ifndef _H_CTetrisGameState
#define _H_CTetrisGameState
#pragma once

#include "TetrisPieces.h"

struct PieceGenerationStrategy {
	enum Type {
		ClassicRandom = 0,
		ModernPieceBag = 1
	};
};

//
// TetrisGameState contains all state for a Tetris game.
// Includes game board, piece position, piece bag, score, etc.
//
class CTetrisGameState
{

public:

							CTetrisGameState();

							~CTetrisGameState();
							
				UInt32		mScore;
				UInt32		mLevel;
				UInt32		mLinesCleared;
				UInt32		mGameTime;
	
				PieceKind::Type	mCurrentPiece;
				PieceOrientation::Type		mCurrentPieceOrientation;

				SInt8		mCurrentPieceXPosition;
				SInt8		mCurrentPieceYPosition;
				
				PieceKind::Type mCurrentHoldPiece;
				Boolean mHoldPieceTriggeredThisTurn;

				BlockKind::Type	mBoardState[20][10];
	
				PieceKind::Type	mNextPieces[7];
				PieceGenerationStrategy::Type	mPieceGenerationStrategy;
protected:

};

#endif // _H_CTetrisGameState
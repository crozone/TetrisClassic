#ifndef _H_CTetrisGameState
#define _H_CTetrisGameState
#pragma once

#include "TetrisPieces.h"
#include "PieceGenerationStrategy.h"

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
				Boolean		mGameOver;
	
				PieceKind::Type	mCurrentPiece;
				PieceOrientation::Type		mCurrentPieceOrientation;

				SInt8		mCurrentPieceXPosition;
				SInt8		mCurrentPieceYPosition;
				
				PieceKind::Type mCurrentHoldPiece;
				Boolean mHoldPieceTriggeredThisTurn;

				BlockKind::Type	mBoardState[20][10];
				
				PieceGenerationStrategy::Type	mPieceGenerationStrategy;
				
				// The minimum number of pieces that should always be available
				// in the piece bag
				static	const	UInt8	MinimumPiecesCount	=	7;
				
				// The total number of pieces in the buffer.
				// This is the min pieces count plus the max the generation set size,
				// so that another set of pieces can always be generated before the
				// number of pieces ever drops below the min pieces count.
				static	const	UInt8	PieceBagBufferCount	=	MinimumPiecesCount + PieceGenerationStrategy::MaxSetSize;
				
				// The number of pieces currently in the piece bag buffer
				UInt8 mCurrentPieceBagCount;
				
				// The piece bag buffer
				PieceKind::Type	mPieceBag[PieceBagBufferCount];
				
protected:

};

#endif // _H_CTetrisGameState
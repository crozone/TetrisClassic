#ifndef _H_CTetrisGameRuleset
#define _H_CTetrisGameRuleset
#pragma once

#include "TetrisPieces.h"
#include "TetrisScoringSystem.h"
#include "PieceGenerationStrategy.h"
#include "LineClearMode.h"

//
// TetrisGameState contains all state for a Tetris game.
// Includes game board, piece position, piece bag, score, etc.
//
class CTetrisGameRuleset
{

public:
							CTetrisGameRuleset();

							~CTetrisGameRuleset();
				
				TetrisScoringSystem::Type	mScoringSystem;
				
							// The piece generation strategy to use for this game
				PieceGenerationStrategy::Type	mPieceGenerationStrategy;
				
							// The level to start the game on
				UInt32		mStartingLevel;
				
							// The number of lines to clear before advancing a level
							// <= 0 is Infinite
				UInt32		mLinesPerLevel;
							
							// If true, hold piece is allowed
				Boolean		mEnableHoldPiece;
				
							// Number of pieces in the piecebag that the user can view
				UInt32		mPieceBagLookahead;
				
							// The board width (normal game rules = 10)
				UInt8		mBoardWidth;
							// The board height (normal game rules = 20)
				UInt8		mBoardHeight;
				
				LineClearMode::Type	mLineClearMode;
				
				// Developer oriented rules
				Boolean		mDrawRenderPaneBoundingBoxes;
				
protected:

};

#endif // _H_CTetrisGameRuleset
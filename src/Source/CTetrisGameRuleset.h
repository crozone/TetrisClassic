#ifndef _H_CTetrisGameRuleset
#define _H_CTetrisGameRuleset
#pragma once

#include "TetrisGameMode.h"
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
				
				//
				// Standard game rules
				//
				
				TetrisGameMode::Type	mGameMode;
				
							// The scoring system used for calculating score
				TetrisScoringSystem::Type	mScoringSystem;
				
							// The piece generation strategy to used to generate piece bags
				PieceGenerationStrategy::Type	mPieceGenerationStrategy;
				
							// The level to start the game on
				UInt32		mStartingLevel;
				
							// If true, hold piece is allowed
				Boolean		mEnableHoldPiece;
				
							// Number of pieces in the piecebag that the user can view
				UInt32		mPieceBagLookahead;
				
				//
				// Advanced game rules
				//
				
							// The number of lines to clear before advancing a level
							// <= 0 is Infinite
				UInt32		mLinesPerLevel;
				
							// The board width (normal game rules = 10)
				UInt8		mBoardWidth;
							// The board height (normal game rules = 20)
				UInt8		mBoardHeight;
				
				
				// TODO:
				// In Tetris, the game board is actually 40 blocks high, but only the first
				// 20 blocks are ever rendered (with maybe a little sliver of the 21st block above)
				//
				// Investigating the rationale for this design decision.
				// Then, change mBoardHeight to a default of 40, and enable mVisibleBoardHeight
				// with a default of 20.
				// Adjust the renderer to account for this.
				// The game logic should already work with any arbitrary mBoardHeight.
				
				// UInt8		mVisibleBoardHeight;
				
				
				//
				// Cosmetic rules
				//
							// The type of animation to use when clearing lines
				LineClearMode::Type	mLineClearMode;
				
				// TODO: Game piece colors
				
				//
				// Developer
				//
							// Draw bounding boxes to show the edges of the render panes
				Boolean		mDrawRenderPaneBoundingBoxes;
				
protected:

};

#endif // _H_CTetrisGameRuleset
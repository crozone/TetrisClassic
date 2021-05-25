// Tetris Game Ruleset Implementation

#include "CTetrisGameRuleset.h"

CTetrisGameRuleset::CTetrisGameRuleset() :
	mScoringSystem(TetrisScoringSystem::OriginalNintendo),
	mPieceGenerationStrategy(PieceGenerationStrategy::ClassicRandom),
	mStartingLevel(0),
	mEnableHoldPiece(TRUE),
	mPieceBagLookahead(7),
	mBoardWidth(10),
	mBoardHeight(20),
	mLineClearMode(LineClearMode::FourTick),
	mDrawRenderPaneBoundingBoxes(FALSE)
{

}

CTetrisGameRuleset::~CTetrisGameRuleset() {
}
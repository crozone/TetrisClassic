#ifndef _H_CTetrisGame
#define _H_CTetrisGame
#pragma once

#include "BlockKind.h"
#include "CTetrisGameState.h"
#include "CTetrisGameRuleset.h"

//
// TetrisGame handles all Tetris game logic and state
//
class CTetrisGame
{

public:
							CTetrisGame();

							CTetrisGame(CTetrisGameRuleset ruleset);

							~CTetrisGame();
	
				Boolean		DoPieceLeft();
				
				Boolean		DoPieceRight();
				
				Boolean		DoPieceRotateCW();
				
				Boolean		DoPieceRotateCCW();
				
				Boolean		DoPieceSoftDrop();
				
				Boolean		DoPieceHardDrop();
				
				Boolean		DoPieceHold();
				
				Boolean		DoGameTick();
				
				Boolean		IsGameOver();
				
				Boolean		IsInAnimation();
				
				SInt32		GetCurrentTickDelay();
				
				UInt8		GetBoardWidth();
				
				UInt8		GetBoardHeight();
				
				void		RenderBoard(BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight);
				
				void		RenderHoldPiece(BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight);

				void		RenderBagPiece(UInt8 index, BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight);
				
				void		ResetState();
				
				CTetrisGameState*	GetState();
				
				CTetrisGameRuleset*	GetRuleset();

protected:
				void		RenderSingleDisplayPiece(PieceKind::Type piece, BlockKind::Type* blockBuffer, UInt8 bufferWidth, UInt8 bufferHeight);

				void		StartNewTurn(PieceKind::Type pieceKind, Boolean resetHold);
				
				void		StartNextTurn();
				
				PieceKind::Type	GetNextPiece();
				
				void		UpdatePieceBag();
				
				Boolean		DoDrop();
				
				Boolean		ProcessFlaggedToClearBlocks();
				
				Boolean		DropAndStampPiece();
				
				UInt8		DoRowClears();
				
				Boolean		IsAcceptingInput();
				
				void		ScoreRowsCleared(SInt32 linesCleared);
				
				void		ScoreHardDrop(SInt32 linesDropped);
				
				void		ScoreSoftDrop(SInt32 linesDropped);
				
				void		ScoreNaturalDrop();

				CTetrisGameState* mState;
				
				CTetrisGameRuleset* mRuleset;
};

#endif // _H_CTetrisGame
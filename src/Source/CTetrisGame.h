#ifndef _H_CTetrisGame
#define _H_CTetrisGame
#pragma once

#include "BlockKind.h"
#include "CTetrisGameState.h"

//
// TetrisGame handles all Tetris game logic and state
//
class CTetrisGame
{

public:
							CTetrisGame();

							CTetrisGame(UInt32 startLevel);

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
				
				CTetrisGameState*	GetState();

protected:
				void		StartNewTurn(PieceKind::Type pieceKind, Boolean resetHold);
				
				void		StartNextTurn();
				
				PieceKind::Type	GetNextPiece();
				
				void		UpdatePieceBag();
				
				Boolean		DoDrop();
				
				Boolean		ProcessFlaggedToClearBlocks();
				
				Boolean		DropAndStampPiece();
				
				UInt8		DoRowClears();
				
				Boolean		IsAcceptingInput();

				CTetrisGameState mState;
};

#endif // _H_CTetrisGame
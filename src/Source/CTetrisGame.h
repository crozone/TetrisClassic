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
				
				SInt32		GetCurrentTickDelay();
				
				void		RenderBoard(BlockKind::Type blockBuffer[20][10]);
				
				void		RenderHoldPiece(BlockKind::Type blockBuffer[4][4]);

				void		RenderBagPiece(UInt8 index, BlockKind::Type blockBuffer[4][4]);
				
				CTetrisGameState*	GetState();

protected:
				void		StartNewTurn(PieceKind::Type pieceKind);
				
				void		StartNextTurn();
				
				PieceKind::Type	GetNextPiece();
				
				void		UpdatePieceBag();
				
				Boolean		DoDrop();
				
				Boolean		DropAndStampPiece();
				
				void		CheckForRowClears();

				CTetrisGameState mState;
};

#endif // _H_CTetrisGame
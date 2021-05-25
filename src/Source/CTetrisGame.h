#ifndef _H_CTetrisGame
#define _H_CTetrisGame
#pragma once

#include "CTetrisGameState.h"

//
// TetrisGame handles all Tetris game logic and state
//
class CTetrisGame
{

public:

							CTetrisGame();

							~CTetrisGame();
							
				// TODO: Make all "Do" methods return Boolean to indicate if the game state changed.
				// TRUE if state was updated (and needs to be re-rendered).
				// FALSE if no change occured.
	
				void		InitializeGame(UInt32 startLevel);
	
				void		DoPieceLeft();
				
				void		DoPieceRight();
				
				void		DoPieceRotateCW();
				
				void		DoPieceRotateCCW();
				
				void		DoPieceSoftDrop();
				
				void		DoPieceHardDrop();
				
				Boolean		DoPieceHold();
				
				Boolean		DoGameTick();
				
				UInt32		GetCurrentTickDelay();
				
				void		RenderBoard(BlockKind::Type blockBuffer[20][10]);
				
// TODO:		void		RenderHoldPiece(BlockKind::Type blockBuffer[4][4]);

// TODO:		void		RenderBagPiece(UInt8 index, BlockKind::Type blockBuffer[4][4]);
				
				CTetrisGameState*	GetState();

protected:
				void		StartNewTurn(PieceKind::Type pieceKind);
				
				void		StartNextTurn();
				
				PieceKind::Type	GetNextPiece();
				
				void		UpdatePieceBag();
				
				Boolean		DoDrop();

				CTetrisGameState mState;
};

#endif // _H_CTetrisGame
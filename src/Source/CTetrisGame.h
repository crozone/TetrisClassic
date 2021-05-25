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
	
				void		InitializeGame(UInt32 startLevel);
	
				void		DoPieceLeft();
				
				void		DoPieceRight();
				
				void		DoPieceSoftDrop();
				
				void		DoPieceFastDrop();
				
				Boolean		DoPieceHold();
				
				Boolean		StartNewTurn(PieceKind::Type pieceKind);
				
				Boolean		DoDropCheck();
				
				void		DoGameTick();
				
				UInt32		GetCurrentTickDelay();
				
				void		RenderBoard(BlockKind::Type boardBuffer[20][10]);
				
				CTetrisGameState*	GetState();

protected:
				CTetrisGameState mState;
};

#endif // _H_CTetrisGame
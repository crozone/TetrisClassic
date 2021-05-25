#ifndef _H_TetrisPieces
#define _H_TetrisPieces
#pragma once

#include "PieceKind.h"
#include "BlockKind.h"
#include "PieceOrientation.h"

class TetrisPieces {

public:
	
	static	UInt8 GetPieceIndex(PieceKind::Type pieceKind);
	
	static	PieceKind::Type	GetPieceFromIndex(UInt8 pieceIndex);
	
	static	PieceKind::Type	GetPieceFromBlock(BlockKind::Type blockKind);
	
	static	UInt8	GetOrientationIndex(PieceOrientation::Type orientation);
	
	static	BlockKind::Type GetBlockFromPiece(
				PieceKind::Type pieceKind,
				Boolean collidable,
				Boolean ghost);

	static	Boolean IsBlockCollidable(BlockKind::Type blockKind);
	
	static	void	SetBlockCollidable(BlockKind::Type &blockKind, Boolean collidable);

	static	Boolean	IsBlockGhost(BlockKind::Type blockKind);
	
	static	void	SetBlockGhost(BlockKind::Type &blockKind, Boolean ghost);
	
	static	Boolean	IsFlaggedForClear(BlockKind::Type blockKind);
	
	static	void	SetFlaggedForClear(BlockKind::Type &blockKind, Boolean clear);
	
	static	UInt8	GetClearCountdown(BlockKind::Type blockKind);
	
	static	void	SetClearCountdown(BlockKind::Type &blockKind, UInt8 value);
	
	static	void	RenderPiece(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				Boolean collidable,
				Boolean ghost,
				BlockKind::Type buffer[4][4]);
			
	static	void	StampPieceOntoBoard(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				SInt8 xPosition,
				SInt8 yPosition,
				Boolean collidable,
				Boolean ghost,
				BlockKind::Type boardBuffer[20][10]);
				
	static	Boolean	CheckCollisionWithBoard(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				SInt8 xPosition,
				SInt8 yPosition,
				BlockKind::Type boardBuffer[20][10]);

protected:
	// [Piece][Rotation][y][x]
	// Each piece has 4 individual 4x4 bitmaps describing each of its rotations.
	// Rotations are counter clockwise from 0-3
	static	const	Boolean	piecesArray[8][4][4][4];

};
#endif // _H_TetrisPieces
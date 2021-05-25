#ifndef _H_TetrisPieces
#define _H_TetrisPieces
#pragma once

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.

struct PieceKind {
	enum Type {
		None			=	0,		// Transparent
		I				=	1,		// Cyan
		O				=	2,		// Yellow
		T				=	3,		// Purple
		S				=	4,		// Green
		Z				=	5,		// Red
		J				=	6,		// Blue
		L				=	7		// Orange
	};
};

struct BlockKind {
	enum Type {
		None			=	0,		// Transparent
		I				=	1,		// Cyan
		O				=	2,		// Yellow
		T				=	3,		// Purple
		S				=	4,		// Green
		Z				=	5,		// Red
		J				=	6,		// Blue
		L				=	7,		// Orange
		PieceKindMask	=	7,		// 00000111
		CollidableFlag	=	8,		// 00001000
		GhostFlag		=	16		// 00010000
	};
};

struct PieceOrientation {
	enum Type {
		Down = 0,
		Right = 1,
		Up = 2,
		Left = 3
	};
};

class TetrisPieces {

public:
	
	static	UInt8 GetPieceIndex(PieceKind::Type pieceKind);
	
	static	PieceKind::Type	GetPieceFromBlock(BlockKind::Type blockKind);
	
	static	BlockKind::Type GetBlockFromPiece(
				PieceKind::Type pieceKind,
				Boolean collidable,
				Boolean ghost);

	static	Boolean IsBlockCollidable(BlockKind::Type blockKind);

	static	Boolean	IsBlockGhost(BlockKind::Type blockKind);
	
	static	void	RenderPiece(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				Boolean collidable,
				Boolean ghost,
				BlockKind::Type buffer[4][4]);
			
	static	void	StampPieceOntoBoard(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				UInt8 xPosition,
				UInt8 yPosition,
				Boolean collidable,
				Boolean ghost,
				BlockKind::Type boardBuffer[20][10]);
				
	static	Boolean	CheckCollisionWithBoard(
				PieceKind::Type pieceKind,
				PieceOrientation::Type orientation,
				UInt8 xPosition,
				UInt8 yPosition,
				BlockKind::Type boardBuffer[20][10]);

protected:
	// [Piece][Rotation][y][x]
	// Each piece has 4 individual 4x4 bitmaps describing each of its rotations.
	// Rotations are counter clockwise from 0-3
	static	const	Boolean	piecesArray[8][4][4][4];

};
#endif // _H_TetrisPieces
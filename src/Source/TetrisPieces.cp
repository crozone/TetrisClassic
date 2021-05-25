#include "TetrisPieces.h"

UInt8
TetrisPieces::GetPieceIndex(PieceKind::Type pieceKind) {
	return static_cast<UInt8>(pieceKind);
}

PieceKind::Type
TetrisPieces::GetPieceFromBlock(BlockKind::Type blockKind) {
	return static_cast<PieceKind::Type>(blockKind & BlockKind::PieceKindMask);
}

BlockKind::Type
TetrisPieces::GetBlockFromPiece(
	PieceKind::Type pieceKind,
	Boolean collidable,
	Boolean ghost) {
	
	return static_cast<BlockKind::Type>(
		pieceKind
		& (collidable ? BlockKind::CollidableFlag : 0)
		& (ghost ? BlockKind::GhostFlag : 0)
		);	
}

Boolean TetrisPieces::IsBlockCollidable(BlockKind::Type blockKind) {
	return (blockKind & BlockKind::CollidableFlag) > 0;
}

Boolean TetrisPieces::IsBlockGhost(BlockKind::Type blockKind) {
	return (blockKind & BlockKind::GhostFlag) > 0;
}

void
TetrisPieces::RenderPiece(
	PieceKind::Type pieceKind,
	PieceOrientation::Type orientation,
	Boolean collidable,
	Boolean ghost,
	BlockKind::Type buffer[4][4]) {
	
	UInt8 pieceIndex = GetPieceIndex(pieceKind);
	BlockKind::Type blockKind = GetBlockFromPiece(pieceKind, collidable, ghost);
	
	for(int j = 0; j < 4; j++) {
		for(int i = 0; i < 4; i++) {
			if(piecesArray[pieceIndex][orientation][j][i]) {
				buffer[j][i] = blockKind;
			}
			else {
				buffer[j][i] = BlockKind::None;
			}
		}
	}
}

void
TetrisPieces::StampPieceOntoBoard(
	PieceKind::Type pieceKind,
	PieceOrientation::Type orientation,
	UInt8 xPosition,
	UInt8 yPosition,
	Boolean collidable,
	Boolean ghost,
	BlockKind::Type boardBuffer[20][10]) {
	
	// TODO: Make board width and height a constant
	
	UInt8 pieceIndex = GetPieceIndex(pieceKind);
	BlockKind::Type blockKind = GetBlockFromPiece(pieceKind, collidable, ghost);
	
	for(int j = 0; j < 4; j++) {
		int boardY = yPosition + j;
		if(boardY >= 0 && boardY < 20) {
			for(int i = 0; i < 4; i++) {
				int boardX = xPosition + i;
				if(boardX >= 0 && boardY < 10) {
					if(piecesArray[pieceIndex][orientation][j][i]) {
						boardBuffer[j][i] = blockKind;
					}
				}
			}
		}
	}
}

Boolean
TetrisPieces::CheckCollisionWithBoard(
	PieceKind::Type pieceKind,
	PieceOrientation::Type orientation,
	UInt8 xPosition,
	UInt8 yPosition,
	BlockKind::Type boardBuffer[20][10]) {
	
	// TODO: Make board width and height a constant
	
	UInt8 pieceIndex = GetPieceIndex(pieceKind);
	
	for(int j = 0; j < 4; j++) {
		for(int i = 0; i < 4; i++) {
			int boardY = yPosition + j;
			int boardX = xPosition + i;
			if(boardY >= 0 && boardX >= 0 && boardX < 10) {
				Boolean collidable = IsBlockCollidable(boardBuffer[boardY][boardX]);
				
				if(collidable && piecesArray[pieceIndex][orientation][j][i]) {
					// We have a collision with an existing collidable block on the board
					return TRUE;
				}
			}
			else {
				// We have a collision with the edge of the board
				return TRUE;
			}
		}
	}
	
	// No collision
	return FALSE;
}


// [Piece][Rotation][y][x]
// Each piece has 4 individual 4x4 bitmaps describing each of its rotations.
// Rotations are counter clockwise from 0-3
const Boolean TetrisPieces::piecesArray[8][4][4][4] = {
	// Blank Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	},
	// I (Straight) Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0}
		},
		// 2
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0}
		}
	},
	// O (Square) Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		}
	},
	// T Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		}
	},
	// S Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		}
	},
	// Z Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{0, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		}
	},
	// J Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},
		// 2
		{
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0}
		}
	},
	// L Piece
	{
		// 0
		{
			{0, 0, 0, 0},
			{1, 1, 1, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 1
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0}
		},
		// 2
		{
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		// 3
		{
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		}
	}
};
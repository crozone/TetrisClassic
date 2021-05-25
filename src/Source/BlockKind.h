#ifndef _H_BlockKind
#define _H_BlockKind
#pragma once

#include "PieceKind.h"

//
// Define the BlockKind enum.
//
// BlockKind::Type variable = BlockKind::L & BlockKind::CollidableFlag;
//

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
struct BlockKind {
	enum Type {
		None				=	0,				// Transparent
		I					=	PieceKind::I,	// Cyan
		O					=	PieceKind::O,	// Yellow
		T					=	PieceKind::T,	// Purple
		S					=	PieceKind::S,	// Green
		Z					=	PieceKind::Z,	// Red
		J					=	PieceKind::J,	// Blue
		L					=	PieceKind::L,	// Orange
		PieceKindShift		=	0,				// >> 0
		PieceKindMask		=	7 << PieceKindShift,	// 0000000000000111
		CollidableFlag		=	1 << 3,				// 0000000000001000
		GhostFlag			=	1 << 4,				// 0000000000010000
		FlaggedForClear		=	1 << 5,				// 0000000000100000
		ClearCountdownShift	=	6,				// >> 6
		ClearCountdownMask	=	15 << ClearCountdownShift	// 0000001111000000
		
	};
};

#endif // _H_BlockKind

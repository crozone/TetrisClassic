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
		None			=	0,				// Transparent
		I				=	PieceKind::I,	// Cyan
		O				=	PieceKind::O,	// Yellow
		T				=	PieceKind::T,	// Purple
		S				=	PieceKind::S,	// Green
		Z				=	PieceKind::Z,	// Red
		J				=	PieceKind::J,	// Blue
		L				=	PieceKind::L,	// Orange
		PieceKindMask	=	7,				// 00000111
		CollidableFlag	=	8,				// 00001000
		GhostFlag		=	16				// 00010000
	};
};

#endif // _H_BlockKind

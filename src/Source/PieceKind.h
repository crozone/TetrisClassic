#ifndef _H_PieceKind
#define _H_PieceKind
#pragma once

//
// Define the PieceKind enum.
//
// PieceKind::Type variable = PieceKind::L;
//

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

#endif // _H_PieceKind

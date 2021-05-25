#ifndef _H_PieceOrientation
#define _H_PieceOrientation
#pragma once

//
// Define the PieceKind enum.
//
// PieceKind::Type variable = PieceKind::L;
//

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
struct PieceOrientation {
	enum Type {
		Down = 0,
		Right = 1,
		Up = 2,
		Left = 3
	};
	
	static	PieceOrientation::Type	RotateCW(PieceOrientation::Type orientation);
	
	static	PieceOrientation::Type	RotateCCW(PieceOrientation::Type orientation);
};

#endif // _H_PieceOrientation

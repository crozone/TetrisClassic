#ifndef _H_TetrisPiecesArrays
#define _H_TetrisPiecesArrays
#pragma once

class TetrisPiecesArrays {

public:
	// [Piece][Rotation][y][x]
	// Each piece has 4 individual 4x4 bitmaps describing each of its rotations.
	// Rotations are counter clockwise from 0-3
	static const Boolean NRSLeftPiecesArray[8][4][4][4];
};

#endif // _H_TetrisPiecesArrays
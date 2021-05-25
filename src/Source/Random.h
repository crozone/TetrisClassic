#ifndef _H_Random
#define _H_Random
#pragma once

//
// Define the PieceKind enum.
//
// PieceKind::Type variable = PieceKind::L;
//

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
class Random {
	public:
		static SInt32 RandomRange(SInt32 min, SInt32 max);
		
	protected:
};

#endif // _H_Random

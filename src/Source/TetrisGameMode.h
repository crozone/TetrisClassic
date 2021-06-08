#ifndef _H_TetrisGameMode
#define _H_TetrisGameMode
#pragma once

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
struct TetrisGameMode {
	enum Type {
		
		// The standard game mode.
		// The objective is to obtain the highest game score possible.
		// The game either ends after a set number of levels (usually 15),
		// or continues forever ("Endless mode").
		// If the player dies, the game ends, and they receive whatever score
		// they acheived.
		Marathon	=	0,
		
		// Time trial mode.
		// The objective is to clear a set number of lines in the fastest possible time.
		// The game ends after a set number of lines have been cleared.
		// The actual game score makes no difference in this mode.
		// If the player dies, the game ends, and the player receives no score.
		// Aka "40 lines", "Time attack".
		Sprint		=	1,
		
		// Time limit mode.
		// The objective is to get the highest score in a fixed timeframe.
		// The game ends after a fixed time limit, usually 2 or 3 minutes.
		// If the player dies, the game ends, and they they receive no score.
		// Aka "Time trial", "Ultra2min".
		Ultra		=	2
	};
};

#endif // _H_TetrisGameMode
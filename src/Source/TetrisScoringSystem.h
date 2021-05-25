#ifndef _H_TetrisScoringSystem
#define _H_TetrisScoringSystem
#pragma once

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
struct TetrisScoringSystem {
	enum Type {
		OriginalBPS					=	0,
		OriginalSega				=	1,
		OriginalNintendo			=	2,
		TetrisGuidelinesCompatible	=	3
	};
};

#endif // _H_TetrisScoringSystem
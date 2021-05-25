#ifndef _H_LineClearMode
#define _H_LineClearMode
#pragma once

// If we had C++11 we could use enum class, but we have C++98, so we have to make do.
struct LineClearMode {
	enum Type {
		Instant		=	0,
		OneTick		=	1,
		TwoTick		=	2,
		ThreeTick	=	3,
		FourTick	=	4,
		Curtain		=	5,
		Radiate		=	6
	};
};

#endif // _H_CTetrisGameRuleset
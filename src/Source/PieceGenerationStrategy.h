#ifndef _H_PieceGenerationStrategy
#define _H_PieceGenerationStrategy
#pragma once

#include "PieceKind.h"

struct PieceGenerationStrategy {

	// ClassicRandom	:	Every piece is totally random. Similar to NES or GameBoy.
	// ModernPieceBag	:	Set of 7 unqiue pieces are shuffled.
	// TGMPieceBag		:	Tetris Grand Master. Same as Modern, but first piece is always I,J,L,T.
	enum Type {
		ClassicRandom = 0,
		ModernPieceBag = 1,
		TGMPieceBag = 2
	};
	
	static	const	UInt8 ClassicRandomSetSize = 1;
	
	static	const	UInt8 ModernPieceBagSetSize = 7;
	static	const	UInt8 TGMPieceBagSetSize = 7;
	static	const	UInt8 MaxSetSize = 7;
	
	static	UInt8	GetPieceSetSize(PieceGenerationStrategy::Type pieceGenerationStrategy);
	
	static	UInt8	GeneratePieceSet(
						PieceGenerationStrategy::Type pieceGenerationStrategy,
						PieceKind::Type setDestination[]
						);
};

#endif // _H_PieceGenerationStrategy
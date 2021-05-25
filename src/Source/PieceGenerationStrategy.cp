#include "PieceGenerationStrategy.h"

UInt8
PieceGenerationStrategy::GetPieceSetSize(PieceGenerationStrategy::Type pieceGenerationStrategy) {
	
	switch(pieceGenerationStrategy) {
		case PieceGenerationStrategy::ClassicRandom:
			return PieceGenerationStrategy::ClassicRandomSetSize;
		case PieceGenerationStrategy::ModernPieceBag:
			return PieceGenerationStrategy::ModernPieceBagSetSize;
		case PieceGenerationStrategy::TGMPieceBag:
			return PieceGenerationStrategy::TGMPieceBagSetSize;
		default:
			// Unknown strategy
			Throw_(-1);
			return -1;
	}
}
	
UInt8
PieceGenerationStrategy::GeneratePieceSet(
	PieceGenerationStrategy::Type pieceGenerationStrategy,
	PieceKind::Type setDestination[]) {
	
	switch(pieceGenerationStrategy) {
		case PieceGenerationStrategy::ClassicRandom:
			
		
		
			return PieceGenerationStrategy::ClassicRandomSetSize;
		case PieceGenerationStrategy::ModernPieceBag:
			return PieceGenerationStrategy::ModernPieceBagSetSize;
		case PieceGenerationStrategy::TGMPieceBag:
			return PieceGenerationStrategy::TGMPieceBagSetSize;
		default:
			// Unknown strategy
			Throw_(-1);
			return -1;
	}	
}
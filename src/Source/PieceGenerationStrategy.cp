#include "PieceGenerationStrategy.h"
#include "RandomHelpers.h"
#include "TetrisPieces.h"

using namespace RandomHelpers;

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
			return 0;
	}
}
	
UInt8
PieceGenerationStrategy::GeneratePieceSet(
	PieceGenerationStrategy::Type pieceGenerationStrategy,
	PieceKind::Type setDestination[]) {
	
	switch(pieceGenerationStrategy) {
		case PieceGenerationStrategy::ClassicRandom:
			PieceKind::Type randomPiece = TetrisPieces::GetPieceFromIndex(
				RandomRange(1, 7)
			);
			
			setDestination[0] = randomPiece;
		
			return PieceGenerationStrategy::ClassicRandomSetSize;
		case PieceGenerationStrategy::ModernPieceBag:
			// Initialize the piece set
			for(int i = 0; i < PieceGenerationStrategy::ModernPieceBagSetSize; i++) {
				setDestination[i] = PieceKind::None;
			}
		
			// Generate a new piece set
			int piecesGenerated = 0;
			while(piecesGenerated < PieceGenerationStrategy::ModernPieceBagSetSize) {
				PieceKind::Type randomPiece = TetrisPieces::GetPieceFromIndex(
					RandomRange(1, 7)
				);
				
				// Check for collision with any of the pieces already generated
				for(int i = 0; i < piecesGenerated; i++) {
					if(setDestination[i] == randomPiece) {
						// We have a collision, generate a new piece and try again
						continue;
					}
				}
				
				// No collision, insert the new piece
				setDestination[piecesGenerated] = randomPiece;
			}
		
			return PieceGenerationStrategy::ModernPieceBagSetSize;
		case PieceGenerationStrategy::TGMPieceBag:
			// TODO - Implement TGM Piece Bag Generation
			Throw_(-1);
			return PieceGenerationStrategy::TGMPieceBagSetSize;
		default:
			// Unknown strategy
			Throw_(-1);
			return 0;
	}	
}
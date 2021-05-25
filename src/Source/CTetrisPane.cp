// Tetris render pane implementation

#include "CTetrisPane.h"

CTetrisPane::CTetrisPane()
: LPane::LPane() {

}

CTetrisPane::CTetrisPane(const SPaneInfo &inPaneInfo)
: LPane::LPane(inPaneInfo) {

}

CTetrisPane::CTetrisPane(LStream *inStream)
: LPane::LPane(inStream) {
	// TODO: Figure out how to deal with different Tetris pane types
	
	CTetrisGame game(0);
	
	for(int i = 0; i < 50; i++) {
		game.DoGameTick();
	}
	
	
	
	game.DoPieceRight();
	game.DoPieceRight();
	game.DoPieceRight();
	game.DoPieceRight();
	
	// Render the board state out to buffer
	game.RenderBoard(mBoardState);
}

CTetrisPane::~CTetrisPane() {
	
}

// LPane
void
CTetrisPane::DrawSelf() {
	Rect frameRect;
	// CalcLocalFrameRect returns the paneÕs frame
	// as a QuickDraw rectangle in local coordinates
	CalcLocalFrameRect(frameRect);
	
	// Define the rectangle in which the piece squares will be drawn
	Rect gameRect = frameRect;
	MacInsetRect(&gameRect, 2, 2);
	
	// TODO:
	// * Learn macOS QuickDraw
	// * Get this method to draw the Tetris gameboard from state
	
	::PenNormal();
	
	// TODO: Make varialbe based on type and size of pane
	UInt8 gameWidth = 10;
	UInt8 gameHeight = 20;
	
	UInt32 scaledSquareWidth = (gameRect.right - gameRect.left) / gameWidth;
	UInt32 scaledSquareHeight = (gameRect.bottom - gameRect.top) / gameHeight;
	
	UInt32 squareWidth = scaledSquareWidth > scaledSquareHeight
						? scaledSquareWidth : scaledSquareHeight;
		
	
	Rect basePieceRect;
	MacSetRect(&basePieceRect, 0, 0, squareWidth, squareWidth);
	MacInsetRect(&basePieceRect, 1, 1);
	MacOffsetRect(&basePieceRect, gameRect.left, gameRect.top);
	
	for(SInt32 j = 0; j < gameHeight; j++) {
		for(SInt32 i = 0; i < gameWidth; i++) {
			BlockKind::Type currentBlock = mBoardState[j][i];
		
			if(currentBlock & BlockKind::CollidableFlag) {
				// Draw a single piece rectangle
				Rect currentPieceRect = basePieceRect;
				// Offset the piece by the current ammount
				MacOffsetRect(
					&currentPieceRect,
					i * squareWidth,
					(gameHeight - j) * squareWidth
					);
					
				// Change colour/style based on piece kind
				PieceKind::Type pieceKind = TetrisPieces::GetPieceFromBlock(currentBlock);
				
				// TODO: Set colour based on piece kind
				switch(pieceKind) {
					case PieceKind::I:
						break;
					case PieceKind::O:
						break;
					case PieceKind::T:
						break;
					case PieceKind::S:
						break;
					case PieceKind::Z:
						break;
					case PieceKind::J:
						break;
					case PieceKind::L:
						break;
				}
				
				// Paint the rectangle
				::PaintRect(&currentPieceRect);
			}
		}
	}
		
	// Draws boarder
	::MacFrameRect(&gameRect);
	
	// Draws boarder
	::MacFrameRect(&frameRect);
	
	// Draw temp standin cross
	::MoveTo(frameRect.left, frameRect.top);
	::LineTo(frameRect.right, frameRect.bottom);
	::MoveTo(frameRect.right, frameRect.top);
	::LineTo(frameRect.left, frameRect.bottom);
}

// LListener
void
CTetrisPane::ListenToMessage( MessageT inMessage, void *ioParam ) {
	if(inMessage == 2000) {
		// ioParam is a TetrisGame
		CTetrisGame* game = static_cast<CTetrisGame*>(ioParam);
		
		// Render the board state out to buffer
		game->RenderBoard(mBoardState);
		
	}
}
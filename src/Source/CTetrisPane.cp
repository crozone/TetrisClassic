// Tetris render pane implementation

#include "CTetrisPane.h"

CTetrisPane::CTetrisPane()
: LPane::LPane() {

}

CTetrisPane::CTetrisPane(const SPaneInfo &inPaneInfo)
: LPane::LPane(inPaneInfo) {
	// Initialize board buffer
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			mBoardState[j][i] = BlockKind::None;
		}
	}
}

CTetrisPane::CTetrisPane(LStream *inStream)
: LPane::LPane(inStream) {
	// TODO: Figure out how to deal with different Tetris pane types
	// with different sizes
	
	// Initialize board buffer
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			mBoardState[j][i] = BlockKind::None;
		}
	}
}

CTetrisPane::~CTetrisPane() {
	
}

// LPane
void
CTetrisPane::DrawSelf() {
	// TODO: Make varialbe based on type and size of pane
	UInt8 gameWidth = 10;
	UInt8 gameHeight = 20;

	Rect frameRect;
	// CalcLocalFrameRect returns the paneÕs frame
	// as a QuickDraw rectangle in local coordinates
	
	CalcLocalFrameRect(frameRect);
	
	// Define the rectangle in which the piece squares will be drawn
	Rect gameRect = frameRect;
	MacInsetRect(&gameRect, 2, 2);
	
	UInt32 scaledSquareWidth = (gameRect.right - gameRect.left) / gameWidth;
	UInt32 scaledSquareHeight = (gameRect.bottom - gameRect.top) / gameHeight;
	
	UInt32 squareEdge = scaledSquareWidth < scaledSquareHeight
						? scaledSquareWidth : scaledSquareHeight;
	
	// Pane size scales with its height
	//UInt32 squareEdge = scaledSquareHeight;
						
	// Tweak game rectangle to match exactly the width and height of
	// a block
	gameRect.right = gameRect.left + 10 * squareEdge;
	gameRect.bottom = gameRect.top + 20 * squareEdge;
	
	::PenNormal();
	
	// Draw temp background cross
	::ForeColor(blackColor);
	::MoveTo(gameRect.left, gameRect.top);
	::LineTo(gameRect.right, gameRect.bottom);
	::MoveTo(gameRect.right, gameRect.top);
	::LineTo(gameRect.left, gameRect.bottom);
		
	
	Rect basePieceRect;
	MacSetRect(&basePieceRect, 0, 0, squareEdge, squareEdge);
	//MacInsetRect(&basePieceRect, 1, 1);
	MacOffsetRect(&basePieceRect, gameRect.left, gameRect.bottom - squareEdge);
	
	for(SInt32 j = 0; j < gameHeight; j++) {
		for(SInt32 i = 0; i < gameWidth; i++) {
			BlockKind::Type currentBlock = mBoardState[j][i];
			
			PieceKind::Type pieceKind = TetrisPieces::GetPieceFromBlock(currentBlock);
			Boolean blockCollidable = TetrisPieces::IsBlockCollidable(currentBlock);
			Boolean blockGhost = TetrisPieces::IsBlockGhost(currentBlock);
			Boolean flaggedForClear = TetrisPieces::IsFlaggedForClear(currentBlock);
		
			if(pieceKind != PieceKind::None) {
				// Draw a single piece rectangle
				Rect currentPieceRect = basePieceRect;
				// Offset the piece by the current ammount
				MacOffsetRect(
					&currentPieceRect,
					i * squareEdge,
					-j * squareEdge
					);
					
				// Change colour/style based on piece kind
				
				//{basic QuickDraw colors}
				//whiteColor = 30;
				//blackColor = 33;
				//yellowColor = 69;
				//magentaColor = 137;
				//redColor = 205;
				//cyanColor = 273;
				//greenColor = 341;
				//blueColor = 409;
				SInt32 basicColour = 0;
				RGBColor pieceColour = { 0x00, 0x00, 0x00 };
				
				// TODO: Set colour based on piece kind
				switch(pieceKind) {
					case PieceKind::I:
						// I = Cyan
						basicColour = cyanColor;
						pieceColour.red = 0x00;
						pieceColour.green = 0xFF;
						pieceColour.blue = 0xFF;
						break;
					case PieceKind::O:
						// O = Yellow
						basicColour = yellowColor;
						pieceColour.red = 0xFF;
						pieceColour.green = 0xFF;
						pieceColour.blue = 0x00;
						break;
					case PieceKind::T:
						// T = Magenta
						basicColour = magentaColor;
						pieceColour.red = 0xFF;
						pieceColour.green = 0x00;
						pieceColour.blue = 0xFF;
						break;
					case PieceKind::S:
						// S = Green
						basicColour = greenColor;
						pieceColour.red = 0x00;
						pieceColour.green = 0xFF;
						pieceColour.blue = 0x00;
						break;
					case PieceKind::Z:
						// Z = Red
						basicColour = redColor;
						pieceColour.red = 0xFF;
						pieceColour.green = 0x00;
						pieceColour.blue = 0x00;
						break;
					case PieceKind::J:
						// J = Blue
						basicColour = blueColor;
						pieceColour.red = 0x00;
						pieceColour.green = 0x00;
						pieceColour.blue = 0xFF;
						break;
					case PieceKind::L:
						// L = Orange
						// There is no orange basic colour, so use yellow.
						// TODO: Use a pen pattern with yellow and black
						basicColour = yellowColor;
						pieceColour.red = 0xFF;
						pieceColour.green = 0x99;
						pieceColour.blue = 0x00;
						break;
				}
				
				
				
				// Paint the rectangle
				if(blockCollidable) {
					// TODO: Use RGB colours when available
					//::RGBForeColor(&pieceColour);
					::ForeColor(basicColour);
					::PaintRect(&currentPieceRect);
					::ForeColor(blackColor);
					::MacFrameRect(&currentPieceRect);
				}
				
				// Paint the outline
				if(blockGhost) {
					::ForeColor(basicColour);
					::MacFrameRect(&currentPieceRect);
				}
				
				// TODO: Change based on current tick
				if(flaggedForClear) {
					::ForeColor(blackColor);
					::PaintRect(&currentPieceRect);
				}
			}
		}
	}
	
	::ForeColor(blackColor);
	
	Rect boarderRect = gameRect;
	MacInsetRect(&boarderRect, -1, -1);
	
	// Draws game boarder
	::MacFrameRect(&boarderRect);
	
	// Draws frame boarder
	::MacFrameRect(&frameRect);
}

// LListener
void
CTetrisPane::ListenToMessage(MessageT inMessage, void *ioParam) {
	if(inMessage == 2000) {
		// ioParam is a TetrisGame
		CTetrisGame* game = static_cast<CTetrisGame*>(ioParam);
		
		// Render the board state out to buffer
		game->RenderBoard(mBoardState);
		
		// Invalidate the drawing area
		this->Refresh();
	}
}
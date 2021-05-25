// Tetris render pane implementation
//
// The purpose of CTetrisPane is to render graphics related to the Tetris game.
// It listens to game update messages from CTetrisGameRunnerAttachment and
// re-renders its own contents based upon its role and the game state.
//
// The role is set by the User Constant:
//
// 0: The pane will render the main Tetris game board.
// 1: The pane will render the current hold piece.
// >= 10: The pane will render one of the future pieces.
//         The index of the future piece is equal to the User Constant - 10,
//         for example, 10 will render the very next piece,
//         13 will render the fourth next piece.
//

#include "CTetrisPane.h"

CTetrisPane::CTetrisPane()
: LPane::LPane() {

}

CTetrisPane::CTetrisPane(const SPaneInfo &inPaneInfo)
: LPane::LPane(inPaneInfo),
	mDrawRenderPaneBoundingBoxes(FALSE),
	mBuffer(NULL),
	mBufferWidth(0),
	mBufferHeight(0)
{
	InitializeBuffer(10, 20);
}

CTetrisPane::CTetrisPane(LStream *inStream)
: LPane::LPane(inStream),
	mDrawRenderPaneBoundingBoxes(FALSE),
	mBuffer(NULL),
	mBufferWidth(0),
	mBufferHeight(0)
{	
	// Initialize board buffer.
	// This is just a best guess as to the board size based on the current mode
	// this pane is in (for a game board, the standard size is 10x20).
	// If the board is later sent through in a more exotic size, it will be reinitialized.
	SInt32 tetrisPaneKind = this->GetUserCon();
	if(tetrisPaneKind == 0) {
		// Game board
		InitializeBuffer(10, 20);
	}
	else if(tetrisPaneKind == 1 || tetrisPaneKind >= 10) {
		// Hold piece
		InitializeBuffer(4, 4);
	}
}

void
CTetrisPane::InitializeBuffer(UInt8 width, UInt8 height) {
	mBufferWidth = width;
	mBufferHeight = height;
	
	mBuffer = new BlockKind::Type[height * width];
	ThrowIfNil_(mBuffer);
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			mBuffer[j * width + i] = BlockKind::None;
		}
	}
}

CTetrisPane::~CTetrisPane() {
	delete[] mBuffer;
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
	MacInsetRect(&gameRect, 1, 1);
	
	UInt32 scaledSquareWidth = (gameRect.right - gameRect.left) / mBufferWidth;
	UInt32 scaledSquareHeight = (gameRect.bottom - gameRect.top) / mBufferHeight;
	
	// blockWidth is the width and height of each block that makes up pieces and the board.
	// This finds the largest blockwidth that can fit within the width and height of the board.
	UInt32 blockWidth = scaledSquareWidth < scaledSquareHeight
						? scaledSquareWidth : scaledSquareHeight;
	
	// The block's black border thickness.
	// When the block is above a certain size, increase the thickness.
	UInt32 borderWidth = blockWidth > 15 ? 2 : 1;
						
	// Tweak game rectangle to match exactly the width and height of
	// the calculated block width
	gameRect.right = gameRect.left + mBufferWidth * blockWidth - mBufferWidth * borderWidth + borderWidth;
	gameRect.bottom = gameRect.top + mBufferHeight * blockWidth - mBufferHeight * borderWidth + borderWidth;
	
	// Move game rectangle into middle of pane rectangle
	MacOffsetRect(&gameRect,
		static_cast<short>(((frameRect.right - frameRect.left) / 2)
		- ((gameRect.right - gameRect.left) / 2)),
		static_cast<short>(((frameRect.bottom - frameRect.top) / 2)
		- ((gameRect.bottom - gameRect.top) / 2))
		);
	
	::PenNormal();
	
	if(mDrawRenderPaneBoundingBoxes) {
		// Draw debug background cross
		::ForeColor(blackColor);
		::MoveTo(gameRect.left, gameRect.top);
		::LineTo(gameRect.right, gameRect.bottom);
		::MoveTo(gameRect.right, gameRect.top);
		::LineTo(gameRect.left, gameRect.bottom);
	}
		
	
	Rect basePieceRect;
	MacSetRect(&basePieceRect, 0, 0, blockWidth, blockWidth);
	//MacInsetRect(&basePieceRect, 1, 1);
	MacOffsetRect(&basePieceRect, gameRect.left, gameRect.bottom - blockWidth);
	
	
	// Split rendering into several passes.
	//
	// Pass 0: Ghost piece
	// Pass 1: Collision pieces
	// Pass 2: Animation
	
	for(SInt32 pass = 0; pass < 3; pass++) {
		for(SInt32 j = 0; j < mBufferHeight; j++) {
			for(SInt32 i = 0; i < mBufferWidth; i++) {
				BlockKind::Type currentBlock = mBuffer[j * mBufferWidth + i];
				
				PieceKind::Type pieceKind = TetrisPieces::GetPieceFromBlock(currentBlock);
				Boolean blockCollidable = TetrisPieces::IsBlockCollidable(currentBlock);
				Boolean blockGhost = TetrisPieces::IsBlockGhost(currentBlock);
				Boolean flaggedForClear = TetrisPieces::IsFlaggedForClear(currentBlock);
				UInt8	clearCountdown = TetrisPieces::GetClearCountdown(currentBlock);
			
				if(pieceKind != PieceKind::None) {
					// Draw a single piece rectangle
					Rect currentPieceRect = basePieceRect;
					
					// Offset the block
					MacOffsetRect(
						&currentPieceRect,
						i * blockWidth - i * borderWidth,
						-j * blockWidth + j * borderWidth
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
					
					// Set colour based on piece kind
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
					if(blockCollidable && pass == 1) {
						// Draw black box as the border first, then draw inside
						::ForeColor(blackColor);
						::PaintRect(&currentPieceRect);
						
						Rect innerColourRect = currentPieceRect;
						MacInsetRect(&innerColourRect, borderWidth, borderWidth);
						
						// Paint the inside of the block
						// TODO: Use RGB colours when available
						//::RGBForeColor(&pieceColour);
						::ForeColor(basicColour);
						::PaintRect(&innerColourRect);
					
						//// Paint the inside of the block
						//// TODO: Use RGB colours when available
						////::RGBForeColor(&pieceColour);
						//::ForeColor(basicColour);
						//::PaintRect(&currentPieceRect);
						//
						//// Paint the black block border
						//::ForeColor(blackColor);
						//::MacFrameRect(&currentPieceRect);
					}
					
					// TODO: Change based on current tick
					if(flaggedForClear && pass == 2) {
						if(clearCountdown % 2 == 0) {
							::ForeColor(blackColor);
						}
						else {
							::ForeColor(whiteColor);
						}
						::PaintRect(&currentPieceRect);
					}
					
					// Paint the outline
					if(blockGhost && pass == 0) {
						// Draw coloured box as the border first, then draw inside
						// TODO: Use RGB colours when available
						//::RGBForeColor(&pieceColour);
						::ForeColor(basicColour);
						::PaintRect(&currentPieceRect);
						
						Rect innerColourRect = currentPieceRect;
						MacInsetRect(&innerColourRect, borderWidth, borderWidth);
						
						// Paint the inside of the block white
						::ForeColor(whiteColor);
						::PaintRect(&innerColourRect);
					
						//::ForeColor(basicColour);
						//::MacFrameRect(&currentPieceRect);
					}
				}
			}
		}
	}
	
	
	::ForeColor(blackColor);
	
	Rect boarderRect = gameRect;
	MacInsetRect(&boarderRect, -1, -1);
	
	// Draws game boarder
	::MacFrameRect(&boarderRect);
		
	if(mDrawRenderPaneBoundingBoxes) {
		// Draws frame boarder
		::MacFrameRect(&frameRect);
	}
}

// LListener
void
CTetrisPane::ListenToMessage(MessageT inMessage, void *ioParam) {
	switch(inMessage) {
		case 2000:
		// ioParam is a TetrisGame
		CTetrisGame* game = static_cast<CTetrisGame*>(ioParam);
		ThrowIfNil_(game);
		
		mDrawRenderPaneBoundingBoxes = game->GetRuleset()->mDrawRenderPaneBoundingBoxes;
		
		// TODO: Make this a proper enum
		SInt32 tetrisPaneKind = this->GetUserCon();
		
		// Game board
		if(tetrisPaneKind == 0) {
			// Reinitialize the board buffer if the game board
			// is a different dimension
			UInt8 boardWidth = game->GetBoardWidth();
			UInt8 boardHeight = game->GetBoardHeight();
			
			if(boardWidth != mBufferWidth || boardHeight != mBufferHeight) {
				InitializeBuffer(boardWidth, boardHeight);
			}
			
			// Render the game board
			game->RenderBoard(mBuffer, mBufferWidth, mBufferHeight);
			
			// Invalidate the drawing area
			this->Refresh();
		}
		// Hold piece window
		else if(tetrisPaneKind == 1) {
			UInt8 boardWidth = 4;
			UInt8 boardHeight = 4;
			
			if(boardWidth != mBufferWidth || boardHeight != mBufferHeight) {
				InitializeBuffer(boardWidth, boardHeight);
			}
			
			// Render the game board
			game->RenderHoldPiece(mBuffer, mBufferWidth, mBufferHeight);
			
			// Invalidate the drawing area
			this->Refresh();
		}
		// Next piece window
		else if(tetrisPaneKind >= 10) {
			SInt32 nextIndex = tetrisPaneKind - 10;
			UInt8 boardWidth = 4;
			UInt8 boardHeight = 4;
			
			if(boardWidth != mBufferWidth || boardHeight != mBufferHeight) {
				InitializeBuffer(boardWidth, boardHeight);
			}
			
			// Render the game board
			game->RenderBagPiece(nextIndex, mBuffer, mBufferWidth, mBufferHeight);
			
			// Invalidate the drawing area
			this->Refresh();
		}
	break;
	}
}
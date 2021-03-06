#ifndef _H_CTetrisPane
#define _H_CTetrisPane
#pragma once

#include <LPane.h>
#include <LCommander.h>
#include <LPeriodical.h>

#include "CTetrisGame.h"

///
/// A pane that renders Tetris game state, updates it periodically,
/// handles input commands from the keyboard, and broadcasts changes in game state.
///
class CTetrisPane :
	public LPane, 
	public LListener
	{

public:
	// Tetr = Tetris
	// Use at least one capital letter to avoid conflict with
	// CodeWarrior built in class ids
	enum { class_ID = FOUR_CHAR_CODE('Tetr') };

							CTetrisPane();
							CTetrisPane(const SPaneInfo	&inPaneInfo);
							CTetrisPane(LStream			*inStream);

	virtual					~CTetrisPane();
							
	// LPane
	virtual		void		DrawSelf();
								
	// LListener
	virtual void			ListenToMessage( MessageT inMessage, void *ioParam );

protected:
	void	InitializeBuffer(UInt8 width, UInt8 height);
	
	Boolean mDrawRenderPaneBoundingBoxes;
	UInt8	mBufferWidth;
	UInt8	mBufferHeight;
	BlockKind::Type*	mBuffer;
};

#endif // _H_CTetrisPane
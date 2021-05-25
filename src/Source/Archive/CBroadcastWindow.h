#ifndef _H_CBroadcastWindow
#define _H_CBroadcastWindow
#pragma once

#include <LWindow.h>
#include <LBroadcaster.h>

const MessageT	msg_WindowClosed		= 210;	// CBroadcastWindow*

///
/// A Window that broadcasts events that occur to it
///
class CBroadcastWindow : public LWindow, public LBroadcaster {

public:
	// Bwin = Broadcast Window
	// Use at least one capital letter to avoid conflict with
	// CodeWarrior built in class ids
	enum { class_ID = FOUR_CHAR_CODE('Bwin') };

							CBroadcastWindow();

							CBroadcastWindow( const SWindowInfo& inWindowInfo );

							CBroadcastWindow(
									ResIDT				inWINDid,
									UInt32				inAttributes,
									LCommander*			inSuperCommander);

							CBroadcastWindow(
									LCommander*			inSuperCommander,
									const Rect&			inGlobalBounds,
									ConstStringPtr		inTitle,
									SInt16				inProcID,
									UInt32				inAttributes,
									WindowPtr			inBehind);
									
							CBroadcastWindow( LStream* inStream );
							
	// LCommander
	virtual Boolean		ObeyCommand(
								CommandT			inCommand,
								void*				ioParam);
	

	virtual					~CBroadcastWindow();
	

protected:

};

#endif // _H_CBroadcastWindow
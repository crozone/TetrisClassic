#include "CBroadcastWindow.h"

CBroadcastWindow::CBroadcastWindow()
: LWindow::LWindow()
, LBroadcaster::LBroadcaster()
{
}

CBroadcastWindow::CBroadcastWindow( const SWindowInfo& inWindowInfo )
: LWindow::LWindow(inWindowInfo)
, LBroadcaster::LBroadcaster()
{
}

CBroadcastWindow::CBroadcastWindow(
		ResIDT				inWINDid,
		UInt32				inAttributes,
		LCommander*			inSuperCommander)
: LWindow::LWindow(inWINDid, inAttributes, inSuperCommander)
, LBroadcaster::LBroadcaster()
{
}

CBroadcastWindow::CBroadcastWindow(
		LCommander*			inSuperCommander,
		const Rect&			inGlobalBounds,
		ConstStringPtr		inTitle,
		SInt16				inProcID,
		UInt32				inAttributes,
		WindowPtr			inBehind)
: LWindow::LWindow(inSuperCommander, inGlobalBounds, inTitle, inProcID, inAttributes, inBehind)
, LBroadcaster::LBroadcaster()
{
}
									
CBroadcastWindow::CBroadcastWindow( LStream* inStream )
: LWindow::LWindow(inStream)
, LBroadcaster::LBroadcaster()
{
}

Boolean
CBroadcastWindow::ObeyCommand(CommandT inCommand, void* ioParam) {
	if(inCommand == cmd_Close) {
		BroadcastMessage(msg_WindowClosed, this);
		return true;
	}
	
	return false;
}

CBroadcastWindow::~CBroadcastWindow()
{
	// Notify Listeners that the window is closing
	BroadcastMessage(msg_WindowClosed, this);
}
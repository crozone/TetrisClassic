#ifndef _H_PaneHelpers
#define _H_PaneHelpers
#pragma once

namespace PaneHelpers {

	void AttachListenerToMessageBusOnPane(
	LListener* listener,
	LPane* pane);

	void AttachNodesRecursively(
	LPane* pane,
	LBroadcaster* upstreamBroadcaster,
	LListener* upstreamListener,
	bool allowSelfAttach = false);
}

#endif // _H_PaneHelpers

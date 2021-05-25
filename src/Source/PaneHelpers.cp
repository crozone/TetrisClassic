#include "PaneHelpers.h"
#include <stdlib.h>

using namespace PaneHelpers;

void
PaneHelpers::AttachNodesRecursively(
	LPane* pane,
	LBroadcaster* upstreamBroadcaster,
	LListener* upstreamListener,
	bool allowSelfAttach) // allowSelfAttach allows upstreamListener to be added as a listener to upstreamBroadcaster
{	
	//
	// Handle the pane
	//
	
	// Attach upstream broadcaster -> pane listener
	LListener* listener = dynamic_cast<LListener*>(pane);
	if (upstreamBroadcaster != nil && listener != nil) {
		if(allowSelfAttach || upstreamListener != listener) {
			upstreamBroadcaster->AddListener(listener);
		}
	}
	
	// Attach upstream listener -> pane broadcaster
	LBroadcaster* broadcaster = dynamic_cast<LBroadcaster*>(pane);
	if (upstreamListener != nil && broadcaster != nil) {
		if(allowSelfAttach || upstreamBroadcaster != broadcaster) {
			broadcaster->AddListener(upstreamListener);
		}
	}
	
	//
	// Handle the pane's attachments
	//
	
	// Iterate the pane's attachments and check those for LListener and LBroadcaster
	TArray<LAttachment*>* attachments = pane->GetAttachmentsList();
	if(attachments != nil) {
		TArrayIterator<LAttachment*> attachmentsIterator(*attachments);
		LAttachment	*theAttachment = nil;
		while (attachmentsIterator.Next(theAttachment)) {
			// Attach upstream broadcaster -> attachment listener
			LListener* attachmentListener = dynamic_cast<LListener*>(theAttachment);
			if (upstreamBroadcaster != nil && attachmentListener != nil) {
				if(allowSelfAttach || upstreamListener != attachmentListener) {
					upstreamBroadcaster->AddListener(attachmentListener);
				}
			}
			
			// Attach upstream listener -> attachment broadcaster
			LBroadcaster* attachmentBroadcaster = dynamic_cast<LBroadcaster*>(theAttachment);
			if (upstreamListener != nil && attachmentBroadcaster != nil) {
				if(allowSelfAttach || upstreamBroadcaster != attachmentBroadcaster) {
					attachmentBroadcaster->AddListener(upstreamListener);
				}
			}
		}
	}
	
	//
	// Recurse the pane's subpanes (if it's a view)
	//
	
	// Check if this is a view, and if so recursively traverse its subpanes.
	LView* view = dynamic_cast<LView*>(pane);
	if(view != nil) {
		// Search all subpanes
		TArray<LPane*>& subPanes = view->GetSubPanes();
		TArrayIterator<LPane*> subPanesIterator(subPanes);
		LPane	*theSub = nil;
		while (subPanesIterator.Next(theSub)) {
			AttachNodesRecursively(theSub, upstreamBroadcaster, upstreamListener, allowSelfAttach);
		}
	}
}

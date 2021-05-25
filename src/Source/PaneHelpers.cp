#include "PaneHelpers.h"
#include <stdlib.h>

using namespace PaneHelpers;

// Recurses the pane and adds the pane and attachments,
// subpanes and attachments, to those subpanes, as listeners if they
// implement LListener.
void
PaneHelpers::AttachBroadcasterToPaneListenersRecursively(
	LBroadcaster* broadcaster,
	LPane* pane)
{	
	ThrowIfNil_(broadcaster);
	ThrowIfNil_(pane);
	
	// Check the current pane to see if it is an LListener
	LListener* listener = dynamic_cast<LListener*>(pane);
	// If the pane is a listener, attach it.
	if (listener != nil) {
		broadcaster->AddListener(listener);
	}
	
	// Check if any of the current pane's attachments are LListeners too, and attach those.
	TArray<LAttachment*>* attachments = pane->GetAttachmentsList();
	if(attachments != nil) {
		TArrayIterator<LAttachment*> attachmentsIterator(*attachments);
		LAttachment	*theAttachment = nil;
		while (attachmentsIterator.Next(theAttachment)) {
			LListener* attachmentListener = dynamic_cast<LListener*>(theAttachment);
			if(attachmentListener != nil) {
				broadcaster->AddListener(attachmentListener);
			}
		}
	}
	
	
	// Check if this is a view, and if so recursively traverse its subpanes.
	LView* view = dynamic_cast<LView*>(pane);
	if(view != nil) {
		// Search all subpanes
		TArray<LPane*>& subPanes = view->GetSubPanes();
		TArrayIterator<LPane*> subPanesIterator(subPanes);
		LPane	*theSub = nil;
		while (subPanesIterator.Next(theSub)) {
			AttachBroadcasterToPaneListenersRecursively(broadcaster, theSub);
		}
	}
}
//
// The purpose of CMessageBusAttachment is to receive and retransmit game messages from all
// LBroadcasters in the view hierarchy to all LListeners in the view hierarchy, as well as
// also relaying the messages to other message bus nodes (eg in other windows).
// CMessageBusAttachment will automatically recurse the view hierarchy and attach to any
// panes or attachments that are broadcasters or listeners.
//
// CMessageBusAttachment is intended to be attached to a pane at the top of a view hierarchy
// that contains subpains with Tetris game attachments. Several CMessageBusAttachments can be
// connected together over several windows in order to facilitate inter-window communication.
//
// When this attachment is initialised, any listeners that exist within subpains of the attached
// pane will be automatically and recursively attached as a broadcast listener
// (via AddRenderPanesAsListeners(), called in EnsureInitialized())
//

#include "CMessageBusAttachment.h"
#include "PaneHelpers.h"

using namespace PaneHelpers;

CMessageBusAttachment::CMessageBusAttachment()
: LAttachment(msg_AnyMessage), LBroadcaster(), LListener(),
	mInitialized(FALSE)
{
}


CMessageBusAttachment::CMessageBusAttachment(LStream*	inStream)
	: LAttachment(inStream), LBroadcaster(), LListener(),
	mInitialized(FALSE)
{
	mMessage = msg_AnyMessage;
}

void
CMessageBusAttachment::EnsureInitialized() {
	if(!mInitialized) {
		AddListenersRecursivelyFromHost();
		// TODO
		// AddBroadcastersRecursivelyFromHost();
		mInitialized = true;
	}
}


void
CMessageBusAttachment::AddListenersRecursivelyFromHost() {
	// The attachable is the host to which this attachment is currently connected.
	LAttachable* attachable = this->GetOwnerHost();
	
	// The attachable is usually a Pane
	LPane* pane = dynamic_cast<LPane*>(attachable);
	
	if(pane == nil) {
		// If we have no LPane host, there's nothing to do here.
		return;
	}
	
	// TODO: Check that AttachBroadcasterToPaneListenersRecursively doesn't attach this
	//       instance of CMessageBusAttachment to itself.
	PaneHelpers::AttachBroadcasterToPaneListenersRecursively(this, pane);
}


// LListener
void
CMessageBusAttachment::ListenToMessage(MessageT inMessage, void *ioParam) {
	// TODO
}

// LAttachment
void
CMessageBusAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	switch(inMessage) {
		// UI messages
		case msg_FinishCreate:
			EnsureInitialized();
			// The pane hierarchy has been created, we can initialize now.
		break;
	}
}

#ifndef _H_CMessageBusAttachment
#define _H_CMessageBusAttachment
#pragma once

#include <LAttachment.h>

class	CMessageBusAttachment :
	public LAttachment,
	public LListener,
	public LBroadcaster
	{
public:
	// "GAME MESSAGE BUS ATTACHMENT"
	enum { class_ID = FOUR_CHAR_CODE('GMBA') };

					CMessageBusAttachment();
					CMessageBusAttachment(LStream* inStream);

protected:
	Boolean mInitialized;
	
	void	EnsureInitialized();
	void	AddNodesRecursivelyFromHost();
	
	// LListener
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);
	// LAttachment
	virtual void	ExecuteSelf(MessageT inMessage, void* ioParam);
};

#endif // _H_CMessageBusAttachment
#ifndef _H_CTetrisKeyHandlerAttachment
#define _H_CTetrisKeyHandlerAttachment
#pragma once

#include <LAttachment.h>

class	CTetrisKeyHandlerAttachment :
	public LAttachment,
	public LListener,
	public LPeriodical,
	public LBroadcaster
	{
public:
	enum { class_ID = FOUR_CHAR_CODE('TKHA') };

					CTetrisKeyHandlerAttachment();
					CTetrisKeyHandlerAttachment(LStream* inStream);
					
	// LPeriodical
	virtual		void		SpendTime(const EventRecord& inMacEvent);

protected:
	Boolean mInitialized;
	UInt32 mLastUpdateTime;
	
	Boolean	HandleKeyPress(const EventRecord& inKeyEvent);
	
	void	EnsureInitialized();
	
	// LListener
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);
	// LAttachment
	virtual void	ExecuteSelf(MessageT inMessage, void* ioParam);
};

#endif // _H_CTetrisKeyHandlerAttachment
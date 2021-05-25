#ifndef _H_CTetrisControlAttachment
#define _H_CTetrisControlAttachment
#pragma once

#include <LAttachment.h>
#include "CTetrisGame.h"

class	CTetrisControlAttachment :
	public LAttachment,
	public LListener
	{
public:
	enum { class_ID = FOUR_CHAR_CODE('TClA') };

					CTetrisControlAttachment();
					CTetrisControlAttachment(LStream* inStream);

protected:
	// LListener
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);
	// LAttachment
	virtual void	ExecuteSelf(MessageT inMessage, void* ioParam);
};

#endif // _H_CTetrisControlAttachment
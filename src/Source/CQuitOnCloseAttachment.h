#ifndef _H_CQuitOnCloseAttachment
#define _H_CQuitOnCloseAttachment
#pragma once

#include <LAttachment.h>

class	CQuitOnCloseAttachment : public LAttachment {
public:
	enum { class_ID = FOUR_CHAR_CODE('QoCl') };

					CQuitOnCloseAttachment(LCommander* inCommanderToQuit);
					CQuitOnCloseAttachment(LStream* inStream);

protected:
	LCommander*		mCommanderToQuit;
	virtual void	ExecuteSelf(MessageT inMessage, void* ioParam);
};

#endif // _H_CQuitOnCloseAttachment
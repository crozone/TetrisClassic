#include "CQuitOnCloseAttachment.h"

CQuitOnCloseAttachment::CQuitOnCloseAttachment(
	LCommander* inCommanderToQuit)

	: LAttachment(cmd_Close, FALSE)
{
	mMessage = cmd_Close;
	mCommanderToQuit = inCommanderToQuit;
}


CQuitOnCloseAttachment::CQuitOnCloseAttachment(
	LStream*	inStream)

	: LAttachment(inStream)
{
	mMessage = cmd_Close;
	mCommanderToQuit = LCommander::GetTopCommander();
	ThrowIfNil_(mCommanderToQuit);
}


void
CQuitOnCloseAttachment::ExecuteSelf(
	MessageT	 /*inMessage*/,
	void*		/* ioParam */)
{		
	// Send the Quit command
	mCommanderToQuit->ProcessCommand(cmd_Quit, nil);
}
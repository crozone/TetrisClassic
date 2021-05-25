// ===========================================================================
//	CBasicApp.cp 		©1994-2002 Metrowerks Corporation. All rights reserved.
// ===========================================================================
//	This file contains the starter code for a basic PowerPlant project

#include "CBasicApp.h"

#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDrawingState.h>
#include <UMemoryMgr.h>
#include <URegistrar.h>

#include <LString.h>

#include <LWindow.h>
#include <LCaption.h>
#include <LStdControl.h>
#include <LActiveScroller.h>
#include <UAttachments.h>

#include "CQuitOnCloseAttachment.h"
#include "CTetrisGameRunnerAttachment.h"
#include "CTetrisControlAttachment.h"
#include "CTetrisPane.h"

// ---------------------------------------------------------------------------
//	Constant declarations

const ResIDT	PPob_SampleWindow			= 128;
const PaneIDT	Pane_StartButton			= 3;
const PaneIDT	Pane_PauseButton			= 4;
const PaneIDT	Pane_ResumeButton			= 5;
const PaneIDT	Pane_MessageCaption			= 6;
const PaneIDT	Pane_CharCaption			= 7;
const PaneIDT	Pane_CharCodeCaption		= 8;
const PaneIDT	Pane_KeyCodeCaption			= 9;
const MessageT	msg_StartGame				= 1001;
const MessageT	msg_PauseGame				= 1002;
const MessageT	msg_ResumeGame				= 1003;



// ===========================================================================
//	¥ main
// ===========================================================================

int main()
{							
		// Set Debugging options
	SetDebugThrow_(debugAction_Alert);
	SetDebugSignal_(debugAction_Alert);

		// Initialize Memory Manager. Parameter is the number of
		// master pointer blocks to allocate
	InitializeHeap(3);
	
		// Initialize standard Toolbox managers
	UQDGlobals::InitializeToolbox();
	
		// Install a GrowZone to catch low-memory situations	
	new LGrowZone(20000);

		// Create the application object and run
	CBasicApp	theApp;
	theApp.Run();
	
	return 0;
}


// ---------------------------------------------------------------------------
//	¥ CBasicApp										[public]
// ---------------------------------------------------------------------------
//	Application object constructor

CBasicApp::CBasicApp()
{
	RegisterClasses();
}


// ---------------------------------------------------------------------------
//	¥ ~CBasicApp									[public, virtual]
// ---------------------------------------------------------------------------
//	Application object destructor

CBasicApp::~CBasicApp()
{
	// Nothing
}


// ---------------------------------------------------------------------------
//	¥ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
CBasicApp::StartUp()
{
	// Create primary window
	LWindow* theWindow = LWindow::CreateWindow(PPob_SampleWindow, this);
	ThrowIfNil_(theWindow);
	
	// Link all controls in the window to the application listener
	UReanimator::LinkListenerToBroadcasters(this, theWindow, PPob_SampleWindow);

	this->mPrimaryWindow = theWindow;

	theWindow->Show();
}

// Handle application keypresses
Boolean
CBasicApp::HandleKeyPress(
	const EventRecord& inKeyEvent)
{
	// what: Should be a keyDown event, or a key repeat event (autoKey)
	if(inKeyEvent.what != keyDown && inKeyEvent.what != autoKey) {
		Throw_(-1);
	}
	
	// message:
	// Get the Character code and virtual key code from the low-order word.
	// For Apple Desktop Bus keyboards, the low byte of the high-order word
	// contains the ADB address of the keyboard where the keyboard event occured.
	
	// Get the character code and virtual key code
	UInt8 charCode = inKeyEvent.message & charCodeMask;
	UInt8 keyCode = inKeyEvent.message & keyCodeMask;
	UInt8 adbAddr = inKeyEvent.message & adbAddrMask;
	Boolean keyRepeat = inKeyEvent.what == autoKey;
	
	// when:
	// The time when the event was posted, in ticks since system startup
	
	// where:
	// For low level events (like keydown), contains the location of the
	// mouse cursor in global coordinates.
	// For high level events, contains the event ID.
	
	// Set diagnostic information on the main window
	LWindow* theMainWindow = this->mPrimaryWindow;
	ThrowIfNil_(theMainWindow);
	
	// Find the captions within main window
	LCaption* theCharCaption = dynamic_cast<LCaption*>(theMainWindow->FindPaneByID(Pane_CharCaption));
	LCaption* theCharCodeCaption = dynamic_cast<LCaption*>(theMainWindow->FindPaneByID(Pane_CharCodeCaption));
	LCaption* theKeyCodeCaption = dynamic_cast<LCaption*>(theMainWindow->FindPaneByID(Pane_KeyCodeCaption));
	
	if(theCharCaption != nil) {
		LStr255 charString("Char: ");
		// Append as a character
		charString.Append(static_cast<unsigned char>(charCode));
		if(keyRepeat) {
			charString.Append(" (repeat)");
		}
		theCharCaption->SetDescriptor(charString);
	}

	if(theCharCodeCaption != nil) {
		LStr255 charCodeString("CharCode: ");
		// Append as a character
		charCodeString.Append(static_cast<SInt32>(charCode));
		theCharCodeCaption->SetDescriptor(charCodeString);
	}
	
	if(theKeyCodeCaption != nil) {
		LStr255 keyString("KeyCode: ");
		// Append as a number
		keyString.Append(static_cast<SInt32>(keyCode));
		theKeyCodeCaption->SetDescriptor(keyString);
	}
	
	// Return FALSE so that all other commanders have a chance to
	// process the keycode.	
	return FALSE;
}




// ---------------------------------------------------------------------------
//	¥ ObeyCommand									[public, virtual]
// ---------------------------------------------------------------------------
//	Respond to Commands. Returns true if the Command was handled, false if not.

Boolean
CBasicApp::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;	// Assume we'll handle the command

	switch (inCommand) {
	
		case cmd_New: {

		}

		default: {
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
			break;
		}
	}
	
	return cmdHandled;
}


// ---------------------------------------------------------------------------
//	¥ FindCommandStatus								[public, virtual]
// ---------------------------------------------------------------------------
//	Determine the status of a Command for the purposes of menu updating.

void
CBasicApp::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) {

		case cmd_New: {
			outEnabled = true;
			break;
		}

		default: {
			LApplication::FindCommandStatus(inCommand, outEnabled,
											outUsesMark, outMark, outName);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
//	¥ ListenToMessage								[public, virtual]
// ---------------------------------------------------------------------------
//	Listen to messages directed to this LListener
void
CBasicApp::ListenToMessage(
	MessageT inMessage,
	void *ioParam)
{
	LWindow* theMainWindow = this->mPrimaryWindow;
	ThrowIfNil_(theMainWindow);
	
	//LCommander* topCommander = LCommander::GetTopCommander();
	//ThrowIfNil_(topCommander);
			
	switch (inMessage) {
		case msg_StartGame:
			// TODO: Pass a struct with game details into the ioparams
			theMainWindow->ProcessCommand(msg_StartGame, nil);
		break;
		case msg_PauseGame:
		case msg_ResumeGame:
			theMainWindow->ProcessCommand(inMessage, nil);
		break;
	}
	
	// Find the message caption within this window
	LCaption* theMessageCaption = dynamic_cast<LCaption*>(theMainWindow->FindPaneByID(Pane_MessageCaption));
	
	// Check if the message caption still exists.
	// If the main window has been closed, this may be nil
	if(theMessageCaption != nil) {
		// Set the message
		LStr255 messageString("Message: ");
		messageString.Append(inMessage);
		theMessageCaption->SetDescriptor(messageString);
	}
}


// ---------------------------------------------------------------------------
//	¥ RegisterClasses								[protected]
// ---------------------------------------------------------------------------
//	To reduce clutter within the Application object's constructor, class
//	registrations appear here in this seperate function for ease of use.

void
CBasicApp::RegisterClasses()
{
	RegisterClass_(LWindow);
	RegisterClass_(LView);
	RegisterClass_(LCaption);
	RegisterClass_(LStdButton);
	RegisterClass_(CQuitOnCloseAttachment);
	RegisterClass_(CTetrisGameRunnerAttachment);
	RegisterClass_(CTetrisControlAttachment);
	RegisterClass_(CTetrisPane);
	RegisterClass_(LOffscreenView);
	RegisterClass_(LActiveScroller);
}
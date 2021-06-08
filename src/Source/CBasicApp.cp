//
// CBasicApp is the entry point for the application.
//

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
#include <UControlRegistry.h>
#include <LSlider.h>
#include <LCheckbox.h>
#include <LPopupButton.h>
#include <UAttachments.h>

// The following header files are included so the classes within them can be added with
// the RegisterClass_() macro, within the RegisterClasses() function.
// If you get "syntax errors" while adding new classes with the RegisterClass_() macro,
// ensure you have remembered to include that header here!
#include "CQuitOnCloseAttachment.h"
#include "CTetrisGameRunnerAttachment.h"
#include "CTetrisKeyHandlerAttachment.h"
#include "CTetrisControlAttachment.h"
#include "CMessageBusAttachment.h"
#include "CTetrisPane.h"
#include "TetrisMessage.h"
#include "CTetrisGameRuleset.h"
#include "TetrisGameMode.h"
#include "PaneHelpers.h"

// ---------------------------------------------------------------------------
//	Constant declarations

const ResIDT	PPob_SampleWindow			= 128;
const ResIDT	PPob_GameSetupWindow		= 129;
const ResIDT	PPob_MainGameWindow			= 130;
const ResIDT	PPob_ScoreWindow			= 131;
const ResIDT	PPob_NextPieceWindow		= 132;
const ResIDT	PPob_HoldPieceWindow		= 133;
const ResIDT	PPob_DebugWindow			= 134;


const PaneIDT	Pane_StartButton			= 3;
const PaneIDT	Pane_PauseButton			= 4;
const PaneIDT	Pane_ResumeButton			= 5;
const PaneIDT	Pane_MessageCaption			= 6;
const PaneIDT	Pane_CharCaption			= 7;
const PaneIDT	Pane_CharCodeCaption		= 8;
const PaneIDT	Pane_KeyCodeCaption			= 9;


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

CBasicApp::CBasicApp() :
	mPrimaryWindow(NULL),
	mSetupWindow(NULL),
	mGameWindow(NULL)
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
	// Create setup window
	LWindow* theSetupWindow = LWindow::CreateWindow(PPob_GameSetupWindow, this);
	ThrowIfNil_(theSetupWindow);
	
	this->mSetupWindow = theSetupWindow;
	
	PaneHelpers::AttachListenerToMessageBusOnPane(this, this->mSetupWindow);
	
	// Create game window
	LWindow* theGameWindow = LWindow::CreateWindow(PPob_SampleWindow, this);
	ThrowIfNil_(theGameWindow);

	this->mGameWindow = theGameWindow;
	
	PaneHelpers::AttachListenerToMessageBusOnPane(this, this->mGameWindow);
	
	// Switch to the setup window
	this->SwitchPrimaryWindowTo(this->mSetupWindow);
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
	//
	// This method contains most of the "glue code" that manages the
	// transition between the different top level game windows.
	//
	//
	// For example:
	//
	// * It handles the transition from the game setup window to the game window,
	//   and starts the game
	// * It handles the transition on a game over to the high score entry window
	//   and then backto the game setup window 
	// * It handles the transition from the game setup window to the high score
	//   entry window
	//

	LWindow* theMainWindow = this->mPrimaryWindow;
	ThrowIfNil_(theMainWindow);
	
	//LCommander* topCommander = LCommander::GetTopCommander();
	//ThrowIfNil_(topCommander);
			
	switch (inMessage) {
		//case msg_TetrisNewGame:
			// TODO: Pass a struct with game details into the ioparams
		//	theMainWindow->ProcessCommand(msg_TetrisNewGame, nil);
		//break;
		//case msg_TetrisPauseGame:
		//case msg_TetrisResumeGame:
		//	theMainWindow->ProcessCommand(inMessage, nil);
		//break;
		case msg_SetupWindowNewGameClick:
			// TODO strategy:
			//  * Get the current settings from the setup window
			//  * Create and populate game settings struct
			//  * Hide setup window
			//  * Create and show game window
			//     + Game window interally sets up child windows?
			//  * Send msg_TetrisNewGame command to game window with settings struct
				
			// Create the ruleset
			CTetrisGameRuleset gameRuleset = CTetrisGameRuleset();
			
			//
			// Populate with rules
			//
			
			// Game mode
			LPopupButton* gameModePopup = dynamic_cast<LPopupButton*>(
				this->mSetupWindow->FindPaneByID(12) // TODO: Move to constant
			);
			ThrowIfNil_(gameModePopup);
			
			switch(gameModePopup->GetValue()) {
				case 1:
					gameRuleset.mGameMode = TetrisGameMode::Marathon;
					break;
				case 2:
					gameRuleset.mGameMode = TetrisGameMode::Sprint;
					break;
				case 3:
					gameRuleset.mGameMode = TetrisGameMode::Ultra;
					break;
				default:
					Throw_(-1);
				break;
			}
				
			// Starting level	
			LSlider* startingLevelSlider = dynamic_cast<LSlider*>(
				this->mSetupWindow->FindPaneByID(14) // TODO: Move to constant
			);
			ThrowIfNil_(startingLevelSlider);
			gameRuleset.mStartingLevel = startingLevelSlider->GetValue();
			
			// Hold piece enabled
			LCheckBox* holdPieceCheckbox = dynamic_cast<LCheckBox*>(
				this->mSetupWindow->FindPaneByID(23) // TODO: Move to constant
			);
			ThrowIfNil_(holdPieceCheckbox);
			gameRuleset.mEnableHoldPiece = holdPieceCheckbox->GetValue() ? TRUE : FALSE;
			
			// Next pieces lookahead count
			LSlider* nextPiecesSlider = dynamic_cast<LSlider*>(
				this->mSetupWindow->FindPaneByID(24) // TODO: Move to constant
			);
			ThrowIfNil_(nextPiecesSlider);
			// TODO: Make game logic actually respect mPieceBagLookahead 
			gameRuleset.mPieceBagLookahead = nextPiecesSlider->GetValue();
			
			// Rotation System
			LPopupButton* rotationSystemPopup = dynamic_cast<LPopupButton*>(
				this->mSetupWindow->FindPaneByID(21) // TODO: Move to constant
			);
			ThrowIfNil_(rotationSystemPopup);
			
			switch(rotationSystemPopup->GetValue()) {
				case 1:
					gameRuleset.mScoringSystem = TetrisScoringSystem::OriginalNintendo;
					break;
				case 2:
					gameRuleset.mScoringSystem = TetrisScoringSystem::TetrisGuidelinesCompatible;
					break;
				case 3:
					gameRuleset.mScoringSystem = TetrisScoringSystem::OriginalBPS;
					break;
				case 4:
					gameRuleset.mScoringSystem = TetrisScoringSystem::OriginalSega;
					break;
				default:
					Throw_(-1);
				break;
			}
			
			// TODO: Scoring System
			
			
			// Set the active window to the game window
			this->SwitchPrimaryWindowTo(this->mGameWindow);
			
			// Setup the game, sending the game ruleset to the command handler
			this->mPrimaryWindow->ProcessCommand(msg_TetrisSetupGame, &gameRuleset);
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

void
CBasicApp::SwitchPrimaryWindowTo(LWindow* window) {
	if(this->mPrimaryWindow != NULL) {		
		// Hide current primary window
		this->mPrimaryWindow->Hide();
		
		// Clear current primary window
		this->mPrimaryWindow = NULL;
	}
	
	if(window != NULL) {
		// Set current primary window
		this->mPrimaryWindow = window;
		
		// Show primary window
		this->mPrimaryWindow->Show();
		
		// Activate primary window
		this->mPrimaryWindow->Activate();
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
	RegisterClass_(LOffscreenView);
	RegisterClass_(LActiveScroller);
	
	//
	// UControlRegistry::RegisterClasses() handles the registration of all of the
	// "Appearance" control panes.
	//
	// This includes setting up specific backing implementations for each of the controls,
	// depending on which implementation is best suited for the current platform.
	//
	// The backing implementations differ depending on what macOS version
	// the application is running on, and whether or not the Display Manager(?)
	// is available.
	//
	UControlRegistry::RegisterClasses();
	
	RegisterClass_(CQuitOnCloseAttachment);
	RegisterClass_(CTetrisGameRunnerAttachment);
	RegisterClass_(CTetrisKeyHandlerAttachment);
	RegisterClass_(CTetrisControlAttachment);
	RegisterClass_(CMessageBusAttachment);
	RegisterClass_(CTetrisPane);
}
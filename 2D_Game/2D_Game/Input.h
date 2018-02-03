#ifndef INPUT_H_
#define INPUT_H_

#define WIN32_LEAN_AND_MEAN

class Input;

#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>

#include "Constants.h"
#include "GameError.h"

// For HD mouse.
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE		((USHORT) 0x02)
#endif

namespace InputNS
{
	const int KEYS_ARRAY_LEN = 256;

	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE+ TEXT_IN;
}

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);		// default as 20% of range as deadzone
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;								// Trigger range 0-255
const DWORD MAX_CONTROLLERS = 4;

// Bit corresponding to gamepad buttons in state.Gamepad.wButtons
const DWORD GAMEPAD_DPAD_UP			= 0x0001;
const DWORD GAMEPAD_DPAD_DOWN		= 0x0002;
const DWORD GAMEPAD_DPAD_LEFT		= 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT		= 0x0008;
const DWORD GAMEPAD_START_BUTTON	= 0x0010;
const DWORD GAMEPAD_BACK_BUTTON		= 0x0020;
const DWORD GAMEPAD_LEFT_THUMB		= 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB		= 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER	= 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER	= 0x0200;
const DWORD GAMEPAD_A				= 0x1000;
const DWORD GAMEPAD_B				= 0x2000;
const DWORD GAMEPAD_X				= 0x4000;
const DWORD GAMEPAD_Y				= 0x8000;

struct ControllerState
{
	XINPUT_STATE		state;
	XINPUT_VIBRATION	vibration;
	float				vibrateTimeLeft;		// mSec;
	float				vibrateTimeRight;		// mSec;
	bool				connected;				
};

class Input
{
private:

	bool m_bKeysDown[InputNS::KEYS_ARRAY_LEN];			// True for the specified key, if down.
	bool m_bKeysPressed[InputNS::KEYS_ARRAY_LEN];		// True for the specified key, if pressed.
	std::string m_TextIn;								// user entered text.
	char m_charIn;										// Last character entered.
	bool m_bNewLine;									// True on start of new line.
	
	int m_iMouseX, m_iMouseY;							// Mouse screen coordinates.
	int m_iMouseRawX, m_iMouseRawY;						// HD mouse input.
	RAWINPUTDEVICE m_Rid[1];							// For HD mouse.
	
	bool m_bMouseCaptured;								// True if mouse captured.
	bool m_bMouseLButton;								// True if mouse's left button is down.
	bool m_bMouseMButton;								// True if mouse's middle button is down.
	bool m_bMouseRButton;								// True if mouse's right button is down.
	bool m_bMouseX1Button;								// True if X1 mouse button is down.
	bool m_bMouseX2Button;								// True if X2 mouse button is down.
	ControllerState m_Controllers[MAX_CONTROLLERS];		// State of controllers.

public:

	// Constructor.
	Input();

	// Destructor.
	virtual ~Input();

	// Initialize mouse and controller input.
	// Throws GameError.
	void Initialize(HWND hWnd, bool bCaptured);

	// Save key down.
	void KeyDown(WPARAM wParam);

	// Save key up state.
	void KeyUp(WPARAM wParam);

	// Save the char just entered in textIn string.
	void KeyIn(WPARAM wParam);

	// Returns true if specified VIRTUAL KEY is down, else false.
	bool IsKeyDown(UCHAR vKey) const;

	// Returns true if specified VIRTUAL KEY is pressed in most recent frame.
	bool WasKeyPressed(UCHAR vKey) const;

	// Returns true if any key was pressed in the most recent frame.
	// Key pressed are erased at the end of each frame.
	bool AnyKeyPressed(void) const;

	// Clear the specified key press.
	void ClearKeyPress(UCHAR vKey);

	// Clear specified input buffers where what is any combination of
	// KEYS_DOWN, KEYS_PRESSED, MOUSE< TEXT_IN, or KEYS_MOUSE_TEXT
	void Clear(UCHAR what);

	// Clear key, mouse and text input data.
	void ClearAll(void)
	{
		Clear(InputNS::KEYS_MOUSE_TEXT);
	}

	// Clear text input buffer.
	void ClearTextIn(void) 
	{
		m_TextIn.clear();
	}

	// Returns text input as a string.
	std::string GetTextInt(void)
	{
		return m_TextIn;
	}

	// Returns last character entered.
	char GetCharIn(void)
	{
		return m_charIn;
	}

	// Reads mouse screen position input m_iMouseX, m_iMouseY;
	void MouseIn(LPARAM lParam);

	// Reads raw mouse data into mouseRawX, mouseRawY;
	void MouseRawIn(LPARAM lParam);

	// Save state of mouse button.
	void SetMouseLButton(bool b)
	{
		m_bMouseLButton = b;
	}

	// save state of mouse button.
	void SetMouseMButton(bool b)
	{
		m_bMouseMButton = b;
	}

	// save state of mouse button.
	void SetMouseRButton(bool b)
	{
		m_bMouseRButton = b;
	}

	// Save state of mouse button.
	void SetMouseXButton(WPARAM wParam)
	{
		m_bMouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		m_bMouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	// Return mouse X position.
	int GetMouseX(void) const { return m_iMouseX; }

	// Return mouse Y position.
	int GetMouseY(void) const { return m_iMouseY; }

	// Return raw mouseX movement.
	int GetMouseRawX(void) const { return m_iMouseRawX; }

	int GetMouseRawY(void) const { return m_iMouseRawY; }

	// Return state of left mouse button.
	bool GetMouseLButton(void) const { return m_bMouseLButton; }

	bool GetMouseMButton(void) const { return m_bMouseMButton; }

	bool GetMouseRButton(void) const { return m_bMouseRButton; }

	// Return state of X1 mouse button.
	bool GetMouseX1Button(void) const { return m_bMouseX1Button; }

	bool GetMouseX2Button(void) const { return m_bMouseX2Button; }


	// Save input from connected game controllers.
	void ReadControllers(void);

	// Return state of specified game controller.
	const ControllerState* GetControllerState(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return &m_Controllers[n];
	}

	// Return state of controller n buttons.
	const DWORD GetGamepadButtons(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return m_Controllers[n].state.Gamepad.wButtons;
	}

	// Return state of controller n D-pad Up.
	bool GetGamePadDPadUp(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return ((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_UP) != 0);
	}

	// Return state of controller n D-pad Down.
	bool GetGamePadDPadDown(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}
		
		return ((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_DOWN) != 0);
	}

	// Returns state of controller n D-Pad left.
	bool GetGamePadDPadLeft(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return ((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_LEFT) != 0);
	}

	// Returns state of controller n D-pad Right.
	bool GetGamePadDPadRight(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return bool((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_RIGHT) != 0);
	}

	// Return state of controller n Start button.
	bool GetGamepadStart(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return bool((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_START_BUTTON) != 0);
	}

	bool GetGamepadBack(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
		{
			n = MAX_CONTROLLERS - 1;
		}

		return bool((m_Controllers[n].state.Gamepad.wButtons & GAMEPAD_BACK_BUTTON) != 0);
	}
};
#endif
#include "Input.h"

// Constructor.
Input::Input()
	: m_bNewLine(true)
	, m_TextIn("")
	, m_charIn(0)
	, m_iMouseX(0)
	, m_iMouseY(0)
	, m_iMouseRawX(0)
	, m_iMouseRawY(0)
	, m_bMouseLButton(false)
	, m_bMouseMButton(false)
	, m_bMouseRButton(false)
	, m_bMouseX1Button(false)
	, m_bMouseX2Button(false)
{
	// Clear key down array.
	for(size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; ++i)
	{
		m_bKeysDown[i] = false;
	}

	// Clear key pressed array.
	for(size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; ++i)
	{
		m_bKeysPressed[i] = false;
	}

	for(size_t i = 0; i <  MAX_CONTROLLERS; ++i)
	{
		m_Controllers[i].vibrateTimeLeft = 0.0f;
		m_Controllers[i].vibrateTimeRight = 0.0f;
	}

	
}

// Destructor.
Input::~Input()
{
	if(m_bMouseCaptured)
	{
		ReleaseCapture();				// Release mouse.
	}
}

// Initialize mouse and controller input.
// Set capture = true for mouse.
// Throws game error.
void Input::Initialize(HWND hWnd, bool bCaptured)
{
	try
	{
		m_bMouseCaptured = bCaptured;

		// Register high-definition mouse.
		m_Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		m_Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		m_Rid[0].dwFlags = RIDEV_INPUTSINK;
		m_Rid[0].hwndTarget = hWnd;
		RegisterRawInputDevices(m_Rid, 1, sizeof(m_Rid[0]));

		// Clear controllers state.
		ZeroMemory(m_Controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
	}
	catch(...)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Input System!"));
	}
}

void Input::KeyDown(WPARAM wParam)
{
	// Make sure key code is within buffer range.
	if(wParam < InputNS::KEYS_ARRAY_LEN)
	{
		m_bKeysDown[wParam] = true;
		m_bKeysPressed[wParam] = true;
	}
}

void Input::KeyUp(WPARAM wParam)
{
	if(wParam < InputNS::KEYS_ARRAY_LEN)
	{
		m_bKeysDown[wParam] = false;
	}
}

void Input::KeyIn(WPARAM wParam)
{
	if(m_bNewLine)
	{
		m_TextIn.clear();
		m_bNewLine = false;
	}

	if('\b' == wParam)
	{
		if(m_TextIn.length() > 0)
		{
			m_TextIn.erase(m_TextIn.size() - 1);
		}
	}
	else
	{
		m_TextIn += wParam;
		m_charIn = wParam;
	}

	if('\r' == (char)wParam)
	{
		m_bNewLine = true;
	}
}

bool Input::IsKeyDown(UCHAR vKey) const
{
	if(vKey < InputNS::KEYS_ARRAY_LEN)
	{
		return m_bKeysDown[vKey];
	}
	else
	{
		return false;
	}
}

bool Input::WasKeyPressed(UCHAR vKey) const
{
	if(vKey < InputNS::KEYS_ARRAY_LEN)
	{
		return m_bKeysPressed[vKey];
	}
	else
	{
		return false;
	}
}

bool Input::AnyKeyPressed(void) const
{
	for(size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; ++i)
	{
		if(m_bKeysPressed[i] == true)
		{
			return true;
		}
	}

	return false;
}

void Input::ClearKeyPress(UCHAR vKey)
{
	if(vKey < InputNS::KEYS_ARRAY_LEN)
	{
		m_bKeysPressed[vKey] = false;
	}
}

void Input::Clear(UCHAR what)
{
	if(what & InputNS::KEYS_DOWN)
	{
		for(size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; ++i)
		{
			m_bKeysDown[i] = false;
		}
	}

	if(what & InputNS::KEYS_PRESSED)
	{
		for(size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; ++i)
		{
			m_bKeysPressed[i] = false;
		}
	}

	if(what & InputNS::MOUSE)
	{
		m_iMouseX = 0;
		m_iMouseY = 0;
		m_iMouseRawX = 0;
		m_iMouseRawY = 0;
	}

	if(what & InputNS::TEXT_IN)
	{
		ClearTextIn();
	}
}

void Input::MouseIn(LPARAM lParam)
{
	m_iMouseX = GET_X_LPARAM(lParam);
	m_iMouseY = GET_Y_LPARAM(lParam);
}

void Input::MouseRawIn(LPARAM lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if(raw->header.dwType == RIM_TYPEMOUSE)
	{
		m_iMouseRawX = raw->data.mouse.lLastX;
		m_iMouseRawY = raw->data.mouse.lLastY;
	}
}




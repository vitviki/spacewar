#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// Macros

#define SAFE_DELETE(ptr) { if(ptr) { delete (ptr); (ptr) = nullptr; } }

#define SAFE_RELEASE(ptr) { if(ptr) { (ptr)->Release(); (ptr) = nullptr; } }

#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr) = nullptr; } }

// Constants

const bool FULLSCREEN = FALSE;
const UINT GAME_WIDTH = 640;
const UINT GAME_HEIGHT = 480;

// Game
const double PI = 3.14159265;						// Target frame rate
const float FRAME_RATE = 200.0f;					// Minimum frame rate
const float MIN_FRAME_RATE = 10.0f;					// Minimum desired time for 1 frame
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;		// Maximum time use for calculations.
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

const UCHAR ESC_KEY		= VK_ESCAPE;
const UCHAR ALT_KEY		= VK_MENU;
const UCHAR ENTER_KEY	= VK_RETURN;

#endif
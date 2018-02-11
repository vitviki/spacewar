#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// Macros

#define SAFE_DELETE(ptr) { if(ptr) { delete (ptr); (ptr) = nullptr; } }

#define SAFE_RELEASE(ptr) { if(ptr) { (ptr)->Release(); (ptr) = nullptr; } }

#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr) = nullptr; } }

// Safely delete onLostDevice.
#define SAFE_ON_LOST_DEVICE(ptr) { if(ptr) { ptr->OnLostDevice(); } }

// Safely call OnResetDevice
#define SAFE_ON_RESET_DEVICE(ptr) { if(ptr) { ptr->OnResetDevice(); } }

// Color Defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


// Transparent color magenta
#define TRANSCOLOR SETCOLOR_ARGB(0, 255, 0, 255)


// Constants

const char SHIP_IMAGE[] = "textures\\ship.png";
const char SHIP_2_IMAGE[] = "textures\\ship2.png";
const char NEBULA_IMAGE[] = "textures\\orion.jpg";
const char PLANET_IMAGE[] = "textures\\planet.png";

const char CLASS_NAME[] = "Spacewar";
const char GAME_TITLE[] = "Spacewar";
const bool FULLSCREEN = FALSE;
const UINT GAME_WIDTH = 640;
const UINT GAME_HEIGHT = 480;

const int SHIP_START_FRAME = 0;						// Starting frame of ship animation.
const int SHIP_END_FRAME = 3;						// Last frame of ship animation.
const float SHIP_ANIMATION_DELAY = .2f;				// Time between frames of ship animation.
const int SHIP_COLS = 2;							// Ship texture has 2 columns;
const int SHIP_WIDTH = 32;							// Width of ship image.
const int SHIP_HEIGHT = 32;							// Height of ship image.
const float ROTATION_RATE = 180.0f;					// Degrees per second
const float SCALE_RATE = 0.2f;						// % change per second
const float SHIP_SPEED = 100.0f;					// Pixels per second
const float SHIP_SCALE = 1.5f;						// Starting ship scale.

// Game
const double PI = 3.14159265;						// Target frame rate
const float FRAME_RATE = 200.0f;					// Minimum frame rate
const float MIN_FRAME_RATE = 10.0f;					// Minimum desired time for 1 frame
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;		// Maximum time use for calculations.
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

const UCHAR ESC_KEY			= VK_ESCAPE;
const UCHAR ALT_KEY			= VK_MENU;
const UCHAR ENTER_KEY		= VK_RETURN;
const UCHAR SHIP_LEFT_KEY	= VK_LEFT;
const UCHAR SHIP_RIGHT_KEY	= VK_RIGHT;
const UCHAR	SHIP_UP_KEY		= VK_UP;
const UCHAR SHIP_DOWN_KEY	= VK_DOWN;

#endif
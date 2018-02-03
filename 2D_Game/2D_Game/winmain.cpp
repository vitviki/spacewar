#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>

#include "Spacewar.h"

// Function prototypes
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd );
bool CreateMainWindow(HWND& hWnd, HINSTANCE hInstance, int nCmdShow);
LRESULT WINAPI WinProc(HWND hWnd, UINT, WPARAM wParam, LPARAM lParam);

// Global Variable.	
HINSTANCE hInst;
const char *CLASS_NAME = "WinMain";
const char *GAME_TITLE = "Engine Part 1";

// Graphics Pointer
Spacewar *game = nullptr;
HWND hWnd = nullptr;

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	// Check for memory leak if debug build.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;

	// Init game.
	game = new Spacewar();

	// Create MainWindow
	if(!CreateMainWindow(hWnd, hInstance, nShowCmd))
	{
		return 1;
	}
	
	try
	{
		game->Initialize(hWnd);

		// Main message loop/
		int iDone = 0;
		while(!iDone)
		{
			// PeekMessage.
			// Non-blocking method for checking windows messages.
			if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// Look for quit.
				if(msg.message == WM_QUIT)
				{
					iDone = 1;
				}

				// Decode and passed messages to WinProc
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				game->Run(hWnd);
			}
		}
		SAFE_DELETE(game);
		return msg.wParam;
	}
	catch(const GameError& err)
	{
		game->DeleteAll();
		DestroyWindow(hWnd);
		MessageBox(nullptr, err.GetMessage(), "Error", MB_OK);
	}
	catch(...)
	{
		game->DeleteAll();
		DestroyWindow(hWnd);
		MessageBox(nullptr, "Unknown error occurred in game.", "Error", MB_OK);
	}

	SAFE_DELETE(game);
	return 0;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->MessageHandler(hWnd, msg, wParam, lParam);
}

// Create the window.
bool CreateMainWindow(HWND& _hWnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters.
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// Black background.
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = nullptr;

	// Register window class
	if(RegisterClassEx(&wcx) == 0)
	{
		return false;
	}

	DWORD style;
	if(FULLSCREEN)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	// Create window.
	_hWnd = CreateWindow(CLASS_NAME, GAME_TITLE, style, CW_USEDEFAULT, CW_USEDEFAULT, GAME_WIDTH, GAME_HEIGHT, (HWND)nullptr, (HMENU)nullptr, hInstance, (LPVOID)nullptr);

	// If there was an error message creating window.
	if(!_hWnd)
	{
		return false;
	}

	if(!FULLSCREEN)
	{
		RECT clientRect;
		GetClientRect(_hWnd, &clientRect);

		MoveWindow(_hWnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
	}

	ShowWindow(_hWnd, nCmdShow);

	return true;
}


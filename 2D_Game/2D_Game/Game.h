#ifndef GAME_H_
#define GAME_H_

#include <windows.h>
#include <MMSystem.h>

#include "Graphics.h"
#include "Input.h"
#include "GameError.h"


class Game
{
protected:

	// Common game properties.
	Graphics*			m_pGraphics;				// Pointer to game graphics.
	Input*				m_pInput;					// Pointer to Input manager.
	HWND				m_Hwnd;						// Handle to the game window.
	HRESULT				m_Result;					// Standard return type.
	LARGE_INTEGER		m_TimeStart;				// Performance counter start value.
	LARGE_INTEGER		m_TimeEnd;					// Performance counter end value.
	LARGE_INTEGER		m_TimeFreq;					// Performance counter frequency.
	float				m_fFrameTime;				// Time required for frames.
	float				m_fFPS;						// Frames per second.
	DWORD				m_SleepTime;				// Number of milli-seconds to sleep between frames.
	bool				m_bPaused;					// True if game is paused.
	bool				m_bInitialized;		

public:

	// Constructor.
	Game();

	// Destructor.
	virtual ~Game();

	// Member functions..

	// Windows message handler.
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Initialize the game.
	virtual void Initialize(HWND hWnd);

	// Call run repeatedly by the main message loop in WinMain.
	virtual void Run(HWND);

	// Called when the graphics device is lost.
	// Release all reserved video memory so graphics device may be reset.
	virtual void ReleaseAll(void);

	// Recreate all surfaces and reset all entities.
	virtual void ResetAll(void);

	// Delete all reserved memory.
	virtual void DeleteAll(void);

	// Render game items.
	virtual void RenderGame(void);

	//Handle lost graphics device.
	virtual void HandleLostGraphicsDevice(void);

	// Return pointer to graphics.
	Graphics* GetGraphics(void)
	{
		return m_pGraphics;
	}

	// Return pointer to Input manager.
	Input* GetInput(void)
	{
		return m_pInput;
	}

	// Exit the game.
	void ExitGame(void)
	{
		PostMessage(m_Hwnd, WM_DESTROY, 0, 0);
	}

	// Pure virtual functions declarations.
	// These functions must be written in any class that inherits from Game

	// Update game items.
	virtual void Update(void) = 0;

	// Perform AI
	virtual void AI(void) = 0;

	// Check for collisions.
	virtual void Collisions(void) = 0;

	// Render graphics.
	// Call m_pGraphics->SpriteBegin();
	// Draw Sprite
	// Call m_pGraohics->SpriteEnd();
	virtual void Render(void) = 0;
};

#endif
#include "Game.h"

// Constructor.
Game::Game()
	: m_bPaused(false)
	, m_pGraphics(nullptr)
	, m_bInitialized(false)
{
	m_pInput = new Input();
}

// Destructor.
Game::~Game()
{
	DeleteAll();
	ShowCursor(true);
}

// Windows message handler.
LRESULT Game::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_bInitialized)
	{
		switch(msg)
		{
			case WM_DESTROY:

				PostQuitMessage(0);
				return 0;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				
				m_pInput->KeyDown(wParam);
				return 0;

			case WM_KEYUP:
			case WM_SYSKEYUP:

				m_pInput->KeyUp(wParam);
				return 0;

			case WM_CHAR:

				m_pInput->KeyIn(wParam);
				return 0;

			case WM_MOUSEMOVE:

				m_pInput->MouseIn(lParam);
				return 0;

			case WM_INPUT:

				m_pInput->MouseRawIn(lParam);
				return 0;

			case WM_LBUTTONDOWN:

				m_pInput->SetMouseLButton(true);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_LBUTTONUP:

				m_pInput->SetMouseLButton(false);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_MBUTTONDOWN:

				m_pInput->SetMouseMButton(true);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_MBUTTONUP:

				m_pInput->SetMouseMButton(false);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_RBUTTONDOWN:

				m_pInput->SetMouseRButton(true);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_RBUTTONUP:

				m_pInput->SetMouseRButton(false);
				m_pInput->MouseIn(lParam);
				return 0;

			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				
				m_pInput->SetMouseXButton(wParam);
				m_pInput->MouseIn(lParam);
				return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Initialize the game.
void Game::Initialize(HWND hWnd)
{
	m_Hwnd = hWnd;

	m_pGraphics = new Graphics;
	m_pGraphics->Initialize(m_Hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	m_pInput->Initialize(hWnd, false);

	// Attempt to set high resolution timer.
	if(QueryPerformanceFrequency(&m_TimeFreq) == false)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing high resolution timer!"));
	}

	QueryPerformanceCounter(&m_TimeStart);

	m_bInitialized = true;
}

// Render game.
void Game::RenderGame(void)
{
	// start rendering.
	if(SUCCEEDED(m_pGraphics->BeginScene()))
	{
		Render();

		// Stop rendering 
		m_pGraphics->EndScene();
	}

	HandleLostGraphicsDevice();

	// Display back buffer
	m_pGraphics->ShowBackBuffer();
}

// Handle lost graphics device.
void Game::HandleLostGraphicsDevice(void)
{
	m_Result = m_pGraphics->GetDeviceState();
	if(FAILED(m_Result))
	{
		// If the device is lost and not available for reset.
		if(m_Result == D3DERR_DEVICELOST)
		{
			Sleep(100);				// yield CPU time.
			return;
		}
		else if(m_Result == D3DERR_DEVICENOTRESET)
		{
			ReleaseAll();
			m_Result = m_pGraphics->Reset();
			if(FAILED(m_Result))
			{
				return;
			}
			ResetAll();
		}
		else
		{
			return;
		}
	}
}

void Game::SetDisplayMode(GraphicsNS::DISPLAY_MODE mode /* = GraphicsNS::TOGGLE */)
{
	ReleaseAll();
	m_pGraphics->ChangeDisplayMode(mode);
	ResetAll();
}

void Game::Run(HWND hWnd)
{
	if(nullptr == m_pGraphics)
	{
		return;
	}

	// Calculate elapsed time of last frame, save in frameTime;
	QueryPerformanceCounter(&m_TimeEnd);
	m_fFrameTime = (float)(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / (float)(m_TimeFreq.QuadPart);

	// Power saving 
	if(m_fFrameTime < MIN_FRAME_TIME)
	{
		m_SleepTime = (DWORD)((MIN_FRAME_TIME - m_fFrameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(m_SleepTime);
		timeEndPeriod(1);
		return;
	}

	if(m_fFrameTime > 0.0f)
	{
		m_fFPS = (m_fFPS * 0.99f) + (0.01f / m_fFrameTime);
	}

	if(m_fFrameTime > MAX_FRAME_TIME)
	{
		m_fFrameTime = MAX_FRAME_TIME;
	}

	m_TimeStart = m_TimeEnd;

	// Update game functions.
	if(!m_bPaused)
	{
		Update();
		AI();
		Collisions();
	}

	RenderGame();

	// if Alt+Enter toggle fullscreen/window.
	if(m_pInput->IsKeyDown(ALT_KEY) && m_pInput->WasKeyPressed(ENTER_KEY))
	{
		SetDisplayMode(GraphicsNS::TOGGLE);
	}

	// If Esc key is pressed, set window mode.
	if(m_pInput->IsKeyDown(ESC_KEY))
	{
		SetDisplayMode(GraphicsNS::WINDOW);
	}

	// Clear all key presses.
	m_pInput->Clear(InputNS::KEYS_PRESSED);
}

void Game::ReleaseAll(void)
{
	
}

void Game::ResetAll(void)
{

}

// Delete all reserved memory.
void Game::DeleteAll(void)
{
	ReleaseAll();
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pInput);
	m_bInitialized = false;
}
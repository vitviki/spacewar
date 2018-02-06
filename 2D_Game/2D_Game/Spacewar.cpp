#include "Spacewar.h"

Spacewar::Spacewar()
{

}

Spacewar::~Spacewar()
{
	ReleaseAll();
}


void Spacewar::Initialize(HWND hWnd)
{
	Game::Initialize(hWnd);
	m_pGraphics->SetBackColor(GraphicsNS::WHITE);

	// Spaceship texture.
	if(!m_ShipTexture.Initialize(m_pGraphics, SHIP_IMAGE))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing ship texture!"));
	}

	// Ship 1
	if(!m_Ship1.Initialize(m_pGraphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &m_ShipTexture))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing ship 1!"));
	}
	m_Ship1.SetX(GAME_WIDTH / 4);
	m_Ship1.SetY(GAME_HEIGHT / 4);
	m_Ship1.SetFrames(SHIP_START_FRAME, SHIP_END_FRAME);
	m_Ship1.SetCurrentFrame(SHIP_START_FRAME);
	m_Ship1.SetFrameDelay(SHIP_ANIMATION_DELAY);

	// Ship 2
	if(!m_Ship2.Initialize(m_pGraphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &m_ShipTexture))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing ship 2!"));
	}
	m_Ship2.SetX(GAME_WIDTH / 4 + SHIP_WIDTH + 5);
	m_Ship2.SetY(GAME_HEIGHT / 4);
	m_Ship2.SetFrames(SHIP_START_FRAME, SHIP_END_FRAME);
	m_Ship2.SetCurrentFrame(SHIP_START_FRAME);
	m_Ship2.SetFrameDelay(SHIP_ANIMATION_DELAY);
	m_Ship2.SetScale(8);

	return;
}

void Spacewar::Update(void)
{
	m_Ship1.Update(m_fFrameTime);
	m_Ship2.Update(m_fFrameTime);
}

void Spacewar::AI(void)
{

}

void Spacewar::Collisions(void)
{

}

void Spacewar::Render(void)
{
	m_pGraphics->SpriteBegin();

	m_Ship1.Draw();
	m_Ship2.Draw();

	m_pGraphics->SpriteEnd();
}

void Spacewar::ReleaseAll(void)
{
	m_ShipTexture.OnLostDevice();
	Game::ReleaseAll();
	return;
}

void Spacewar::ResetAll(void)
{
	m_ShipTexture.OnResetDevice();
	Game::ResetAll();
	return;
}
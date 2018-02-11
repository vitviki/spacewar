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
	// Initialize 'Game' parent class
	Game::Initialize(hWnd);
	m_pGraphics->SetBackColor(GraphicsNS::WHITE);

	// Nebula Texture.
	if(!m_NebulaTexture.Initialize(m_pGraphics, NEBULA_IMAGE))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Nebula Texture!"));
	}

	// Planet texture.
	if(!m_PlanetTexture.Initialize(m_pGraphics, PLANET_IMAGE))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Planet Texture!"));
	}

	// Nebula Image(game object)
	if(!m_Nebula.Initialize(m_pGraphics, 0, 0, 0, &m_NebulaTexture))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Nebula Image!"));
	}

	// Planet.
	if(!m_Planet.Initialize(m_pGraphics, 0, 0, 0, &m_PlanetTexture))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Planet Image!"));
	}

	// Place the planet at the center of the screen.
	m_Planet.SetX(GAME_WIDTH * .5f - m_Planet.GetWidth() * .5f);
	m_Planet.SetY(GAME_HEIGHT * .5f - m_Planet.GetHeight() * .5f);


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
	m_Ship1.SetRotationInDegrees(45);

	// Ship 2
	if(!m_Ship2Texture.Initialize(m_pGraphics, SHIP_2_IMAGE))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing ship 2 texture!"));
	}

	if(!m_Ship2.Initialize(m_pGraphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &m_Ship2Texture))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing ship 2!"));
	}

	m_Ship2.SetX(GAME_WIDTH / 1.5f);
	m_Ship2.SetY(GAME_HEIGHT / 4);
	m_Ship2.SetFrames(SHIP_START_FRAME, SHIP_END_FRAME);
	m_Ship2.SetCurrentFrame(SHIP_START_FRAME);
	m_Ship2.SetFrameDelay(SHIP_ANIMATION_DELAY);
	m_Ship2.SetRotationInDegrees(145);
	return;
}

void Spacewar::Update(void)
{
	// Update ship 1
	{
		// Update the ship movement based on player's input from keyboard
		if(m_pInput->IsKeyDown(SHIP_RIGHT_KEY))
		{
			// Rotate the ship.
			m_Ship1.SetRotationInDegrees(m_Ship1.GetRotationInDegrees() + m_fFrameTime * ROTATION_RATE);
			if(m_Ship1.GetX() > GAME_WIDTH)
			{
				m_Ship1.SetX((float)-m_Ship1.GetWidth());
			}
		}

		if(m_pInput->IsKeyDown(SHIP_LEFT_KEY))
		{
			// Rotate the ship.
			m_Ship1.SetRotationInDegrees(m_Ship1.GetRotationInDegrees() + m_fFrameTime * -ROTATION_RATE);
			if(m_Ship1.GetX() < - m_Ship1.GetWidth())
			{
				m_Ship1.SetX((float)GAME_WIDTH);
			}
		}

		if(m_pInput->IsKeyDown(SHIP_UP_KEY))
		{
			m_Ship1.SetY(m_Ship1.GetY() - m_fFrameTime * SHIP_SPEED);
			if(m_Ship1.GetY() < -m_Ship1.GetHeight())
			{
				m_Ship1.SetY((float)GAME_HEIGHT);
			}
		}

		if(m_pInput->IsKeyDown(SHIP_DOWN_KEY))
		{
			m_Ship1.SetY(m_Ship1.GetY() + m_fFrameTime * SHIP_SPEED);
			if(m_Ship1.GetY() > GAME_HEIGHT)
			{
				m_Ship1.SetY((float)-m_Ship1.GetHeight());
			}
		}
		m_Ship1.Update(m_fFrameTime);
	}

	// Update ship 2
	{
		m_Ship2.Update(m_fFrameTime);
		
		m_Ship2.SetRotationInDegrees((m_Ship2.GetRotationInDegrees() + m_fFrameTime * -ROTATION_RATE));

		// Move ship downwards.
		m_Ship2.SetY(m_Ship2.GetY() + m_fFrameTime * SHIP_SPEED);

		// Change the size of ship.
		m_Ship2.SetScale(m_Ship2.GetScale() - m_fFrameTime * SCALE_RATE);
		if(m_Ship2.GetY() > GAME_HEIGHT)
		{
			m_Ship2.SetY((float)-m_Ship2.GetHeight());
			m_Ship2.SetScale(SHIP_SCALE);
		}
	}
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

	m_Nebula.Draw();
	m_Planet.Draw();
	m_Ship1.Draw();
	m_Ship2.Draw();
	m_pGraphics->SpriteEnd();
}

void Spacewar::ReleaseAll(void)
{
	m_ShipTexture.OnLostDevice();
	m_Ship2Texture.OnLostDevice();
	m_NebulaTexture.OnLostDevice();
	m_PlanetTexture.OnLostDevice();
	Game::ReleaseAll();
	return;
}

void Spacewar::ResetAll(void)
{
	m_ShipTexture.OnResetDevice();
	m_Ship2Texture.OnResetDevice();
	m_PlanetTexture.OnResetDevice();
	m_NebulaTexture.OnResetDevice();
	Game::ResetAll();
	return;
}
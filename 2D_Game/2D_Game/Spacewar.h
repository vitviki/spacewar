#ifndef SPACEWAR_H_
#define SPACEWAR_H_

#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include "TextureManager.h"
#include "Image.h"

// Main game.
class Spacewar : public Game
{
private:

	// variables.
	TextureManager	m_ShipTexture;
	TextureManager	m_Ship2Texture;
	TextureManager	m_PlanetTexture;
	TextureManager	m_NebulaTexture;
	Image			m_Planet, m_Nebula;
	Image			m_Ship1;
	Image			m_Ship2;


public:

	// Constructor.
	Spacewar();

	// Destructor
	virtual ~Spacewar(void);

	// Initialize the game.
	void Initialize(HWND hWnd);
	void Update(void);
	void AI(void);
	void Collisions(void);
	void Render(void);
	void ReleaseAll(void);
	void ResetAll(void);
};

#endif
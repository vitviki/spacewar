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

	return;
}

void Spacewar::Update(void)
{

}

void Spacewar::AI(void)
{

}

void Spacewar::Collisions(void)
{

}

void Spacewar::Render(void)
{

}

void Spacewar::ReleaseAll(void)
{
	Game::ReleaseAll();
	return;
}

void Spacewar::ResetAll(void)
{
	Game::ResetAll();
	return;
}
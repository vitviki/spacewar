#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>

#include "Constants.h"
#include "GameError.h"

// DirectX Pointer types
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9
#define LP_TEXTURE	LPDIRECT3DTEXTURE9
#define LP_SPRITE	LPD3DXSPRITE

// Color Defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace GraphicsNS
{
	// Some common colors.
	// ARGB numbers range from 0 through 255
	// A = Alpha Channel
	const COLOR_ARGB ORANGE		= D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN		= D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY		= D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY    = D3DCOLOR_ARGB(255,128,128,128);
	const COLOR_ARGB OLIVE   = D3DCOLOR_ARGB(255,128,128,  0);
	const COLOR_ARGB PURPLE  = D3DCOLOR_ARGB(255,128,  0,128);
	const COLOR_ARGB MAROON  = D3DCOLOR_ARGB(255,128,  0,  0);
	const COLOR_ARGB TEAL    = D3DCOLOR_ARGB(255,  0,128,128);
	const COLOR_ARGB GREEN   = D3DCOLOR_ARGB(255,  0,128,  0);
	const COLOR_ARGB NAVY    = D3DCOLOR_ARGB(255,  0,  0,128);
	const COLOR_ARGB WHITE   = D3DCOLOR_ARGB(255,255,255,255);
	const COLOR_ARGB YELLOW  = D3DCOLOR_ARGB(255,255,255,  0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255,255,  0,255);
	const COLOR_ARGB RED     = D3DCOLOR_ARGB(255,255,  0,  0);
	const COLOR_ARGB CYAN    = D3DCOLOR_ARGB(255,  0,255,255);
	const COLOR_ARGB LIME    = D3DCOLOR_ARGB(255,  0,255,  0);
	const COLOR_ARGB BLUE    = D3DCOLOR_ARGB(255,  0,  0,255);
	const COLOR_ARGB BLACK   = D3DCOLOR_ARGB(255,  0,  0,  0);
	const COLOR_ARGB FILTER  = D3DCOLOR_ARGB(  0,  0,  0,  0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB( 64,255,255,255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128,255,255,255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	enum DISPLAY_MODE
	{
		TOGGLE,
		FULLSCREEN,
		WINDOW
	};
}

// SpriteData: The properties required by Graphics::DrawSprite to draw a sprite
struct SpriteData
{
	int				iWidth;			// Width of sprite in pixels
	int				iHeight;		// Height of sprite in pixels
	float			fX;				// Screen location. Top left corner of sprite.
	float			fY;				
	float			fScale;			// <1 smaller, >1 bigger
	float			fAngle;			// Rotation angle in radians.
	RECT			rect;			// Used to select an image from larger texture.
	LP_TEXTURE		texture;		// Pointer to texture
	bool			bFlipHorizontal;// True to flip horizontal.
	bool			bFlipVertical;	// True to flip vertical.
};

class Graphics
{
private:

	// DirectX pointers and stuff
	LP_3D					m_Direct3D;
	LP_3DDEVICE				m_Device3D;
	LP_SPRITE				m_Sprite;
	D3DPRESENT_PARAMETERS	m_D3Dpp;
	D3DDISPLAYMODE			m_pMode;

	// Other variables.
	HRESULT					m_Result;
	HWND					m_Hwnd;
	bool					m_bFullScreen;
	int						m_iWidth;
	int						m_iHeight;
	COLOR_ARGB				m_BackColor;

	// For internal purpose only.
	// Initialize D3D presentation parameters.
	void InitD3Dpp(void);

public:

	// Constructor.
	Graphics();

	// Destructor
	virtual ~Graphics();

	// Release all Directx pointers.
	void ReleaseAll(void);

	// Initialize DirectX graphics.
	void Initialize(HWND hWnd, int iWidth, int iHeight, bool bFullscreen);

	// Load the texture into default D3D memory (normal texture use.)
	// For internal engine use only.
	// Use TextureManager class to load game textures.
	HRESULT LoadTextures(const char* pFileName, COLOR_ARGB transColor, UINT& iWidth, UINT& iHeight, LP_TEXTURE& texture);

	// Display back buffer
	HRESULT ShowBackBuffer(void);

	// Checks if the adapter is compatible with BackBuffer
	// Width and refresh rate specified in D3Dpp. 
	bool IsAdapterCompatible(void);

	// Draw the sprite described in SpriteData structure.
	// Color is optional. It is applied as a filter, WHITE is default.
	// Creates a sprite Begin/End pair.
	void DrawSprite(const SpriteData& spriteData, COLOR_ARGB color = GraphicsNS::WHITE);


	void ChangeDisplayMode(GraphicsNS::DISPLAY_MODE mode = GraphicsNS::TOGGLE);

	// Getter functions.
	LP_3D Get3D(void) const				{ return m_Direct3D; }
		
	LP_3DDEVICE Get3DDevice(void) const	{ return m_Device3D; }

	LP_SPRITE GetSprite(void) const		{return m_Sprite; }

	HDC Get_DC(void)	const			{ GetDC(m_Hwnd); }

	// Test for lost device.
	HRESULT GetDeviceState(void);

	bool GetFullScreen(void) { return m_bFullScreen; }

	HRESULT Reset(void);

	void SetBackColor(COLOR_ARGB c)
	{
		m_BackColor = c;
	}

	// Clear backbuffer and BeginScene
	HRESULT BeginScene(void)
	{
		m_Result = E_FAIL;
		if(nullptr == m_Device3D)
		{
			return m_Result;
		}

		// Clear backbuffer to backcolor.
		m_Device3D->Clear(0, nullptr, D3DCLEAR_TARGET, m_BackColor, 1.0f, 0);
		m_Result = m_Device3D->BeginScene();
		return m_Result;
	}

	HRESULT EndScene(void)
	{
		m_Result = E_FAIL;
		if(m_Device3D)
		{
			m_Result = m_Device3D->EndScene();
		}

		return m_Result;
	}

	void SpriteBegin(void)
	{
		m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	void SpriteEnd(void)
	{
		m_Sprite->End();
	}
};

#endif
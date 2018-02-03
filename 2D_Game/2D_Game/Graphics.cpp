#include "Graphics.h"

Graphics::Graphics()
	: m_bFullScreen(FALSE)
	, m_iWidth(GAME_WIDTH)
	, m_iHeight(GAME_HEIGHT)
{
	m_Direct3D = nullptr;
	m_Device3D = nullptr;
	m_Sprite = nullptr;
	m_BackColor = GraphicsNS::BACK_COLOR;
}

Graphics::~Graphics()
{
	ReleaseAll();
}

void Graphics::ReleaseAll()
{
	SAFE_RELEASE(m_Sprite);
	SAFE_RELEASE(m_Device3D);
	SAFE_RELEASE(m_Direct3D);
}

void Graphics::Initialize(HWND hWnd, int iWidth, int iHeight, bool bFullscreen)
{
	m_Hwnd = hWnd;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_bFullScreen = bFullscreen;

	// Initialize Direct3D
	m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_Direct3D == nullptr)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error. Unable to initialize Direct3D"));
	}

	// Initialize D3D Presentation Parameters.
	InitD3Dpp();
	if(m_bFullScreen)
	{
		if(IsAdapterCompatible())
		{
			m_D3Dpp.FullScreen_RefreshRateInHz = m_pMode.RefreshRate;
		}
		else
		{
			throw(GameError(GameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution!"));
		}
	}

	// Determine if graphic card supports hardware texturing and lighting and vertex shaders.
	D3DCAPS9 caps;
	DWORD behavior;

	m_Result = m_Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in hardware, then switch to software vertex processing.
	if((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	m_Result = m_Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_Hwnd, behavior, &m_D3Dpp, &m_Device3D);

	if(FAILED(m_Result))
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error creating Direct3D Device!"));
	}
}

void Graphics::InitD3Dpp(void)
{
	try
	{
		ZeroMemory(&m_D3Dpp, sizeof(m_D3Dpp));

		// Fill in the parameters we need.
		m_D3Dpp.BackBufferWidth = m_iWidth;
		m_D3Dpp.BackBufferHeight = m_iHeight;
		if(m_bFullScreen)
		{
			m_D3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;		 // Use 24 bit color.
		}
		else
		{
			m_D3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		}

		m_D3Dpp.BackBufferCount = 1;
		m_D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_D3Dpp.hDeviceWindow = m_Hwnd;
		m_D3Dpp.Windowed = (!FULLSCREEN);
		m_D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}catch(...)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing D3D Presentation Parameters"));
	}
}

HRESULT Graphics::LoadTextures(const char* pFileName, COLOR_ARGB transColor, UINT& iWidth, UINT& iHeight, LP_TEXTURE& texture)
{
	// Struct for reading file info.
	D3DXIMAGE_INFO info;
	m_Result = E_FAIL;

	try
	{
		if(nullptr == pFileName)
		{
			texture = nullptr;
			return D3DERR_INVALIDCALL;
		}

		// Get width and height from file.
		m_Result = D3DXGetImageInfoFromFile(pFileName, &info);
		if(D3D_OK != m_Result)
		{
			return m_Result;
		}

		m_iWidth = info.Width;
		m_iHeight = info.Height;

		// Create the new texture by loading from file.
		m_Result = D3DXCreateTextureFromFileEx(m_Device3D, pFileName, info.Width, info.Height, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transColor, &info, nullptr, &texture);
	}
	catch(...)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error in Graphics::LoadTexture"));
	}

	return m_Result;
}

void Graphics::DrawSprite(const SpriteData& spriteData, COLOR_ARGB color /* = GraphicsNS::WHITE */)
{
	if(nullptr == spriteData.texture)
	{
		return;
	}

	// Find the center of sprite.
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.iWidth / 2 * spriteData.fScale), (float)(spriteData.iHeight / 2 * spriteData.fScale));		

	// Screen position of sprite.
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.fX, (float)spriteData.fY);

	// Scaling X, Y
	D3DXVECTOR2 scaling(spriteData.fScale, spriteData.fScale);

	if(spriteData.bFlipHorizontal)
	{
		// Negative X scale to flip.
		scaling.x *= -1;
		
		// Get the center of flipped image.
		spriteCenter.x -= (float)(spriteData.iWidth * spriteData.fScale);

		// Flip occurs around left edge, translate to put
		// Flipped image in same location as original.
		translate.x += (float)(spriteData.iWidth * spriteData.fScale);
	}

	if(spriteData.bFlipVertical)
	{
		// Negate y scale to flip.
		scaling.y *= -1;

		// Get center of flipped image.
		spriteCenter.y -= (float)(spriteData.iHeight * spriteData.fScale);

		// Flip occurs around top edge, translate down to put
		// flipped image in same location as original.
		translate.y += (float)(spriteData.iHeight * spriteData.fScale);
	}

	// Create a matrix to rotate, scale, and position our sprite.
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scaling, &spriteCenter, (float)(spriteData.fAngle), &translate);

	// Tell the sprite about the matrix.
	m_Sprite->SetTransform(&matrix);

	// Draw the sprite.
	m_Sprite->Draw(spriteData.texture, &spriteData.rect, nullptr, nullptr, color);
}

HRESULT Graphics::ShowBackBuffer(void)
{
	m_Result = E_FAIL;

	// Display backbuffer to screen.
	m_Result = m_Device3D->Present(nullptr, nullptr, nullptr, nullptr);
	return m_Result;
}

bool Graphics::IsAdapterCompatible(void)
{
	UINT modes = m_Direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_D3Dpp.BackBufferFormat);

	for(UINT i = 0; i < modes; ++i)
	{
		m_Result = m_Direct3D->EnumAdapterModes(D3DADAPTER_DEFAULT, m_D3Dpp.BackBufferFormat, i, &m_pMode);
		if(m_pMode.Height == m_D3Dpp.BackBufferHeight && m_pMode.Width == m_D3Dpp.BackBufferWidth && m_pMode.RefreshRate >= m_D3Dpp.FullScreen_RefreshRateInHz)
		{
			return true;
		}
	}

	return false;
}

// Test for lost device.
HRESULT Graphics::GetDeviceState(void)
{
	m_Result = E_FAIL;
	if(nullptr == m_Device3D)
	{
		return m_Result;
	}
	
	m_Result = m_Device3D->TestCooperativeLevel();
	return m_Result;
}

// Reset Graphics Device.
HRESULT Graphics::Reset(void)
{
	m_Result = E_FAIL;
	InitD3Dpp();
	m_Result = m_Device3D->Reset(&m_D3Dpp);
	return m_Result;
}
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>

#include "Constants.h"
#include "GameError.h"

// DirectX Pointer types
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// Color Defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Graphics
{
private:

	// DirectX pointers and stuff
	LP_3D					m_Direct3D;
	LP_3DDEVICE				m_Device3D;
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

	// Display back buffer
	HRESULT ShowBackBuffer(void);

	// Checks if the adapter is compatible with BackBuffer
	// Width and refresh rate specified in D3Dpp. 
	bool IsAdapterCompatible(void);

	// Getter functions.
	LP_3D Get3D(void) const				{ return m_Direct3D; }
		
	LP_3DDEVICE Get3DDevice(void) const	{ return m_Device3D; }

	HDC Get_DC(void)	const			{ GetDC(m_Hwnd); }

	// Test for lost device.
	HRESULT GetDeviceState(void);

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

};

#endif
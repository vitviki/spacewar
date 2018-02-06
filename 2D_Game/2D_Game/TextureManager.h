#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#define WIN32_LEAN_AND_MEAN

#include "Graphics.h"
#include "Constants.h"

class TextureManager
{
private:

	// TextureManager properties.
	UINT			m_iWidth;			// Width of texture in pixels
	UINT			m_iHeight;			// Height of texture in pixels
	LP_TEXTURE		m_Texture;			// Pointer to texture.
	const char*		m_pFile;			// Texture file name
	Graphics*		m_pGraphics;		// Pointer to graphics
	bool			m_bInitialized;		// True when successfully initialized
	HRESULT			m_Result;

public:

	// Constructor.
	TextureManager();

	// Destructor.
	~TextureManager();

	LP_TEXTURE GetTexture(void) const { return m_Texture; }

	// Returns the texture width.
	UINT GetWidth(void) const { return m_iWidth; }

	// Returns the texture height
	UINT GetHeight(void) const { return m_iHeight; }

	// Initialize the texture.
	virtual bool Initialize(Graphics *pGraphics, const char* pFile);

	// Release resources.
	virtual void OnLostDevice(void);

	// Restore Resources.
	virtual void OnResetDevice(void);

};

#endif
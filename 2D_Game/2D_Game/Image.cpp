#include "Image.h"

// Constructor.
Image::Image()
	: m_bInitialized(false)
	, m_iCols(1)
	, m_pTextureManager(nullptr)
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_iCurrentFrame(0)
	, m_fFrameDelay(1.0f)			// Default to 1 second per frame of animation
	, m_fAnimTimer(0.0f)
	, m_bVisible(true)
	, m_bLoop(true)
	, m_bAnimComplete(false)
	, m_pGraphics(nullptr)
	, m_ColorFilter(GraphicsNS::WHITE)
{
	m_SpriteData.iWidth = 2;
	m_SpriteData.iHeight = 2;
	m_SpriteData.fX = 0.0f;
	m_SpriteData.fY = 0.0f;
	m_SpriteData.fScale = 1.0f;
	m_SpriteData.fAngle = 0.0f;
	m_SpriteData.rect.left = 0;
	m_SpriteData.rect.top = 0;
	m_SpriteData.rect.right = m_SpriteData.iWidth;
	m_SpriteData.rect.bottom = m_SpriteData.iHeight;
	m_SpriteData.texture = nullptr;
	m_SpriteData.bFlipHorizontal = false;
	m_SpriteData.bFlipVertical = false;
}

// Destructor
Image::~Image()
{

}

// Initialize the image.
bool Image::Initialize(Graphics* pGraphics, int iWidth, int iHeight, int iNCols, TextureManager* pTextManager)
{
	try
	{
		m_pGraphics = pGraphics;
		m_pTextureManager = pTextManager;

		m_SpriteData.texture = m_pTextureManager->GetTexture();
		if(iWidth == 0)
		{
			iWidth = m_pTextureManager->GetWidth();			// Use full width of texture
		}
		m_SpriteData.iWidth = iWidth;

		if(iHeight == 0)
		{
			iHeight = m_pTextureManager->GetHeight();
		}
		m_SpriteData.iHeight = iHeight;

		m_iCols = iNCols;
		if(m_iCols == 0)
		{
			m_iCols = 1;
		}

		// Configure spriteData.rect to draw currentFrame.
		m_SpriteData.rect.left = (m_iCurrentFrame % iNCols) * m_SpriteData.iWidth;
		m_SpriteData.rect.right = m_SpriteData.rect.left + m_SpriteData.iWidth;
		m_SpriteData.rect.top = (m_iCurrentFrame / iNCols) * m_SpriteData.iHeight;
		m_SpriteData.rect.bottom = m_SpriteData.rect.top + m_SpriteData.iHeight;
	}
	catch(...)
	{
		return false;
	}

	m_bInitialized = true;
	return true;
}

void Image::Draw(COLOR_ARGB color)
{
	if(!m_bVisible || nullptr == m_pGraphics)
	{
		return;
	}

	// Get fresh texture incase of OnReset() was called.
	m_SpriteData.texture = m_pTextureManager->GetTexture();
	if(GraphicsNS::FILTER == color)								// If draw with filter.
	{
		m_pGraphics->DrawSprite(m_SpriteData, m_ColorFilter);	// Use color filter
	}
	else
	{
		m_pGraphics->DrawSprite(m_SpriteData, color);			// Else use color as filter.
	}
}

void Image::Draw(SpriteData sd, COLOR_ARGB color /* = GraphicsNS::WHITE */)
{
	if(!m_bVisible || nullptr == m_pGraphics)
	{
		return;
	}

	sd.rect = m_SpriteData.rect;
	sd.texture = m_pTextureManager->GetTexture();
	if(GraphicsNS::FILTER == color)
	{
		m_pGraphics->DrawSprite(sd, m_ColorFilter);
	}
	else
	{
		m_pGraphics->DrawSprite(sd, color);
	}
}

void Image::Update(float fFrameTime)
{
	if(m_iEndFrame - m_iStartFrame > 0)			// If animated spite.
	{
		m_fAnimTimer += fFrameTime;
		if(m_fAnimTimer > m_fFrameDelay)
		{
			m_fAnimTimer -= m_fFrameDelay;
			m_iCurrentFrame++;
			if(m_iCurrentFrame < m_iStartFrame || m_iCurrentFrame > m_iStartFrame)
			{
				if(true == m_bLoop)
				{
					m_iCurrentFrame = m_iStartFrame;
				}
				else
				{
					m_iCurrentFrame = m_iEndFrame;
					m_bAnimComplete = true;
				}
			}
			SetRect();							// Set spriteData.rect
		}
	}
}

void Image::SetCurrentFrame(int iFrame)
{
	if(0 < iFrame)
	{
		m_iCurrentFrame = iFrame;
		m_bAnimComplete = false;
		SetRect();
	}
}

// Set m_SpriteData.rect to draw CurrentFrame.
inline void Image::SetRect(void)
{
	m_SpriteData.rect.left = (m_iCurrentFrame % m_iCols) * m_SpriteData.iWidth;
	m_SpriteData.rect.right = m_SpriteData.rect.left + m_SpriteData.iWidth;
	m_SpriteData.rect.top = (m_iCurrentFrame / m_iCols) * m_SpriteData.iHeight;
	m_SpriteData.rect.bottom = m_SpriteData.rect.top + m_SpriteData.iHeight;
}
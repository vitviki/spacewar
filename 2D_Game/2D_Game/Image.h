#ifndef IMAGE_H_
#define IMAGE_H_

#define WIN32_LEAN_AND_MEAN

#include "TextureManager.h"

class Image
{
protected:

	// Image properties.
	Graphics*			m_pGraphics;			// Pointer to graphics.
	TextureManager*		m_pTextureManager;		// Pointer to texture manager;
	SpriteData			m_SpriteData;			// SpriteData contains the data reqiured to draw the image by Graphics::DrawSprite
	COLOR_ARGB			m_ColorFilter;			// Applied as color filter (use WHITE for no color change)
	int					m_iCols;				// Number of cols (1 to n) in multi-frame sprite.
	int					m_iStartFrame;			// First frame of current animation.
	int					m_iEndFrame;			// Last frame of current animation.
	int					m_iCurrentFrame;		// Current frame of animation.
	float				m_fFrameDelay;			// How long between frames of animation.
	float				m_fAnimTimer;			// Animation Timer;
	HRESULT				m_Result;				// Standard return type.
	bool				m_bLoop;				// True to loop frames.
	bool				m_bVisible;				// True when visible.
	bool				m_bInitialized;			// True when successfully initialized.
	bool				m_bAnimComplete;		// True when loop is false and end frame has finished displaying.

public:

	// Constructor.
	Image();

	// Destructor.
	virtual ~Image();

	/*	Getter Function */

	// Return reference to SpriteData structure.
	const virtual SpriteData& GetSpriteInfo(void) const { return m_SpriteData; }

	// Return Visible
	virtual bool GetVisible(void) const { return m_bVisible; }

	// Return X position.
	virtual float GetX(void) const { return m_SpriteData.fX; }

	// Return Y Position.
	virtual float GetY(void) const { return m_SpriteData.fY; }

	// Return Scale factor.
	virtual float GetScale(void) const { return m_SpriteData.fScale; }

	// Return width;
	virtual int GetWidth(void) const { return m_SpriteData.iWidth; }

	// return height.
	virtual int GetHeight(void) const { return m_SpriteData.iHeight; }

	// Return center X
	virtual float GetCenterX(void) const { return m_SpriteData.fX + m_SpriteData.iWidth / 2 * GetScale(); }

	// Return center Y
	virtual float GetCenterY(void) const { return m_SpriteData.fY  + m_SpriteData.iHeight / 2 * GetScale(); }

	// Return rotation angle in degrees
	virtual float GetRotationInDegrees(void) const { return m_SpriteData.fAngle * (180.0f / (float)PI); }

	// Return rotation angle in radians.
	virtual float GetRotationInRadians(void) const { return m_SpriteData.fAngle; }

	// Return delay between frames of animation.
	virtual float GetFrameDelay(void) const { return m_fFrameDelay; }

	// Return number of starting frame.
	virtual int GetStartFrame(void) const { return m_iStartFrame; }

	// Return the end frame.
	virtual int GetEndFrame(void) const { return m_iEndFrame; }

	// Return the current frame
	virtual int GetCurrentFrame(void) const { return m_iEndFrame; }

	// Return RECT structure of Image.
	virtual RECT GetSpriteDataRect(void) const { return m_SpriteData.rect; }

	// Return state of animation.
	virtual bool HasAnimationCompleted(void) const { return m_bAnimComplete; }

	// Return colorFilter
	virtual COLOR_ARGB GetColorFilter(void) const { return m_ColorFilter; }

	/* Setter Functions */

	// Return X position.
	virtual void SetX(float fNewX) { m_SpriteData.fX = fNewX; }

	// Return Y Position.
	virtual void SetY(float fNewY) { m_SpriteData.fY = fNewY; }

	// Return Scale factor.
	virtual void SetScale(float fS) { m_SpriteData.fScale = fS; }

	// Set the angle in degrees
	virtual void SetRotationInDegrees(float fDeg) { m_SpriteData.fAngle = fDeg * ((float)PI / 180.0f); }

	// Set angle in radians.
	virtual void SetAngleInRadians(float fRad) { m_SpriteData.fAngle = fRad; }

	// Set visible.
	virtual void SetVisible(bool bFlag) { m_bVisible = bFlag; }

	// Set delay between frames of animation.
	virtual void SetFrameDelay(float fD) { m_fFrameDelay = fD; }

	// Set starting and ending frames of animation.
	virtual void SetFrames(int iStart, int iEnd) { m_iStartFrame = iStart; m_iEndFrame = iEnd; }

	// Set current frame of animation.
	virtual void SetCurrentFrame(int iCurrent);

	// Set sprite.rect to draw current frame.
	virtual void SetRect(void);

	// Set spriteData.rect to r
	virtual void SetSpriteDataRect(RECT r) { m_SpriteData.rect = r; }

	// Set animation loop.
	virtual void SetLoop(bool bLoop) { m_bLoop = bLoop; }

	// Set animation complete.
	virtual void SetAnimationComplete(bool bComp) { m_bAnimComplete = bComp; }

	// Set color filter.
	virtual void SetColorFilter(COLOR_ARGB color) { m_ColorFilter = color; }

	// Set texture manager.
	virtual void SetTextureManager(TextureManager* pTM) { m_pTextureManager = pTM; }

	
	/* Other functions. */

	// Init image.
	virtual bool Initialize(Graphics* pGraphics, int iWidth, int iHeight, int iNCols, TextureManager* pTextManager);

	// Flip horizontally.
	virtual void FlipHorizontal(bool bFlip) { m_SpriteData.bFlipHorizontal = bFlip; }

	// Flip vertical.
	virtual void FlipVertical(bool bFlip) { m_SpriteData.bFlipVertical = bFlip; }

	// Draw Image using color as filter. Default color white.
	virtual void Draw(COLOR_ARGB color = GraphicsNS::WHITE);

	// Draw this image using the specified SpriteData	
	virtual void Draw(SpriteData sd, COLOR_ARGB color = GraphicsNS::WHITE);

	// Update animation. FrameTime is used to regulate the speed.
	virtual void Update(float fFrameTime);
};
#endif
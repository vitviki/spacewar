#include "TextureManager.h"

// Default constructor.
TextureManager::TextureManager()
	: m_Texture(nullptr)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_pGraphics(nullptr)
	, m_bInitialized(false)
{

}

// Destructor.
TextureManager::~TextureManager()
{
	SAFE_RELEASE(m_Texture);
}

bool TextureManager::Initialize(Graphics *pGraphics, const char* pFile)
{
	try
	{
		m_pGraphics = pGraphics;
		m_pFile = pFile;

		m_Result = m_pGraphics->LoadTextures(m_pFile, TRANSCOLOR, m_iWidth, m_iHeight, m_Texture);

		if(FAILED(m_Result))
		{
			SAFE_RELEASE(m_Texture);
			return false;
		}
	}
	catch(...)
	{
		return false;
	}

	m_bInitialized = true;
	return true;
}

void TextureManager::OnLostDevice(void)
{
	if(!m_bInitialized)
	{
		return;
	}

	SAFE_RELEASE(m_Texture);
}

void TextureManager::OnResetDevice(void)
{
	if(!m_bInitialized)
	{
		return;
	}

	m_pGraphics->LoadTextures(m_pFile, TRANSCOLOR, m_iWidth, m_iHeight, m_Texture);
}
#include "MiniginPCH.h"
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
	: m_Texture{ texture }
	, m_Width{}
	, m_Height{}
{
	SDL_Rect dst{};
	SDL_QueryTexture(m_Texture, nullptr, nullptr, &dst.w, &dst.h);
	m_Width = dst.w;
	m_Height = dst.h;
}

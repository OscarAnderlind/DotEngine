#include "DotRenderer.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <cmath> 
#include <algorithm>

DotRenderer::DotRenderer(SDL_Window* window) : m_sdlRenderer(nullptr)
{
	m_sdlRenderer = SDL_CreateRenderer(window, nullptr);
}

DotRenderer::~DotRenderer()
{
	if (!m_sdlRenderer) return;

	SDL_DestroyRenderer(m_sdlRenderer);
	m_sdlRenderer = nullptr;
}

void DotRenderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (!m_sdlRenderer) return;

	SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, a);
}

void DotRenderer::Clear()
{
	if (!m_sdlRenderer) return;
	
	SDL_RenderClear(m_sdlRenderer);
}

void DotRenderer::Present()
{
	if (!m_sdlRenderer) return;

	SDL_RenderPresent(m_sdlRenderer);
}

void DotRenderer::DrawPoint(int x, int y)
{
	if (!m_sdlRenderer) return;
	
	SDL_RenderPoint(m_sdlRenderer, x, y);
}

void DotRenderer::DrawThreadedFilledCircle(const int centerX, const int centerY, const int radius, const Uint32 colour)
{
	if (!m_sdlRenderer) return;

	for (int i = -radius; i < radius; i++)
	{
		for (int j = -radius; j < radius; j++)
		{
			int index = SDL_clamp((centerX + i) + (centerY + j) * SCREEN_WIDTH, 0, 1920 * 1080);
			pixelBuffer[index] = colour;
		}
	}
	
	
	return;
	
	int x = 0;
	int y = radius;
	int p = 1 - radius;

	while (x <= y)
	{
		int index = SDL_clamp((centerX + x) + (centerY + y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX - x) + (centerY + y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX + x) + (centerY - y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX - x) + (centerY - y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX + x) + (centerY + y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX - x) + (centerY + y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX + x) + (centerY - y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;
		index = SDL_clamp((centerX - x) + (centerY - y) * SCREEN_WIDTH, 0, 1920 * 1080);
		pixelBuffer[index] = colour;

		if (p < 0)
		{
			p += 2 * x + 3;
		}
		else
		{
			p += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}

}


void DotRenderer::SetPixelBuffer(uint32_t* newPixelBuffer)
{
	pixelBuffer = newPixelBuffer;
}

void DotRenderer::DrawFilledCircle(int centerX, int centerY, int radius)
{
	if (!m_sdlRenderer) return;

	for (int y = -radius; y <= radius; y++) 
	{
		int x = static_cast<int>(std::sqrt(radius * radius - y * y));
		SDL_RenderLine(m_sdlRenderer, centerX - x, centerY + y, centerX + x, centerY + y);
	}
}


void DotRenderer::RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
	if (m_sdlRenderer && texture)
	{
		SDL_RenderTexture(m_sdlRenderer, texture, srcRect, dstRect);
	}
}





#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "Game.h"

class DotRenderer
{
public:
    DotRenderer(SDL_Window* window);

    ~DotRenderer();

    SDL_Renderer* GetSDLRenderer() const { return m_sdlRenderer; }

    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void Clear();
    void Present();

    void DrawFilledCircle(int centerX, int centerY, int radius);
    void RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);
    void DrawPoint(int x, int y);
    void DrawThreadedFilledCircle(const int centerX, const int centerY, const int radius, const Uint32 colour);
    void SetPixelBuffer(uint32_t* newPixelBuffer);

private:
    SDL_Renderer* m_sdlRenderer;


    DotRenderer(const DotRenderer&) = delete;
    DotRenderer& operator=(const DotRenderer&) = delete;

    uint32_t* pixelBuffer;
};


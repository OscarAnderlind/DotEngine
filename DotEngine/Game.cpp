#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include <cstdlib>
#include "glm/glm.hpp"
#include <algorithm>
#include "QuadTree.h"



std::vector<Dot*> dots;

SDL_Texture* screenTexture;
std::vector<uint32_t> pixelBuffer;
std::vector<Dot*> toReset;

const int ThreadAmount = std::max(1u, std::thread::hardware_concurrency());


Game::Game(DotRenderer* aRenderer)
{
	renderer = aRenderer;

	pixelBuffer.resize(SCREEN_WIDTH * SCREEN_HEIGHT, 0);
	screenTexture = SDL_CreateTexture(renderer->GetSDLRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	aRenderer->SetPixelBuffer(pixelBuffer.data());

	for (size_t i = 0; i < DotAmount; i++)
	{
		Dot* d = new Dot({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, DotStartingRadius, DotStartingHealth);

		dots.push_back(d);
	}
	theTree = new QuadTree(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), renderer);

}

void Game::Update(float deltaTime)
{
	if (Threading)
	{
		ThreadLoop(deltaTime);
	}
	else
	{
		NonThreadLoop(deltaTime);
	}
}

void Game::ThreadLoop(float deltaTime)
{
	std::fill(pixelBuffer.begin(), pixelBuffer.end(), 0);
	std::vector<std::future<void>> futures;

	theTree->ClearTree();

	for (int i = 0; i < dots.size(); i++)
	{
		theTree->Insert(dots[i]);
	}

	int dotsPerThread = dots.size() / ThreadAmount;

	for (int i = 0; i < ThreadAmount; i++)
	{
		int startIndex = i * dotsPerThread;
		int endIndex = (i == ThreadAmount - 1) ? dots.size() : (i + 1) * dotsPerThread;

		futures.push_back(std::async(std::launch::async, &Game::ThreadDots, this, startIndex, endIndex, deltaTime, pixelBuffer.data()));
	}

	for (int i = 0; i < ThreadAmount; i++)
	{
		futures[i].get();
	}

	for (int i = 0; i < toReset.size(); i++)
	{
		toReset[i]->Reset({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, DotStartingRadius, DotStartingHealth);
	}
	toReset.clear();
	
	SDL_UpdateTexture(screenTexture, NULL, pixelBuffer.data(), SCREEN_WIDTH * sizeof(uint32_t));
	SDL_RenderTexture(renderer->GetSDLRenderer(), screenTexture, NULL, NULL);
}


void Game::ThreadDots(const int& startIndex, const int& endIndex, const float& deltaTime, uint32_t* pixelBuffer)
{
	ThreadRender(deltaTime, startIndex, endIndex, pixelBuffer);
	ThreadLogic(deltaTime, startIndex, endIndex);
}

void Game::ThreadRender(const float& deltaTime, const int& startIndex, const int& endIndex, uint32_t* pixelBuffer)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		if (dots[i] != nullptr)
		{
			dots[i]->RenderThreaded(renderer, deltaTime, pixelBuffer);
		}
	}
}

void Game::ThreadLogic(float deltaTime, const int& startIndex, const int& endIndex)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		std::vector<Dot*> PointsInRange;

		theTree->Query(dots[i]->Position, glm::vec2(SearchSize, SearchSize), PointsInRange);

		if (CollisionMode)
		{
			for (int j = 0; j < PointsInRange.size(); j++)
			{
				float dist = glm::distance(dots[i]->Position, PointsInRange[j]->Position);
				float minDist = dots[i]->Radius + PointsInRange[j]->Radius;

				if (dist < minDist && dots[i] != PointsInRange[j])
				{
					glm::vec2 normal = glm::normalize(PointsInRange[j]->Position - dots[i]->Position);

					dots[i]->Velocity = glm::reflect(dots[i]->Velocity, normal);
					PointsInRange[j]->Velocity = glm::reflect(PointsInRange[j]->Velocity, -normal);

					float overlap = 1.5f * ((minDist + 1) - dist);
					dots[i]->Position -= normal * overlap;
					PointsInRange[j]->Position += normal * overlap;

					dots[i]->Radius += DotRadiusIncrease;
					PointsInRange[j]->Radius += DotRadiusIncrease;

					if (dots[i]->Radius >= DotStartingRadius * DotStartingHealth)
					{
						toReset.push_back(dots[i]);
					}
					if (PointsInRange[j]->Radius >= DotStartingRadius * DotStartingHealth)
					{
						toReset.push_back(PointsInRange[j]);
					}

				}
			}
		}

		dots[i]->Update(deltaTime, PointsInRange);
	}
}
void Game::NonThreadLoop(float deltaTime)
{
	std::vector<Dot*> toReset;
	std::vector<Dot*> PointsInRange;

	theTree->ClearTree();

	for (int i = 0; i < dots.size(); i++)
	{
		theTree->Insert(dots[i]);

		theTree->Query(dots[i]->Position, glm::vec2(SearchSize, SearchSize), PointsInRange);

		if (CollisionMode)
		{
			for (int j = 0; j < PointsInRange.size(); j++)
			{
				float dist = glm::distance(dots[i]->Position, PointsInRange[j]->Position);
				float minDist = dots[i]->Radius + PointsInRange[j]->Radius;

				if (dist < minDist && dots[i] != PointsInRange[j])
				{
					glm::vec2 normal = glm::normalize(PointsInRange[j]->Position - dots[i]->Position);

					dots[i]->Velocity = glm::reflect(dots[i]->Velocity, normal);
					PointsInRange[j]->Velocity = glm::reflect(PointsInRange[j]->Velocity, -normal);

					float overlap = 1.5f * ((minDist + 1) - dist);
					dots[i]->Position -= normal * overlap;
					PointsInRange[j]->Position += normal * overlap;

					dots[i]->Radius += DotRadiusIncrease;
					PointsInRange[j]->Radius += DotRadiusIncrease;

					if (dots[i]->Radius >= DotStartingRadius * DotStartingHealth)
					{
						toReset.push_back(dots[i]);
					}
					if (PointsInRange[j]->Radius >= DotStartingRadius * DotStartingHealth)
					{
						toReset.push_back(PointsInRange[j]);
					}

				}
			}
		}

		dots[i]->Update(deltaTime, PointsInRange);
		dots[i]->Render(renderer, deltaTime);
		PointsInRange.clear();
	}

	for (int i = 0; i < toReset.size(); i++)
	{
		toReset[i]->Reset({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, DotStartingRadius, DotStartingHealth);
	}
	toReset.clear();
}


void Game::CleanUp()
{

}



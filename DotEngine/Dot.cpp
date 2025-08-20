#include "Dot.h"
#include "DotRenderer.h"
#include "Game.h"
#include <random>
#include <glm/gtc/constants.hpp>

const float DotVelocity = 50.0f;
const SDL_PixelFormatDetails* pixelFormatDetails = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);

Dot::Dot(glm::vec2 aPosition, float aRadius, float health)
{
	Position = aPosition;
	Radius = aRadius;

	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
	std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

	Velocity = glm::vec2(dist(rng), dist(rng));

	float angle = dist(rng) * glm::pi<float>() / 100.0f;
	Velocity = glm::vec2(cos(angle), sin(angle));
}

void Dot::Update(float dt, std::vector<Dot*> nearbyDots)
{

	if (BoidMode)
	{
		Velocity += Seperation(nearbyDots) * DotSeparationStrenght;
		Velocity += Alignment(nearbyDots) * DotAlignmentStrenght;
		Velocity += Cohesion(nearbyDots) * DotCohesionStrenght;
		Velocity = glm::normalize(Velocity);
	}

	Position += Velocity * DotVelocity * dt;

	if (Position.x - Radius < 0.0f)
	{
		Position.x = Radius;
		Velocity.x *= -1;
	}
	else if (Position.x + Radius > SCREEN_WIDTH)
	{
		Position.x = SCREEN_WIDTH - Radius;
		Velocity.x *= -1;
	}

	if (Position.y - Radius < 0.0f)
	{
		Position.y = Radius;
		Velocity.y *= -1;
	}
	else if (Position.y + Radius > SCREEN_HEIGHT)
	{
		Position.y = SCREEN_HEIGHT - Radius;
		Velocity.y *= -1;
	}
}

glm::vec2 Dot::Seperation(std::vector<Dot*> nearbyDots)
{
	glm::vec2 returnVec = glm::vec2(0,0);

	for (int i = 0; i < nearbyDots.size(); i++)
	{
		float distance = glm::distance(Position, nearbyDots[i]->Position);

		if (distance < 0.001)
		{
			continue;
		}

		if (distance < DotSeparationContext)
		{
			glm::vec2 tempVector = glm::vec2(0, 0);
			tempVector += Position - nearbyDots[i]->Position;
			tempVector /= (distance * distance);
			returnVec += tempVector;
		}
	}

	return returnVec;
}

glm::vec2 Dot::Alignment(std::vector<Dot*> nearbyDots)
{
	glm::vec2 returnVec = glm::vec2(0, 0);

	for (int i = 0; i < nearbyDots.size(); i++)
	{
		if (glm::distance(Position, nearbyDots[i]->Position) < DotAlignmentContext)
		{
			returnVec += nearbyDots[i]->Velocity;
		}
	}

	returnVec /= nearbyDots.size();
	return returnVec;
}

glm::vec2 Dot::Cohesion(std::vector<Dot*> nearbyDots)
{
	glm::vec2 returnVec = glm::vec2(0, 0);

	for (int i = 0; i < nearbyDots.size(); i++)
	{
		if (glm::distance(Position, nearbyDots[i]->Position) < DotCohesionContext)
		{
			returnVec += nearbyDots[i]->Position - Position;
		}
	}

	returnVec /= nearbyDots.size();

	return returnVec;
}

void Dot::Render(DotRenderer* aRenderer, float dt)
{
	TotalTime += dt;

	float redColor = (glm::cos((TotalTime + Velocity.x * 100) * 0.1f) * 0.5f + 0.5f) * 255.0f;

	float greenColor = (glm::cos((TotalTime + Velocity.y * 100) * 0.9f) * 0.5f + 0.5f) * 255.0f;

	float blueColor = (glm::cos(TotalTime * 0.4f) * 0.5f + 0.5f) * 255.0f;

	aRenderer->SetDrawColor(redColor, greenColor, blueColor, 255);
	aRenderer->DrawFilledCircle(Position.x, Position.y, Radius);
}

void Dot::RenderThreaded(DotRenderer* aRenderer, float dt, uint32_t* pixelBuffer)
{
	TotalTime += dt;

	float redColor = (glm::cos((TotalTime + Velocity.x * 100) * 0.1f) * 0.5f + 0.5f) * 255.0f;

	float greenColor = (glm::cos((TotalTime + Velocity.y * 100) * 0.9f) * 0.5f + 0.5f) * 255.0f;

	float blueColor = (glm::cos(TotalTime * 0.4f) * 0.5f + 0.5f) * 255.0f;

	Uint32 colour = SDL_MapRGBA(pixelFormatDetails, NULL, redColor, greenColor, blueColor, 255.f);

	int xPixel = static_cast<int>(Position.x);
	int yPixel = static_cast<int>(Position.y);

	aRenderer->DrawThreadedFilledCircle(Position.x, Position.y, Radius, colour);
}

void Dot::Reset(glm::vec2 newPosition, float newRadius, int newHealth)
{
	Position.x = newPosition.x;
	Position.y = newPosition.y;
	Radius = newRadius;
}

#pragma once
#include "glm/glm.hpp"
#include <vector>

class DotRenderer;

class Dot
{

private:
	glm::vec2 Seperation(std::vector<Dot*> nearbyDots);
	glm::vec2 Alignment(std::vector<Dot*> nearbyDots);
	glm::vec2 Cohesion(std::vector<Dot*> nearbyDots);

public:

	Dot(glm::vec2 position, float radius, float health);

	void Update(float dt, std::vector<Dot*> nearbyDots);
	void Render(DotRenderer* aRenderer, float dt);
	void TakeDamage(int someDamage);
	void Reset(glm::vec2 newPosition, float newRadius, int newHealth);

	glm::vec2 Position;
	glm::vec2 Velocity;

	float TotalTime = 0;
	float Radius = 0;

};


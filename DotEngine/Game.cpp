#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include <cstdlib>
#include "glm/glm.hpp"
#include <algorithm>
#include "QuadTree.h"



std::vector<Dot*> dots;


Game::Game(DotRenderer* aRenderer)
{
	renderer = aRenderer;

	for (size_t i = 0; i < DotAmount; i++)
	{
		Dot* d = new Dot({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, DotStartingRadius, DotStartingHealth);

		dots.push_back(d);
	}
	theTree = new QuadTree(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), renderer);

}

void Game::Update(float aDeltaTime)
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

		dots[i]->Update(aDeltaTime, PointsInRange);
		dots[i]->Render(renderer, aDeltaTime);
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

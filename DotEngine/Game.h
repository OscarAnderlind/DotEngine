#pragma once
#include "vector"


static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

const int DotAmount = 10000;

const float DotStartingRadius = 0.5;
const float DotRadiusIncrease = 0.5;
const int DotStartingHealth = 3;

const float DotSeparationStrenght = 5;
const float DotAlignmentStrenght = 15;
const float DotCohesionStrenght = 1;

const float DotSeparationContext = 20;
const float DotAlignmentContext = 50;
const float DotCohesionContext = 75;

const float SearchSize = 75; //75 is good for boids, 15 for collision

const bool BoidMode = true;
const bool CollisionMode = true;


class QuadTree;
class DotRenderer;

class Game
{
public:
	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void CleanUp();

private:
	DotRenderer* renderer;

	QuadTree* theTree;
};


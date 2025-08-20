#pragma once
#include "vector"
#include <future>


static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

const int DotAmount = 50000;

const float DotStartingRadius = 0.5;
const float DotRadiusIncrease = 0.1;
const int DotStartingHealth = 3;

const float DotSeparationStrenght = 5;
const float DotAlignmentStrenght = 15;
const float DotCohesionStrenght = 3;

const float DotSeparationContext = 20;
const float DotAlignmentContext = 50;
const float DotCohesionContext = 75;

const float SearchSize = 15; //75 is good for boids, 15 for collision

const bool BoidMode = false;
const bool CollisionMode = true;
const bool Threading = true;


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
	void ThreadLoop(float deltaTime);
	void NonThreadLoop(float deltaTime);
	void ThreadRender(const float& deltaTime, const int& startIndex, const int& endIndex, uint32_t* pixelBuffer);
	void ThreadLogic(float deltaTime, const int& startIndex, const int& endIndex);
	void ThreadDots(const int& startIndex, const int& endIndex, const float& deltaTime, uint32_t* pixelBuffer);
};

